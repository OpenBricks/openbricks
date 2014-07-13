#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/usb/input.h>
#include <asm/unaligned.h>
#include <asm/byteorder.h>

/*
 * Version Information
 *
 */

//#define DUMP_PACKETS

#define DRIVER_VERSION "v1.1"
#define DRIVER_AUTHOR "James McKenzie"
#define DRIVER_DESC "USB Apple MacMini IR Receiver driver"
#define DRIVER_LICENSE "GPL"

MODULE_AUTHOR (DRIVER_AUTHOR);
MODULE_DESCRIPTION (DRIVER_DESC);
MODULE_LICENSE (DRIVER_LICENSE);

#define USB_VENDOR_ID_APPLE	0x05ac
#define USB_DEVICE_ID_APPLE_IR  0x8240
#define USB_DEVICE_ID_APPLE_IR2 0x8242

#define URB_SIZE 32

#define MAX_KEYS 8
#define MAX_KEYS_MASK (MAX_KEYS - 1 )

struct appleir
{
  struct input_dev *dev;
  uint8_t *data;
  dma_addr_t dma_buf;
  struct usb_device *usbdev;
  struct urb *urb;
  int timer_initted;
  struct timer_list key_up_timer;
  int current_key;
  char phys[32];
};


static struct usb_device_id appleir_ids[] = {
  {USB_DEVICE (USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_IR),.driver_info = 0},
  {USB_DEVICE (USB_VENDOR_ID_APPLE, USB_DEVICE_ID_APPLE_IR2),.driver_info = 0},
  {}
};

MODULE_DEVICE_TABLE (usb, appleir_ids);


/* I have two devices both of which report the following */
/* 25 87 ee 83 0a  	+  */
/* 25 87 ee 83 0c  	-  */
/* 25 87 ee 83 09	<< */
/* 25 87 ee 83 06	>> */
/* 25 87 ee 83 05	>" */
/* 25 87 ee 83 03	menu */
/* 26 00 00 00 00	for key repeat*/

/* Thomas Glanzmann reports the following responses */
/* 25 87 ee ca 0b	+  */
/* 25 87 ee ca 0d	-  */
/* 25 87 ee ca 08	<< */
/* 25 87 ee ca 07	>> */
/* 25 87 ee ca 04	>" */
/* 25 87 ee ca 02 	menu */
/* 26 00 00 00 00       for key repeat*/
/* He also observes the following event sometimes */
/* sent after a key is release, which I interpret */
/* as a flat battery message */
/* 25 87 e0 ca 06	flat battery */


static int keymap[MAX_KEYS] = {
  KEY_RESERVED, KEY_MENU,
  KEY_PLAYPAUSE, KEY_NEXTSONG,
  KEY_PREVIOUSSONG, KEY_VOLUMEUP,
  KEY_VOLUMEDOWN, KEY_RESERVED
};

static void
dump_packet (struct appleir *appleir, char *msg, uint8_t * data, int len)
{
  int i;

  printk (KERN_ERR "appleir: %s (%d bytes)", msg, len);

  for (i = 0; i < len; ++i)
    {
      printk (" %02x", data[i]);
    }

  printk ("\n");
}


static void
key_up (struct appleir *appleir, int key)
{
  //printk (KERN_ERR "key %d up\n", key);
  input_report_key (appleir->dev, key, 0);
  input_sync (appleir->dev);
}

static void
key_down (struct appleir *appleir, int key)
{
  //printk (KERN_ERR "key %d down\n", key);
  input_report_key (appleir->dev, key, 1);
  input_sync (appleir->dev);
}

static void
battery_flat (struct appleir *appleir)
{
  printk (KERN_ERR "appleir: possible flat battery?\n");
}

static void
key_up_tick (unsigned long data)
{
  struct appleir *appleir = (struct appleir *) data;

  if (appleir->current_key)
    {
      key_up (appleir, appleir->current_key);
      appleir->current_key = 0;
    }
}

static void
new_data (struct appleir *appleir, uint8_t * data, int len)
{
  static const uint8_t keydown[] = { 0x25, 0x87, 0xee };
  static const uint8_t keyrepeat[] = { 0x26, 0x00, 0x00, 0x00, 0x00 };
  static const uint8_t flatbattery[] = { 0x25, 0x87, 0xe0 };

#ifdef DUMP_PACKETS
  dump_packet (appleir, "received", data, len);
#endif

  if (len != 5)
    return;

  if (!memcmp (data, keydown, sizeof (keydown)))
    {
      /*If we already have a key down, take it up before marking */
      /*this one down */
      if (appleir->current_key)
        key_up (appleir, appleir->current_key);
      appleir->current_key = keymap[(data[4] >> 1) & MAX_KEYS_MASK];

      key_down (appleir, appleir->current_key);
      /*remote doesn't do key up, either pull them up, in the test */
      /*above, or here set a timer which pulls them up after 1/8 s */
      mod_timer (&appleir->key_up_timer, jiffies + HZ / 8);

      return;
    }

  if (!memcmp (data, keyrepeat, sizeof (keyrepeat)))
    {
      key_down (appleir, appleir->current_key);
      /*remote doesn't do key up, either pull them up, in the test */
      /*above, or here set a timer which pulls them up after 1/8 s */
      mod_timer (&appleir->key_up_timer, jiffies + HZ / 8);
      return;
    }

  if (!memcmp (data, flatbattery, sizeof (flatbattery)))
    {
      battery_flat (appleir);
      /*Fall through */
    }

  dump_packet (appleir, "unknown packet", data, len);
}

static void
appleir_urb (struct urb *urb)
{
  struct appleir *appleir = urb->context;
  int retval;

  switch (urb->status)
    {
    case 0:
      new_data (appleir, urb->transfer_buffer, urb->actual_length);
      break;
    case -ECONNRESET:
    case -ENOENT:
    case -ESHUTDOWN:
      /* this urb is terminated, clean up */
      dbg ("%s - urb shutting down with status: %d", __FUNCTION__,
           urb->status);
      return;
    default:
      dbg ("%s - nonzero urb status received: %d", __FUNCTION__, urb->status);
    }

  retval = usb_submit_urb (urb, GFP_ATOMIC);
  if (retval)
    err ("%s - usb_submit_urb failed with result %d", __FUNCTION__, retval);
}


static int
appleir_open (struct input_dev *dev)
{
  struct appleir *appleir = input_get_drvdata(dev);

  //appleir->urb->dev = appleir->usbdev;

  if (usb_submit_urb (appleir->urb, GFP_KERNEL))
    return -EIO;

  return 0;
}

static void
appleir_close (struct input_dev *dev)
{
  struct appleir *appleir = input_get_drvdata(dev);
  usb_kill_urb (appleir->urb);
  del_timer_sync (&appleir->key_up_timer);
}




static int
appleir_probe (struct usb_interface *intf, const struct usb_device_id *id)
{
  struct usb_device *dev = interface_to_usbdev (intf);
  struct usb_endpoint_descriptor *endpoint;
  struct appleir *appleir = NULL;
  struct input_dev *input_dev;
  int i;

  appleir = kzalloc (sizeof (struct appleir), GFP_KERNEL);
  if (!appleir)
    goto fail;

  memset (appleir, 0, sizeof (struct appleir));


  appleir->data =
    usb_alloc_coherent (dev, URB_SIZE, GFP_KERNEL, &appleir->dma_buf);
  if (!appleir->data)
    goto fail;

  appleir->urb = usb_alloc_urb (0, GFP_KERNEL);
  if (!appleir->urb)
    goto fail;

  appleir->usbdev = dev;

  input_dev = input_allocate_device ();
  if (!input_dev)
    goto fail;

  appleir->dev = input_dev;


  usb_make_path (dev, appleir->phys, sizeof (appleir->phys));
  strlcpy (appleir->phys, "/input0", sizeof (appleir->phys));

  input_dev->name = "Apple Mac mini infrared remote control driver";
  input_dev->phys = appleir->phys;
  usb_to_input_id (dev, &input_dev->id);
  input_dev->dev.parent = &intf->dev;
  input_set_drvdata(input_dev, appleir);

  input_dev->evbit[0] = BIT (EV_KEY) | BIT (EV_REP);
  input_dev->ledbit[0] = 0;

  for (i = 0; i < MAX_KEYS; i++)
    {
      set_bit (keymap[i], input_dev->keybit);
    }

  clear_bit (0, input_dev->keybit);

  input_dev->open = appleir_open;
  input_dev->close = appleir_close;

  endpoint = &intf->cur_altsetting->endpoint[0].desc;

  usb_fill_int_urb (appleir->urb, dev,
                    usb_rcvintpipe (dev, endpoint->bEndpointAddress),
                    appleir->data, 8,
                    appleir_urb, appleir, endpoint->bInterval);

  appleir->urb->transfer_dma = appleir->dma_buf;
  appleir->urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;

  usb_set_intfdata (intf, appleir);

  init_timer (&appleir->key_up_timer);

  appleir->key_up_timer.function = key_up_tick;
  appleir->key_up_timer.data = (unsigned long) appleir;

  appleir->timer_initted++;

  input_register_device (appleir->dev);

  return 0;

fail:

  if (appleir)
    {


      if (appleir->data)
        usb_free_coherent (dev, URB_SIZE, appleir->data, appleir->dma_buf);

      if (appleir->timer_initted)
        del_timer_sync (&appleir->key_up_timer);

      if (appleir->dev)
        input_free_device (appleir->dev);

      kfree (appleir);
    }

  return -ENOMEM;
}

static void
appleir_disconnect (struct usb_interface *intf)
{
  struct appleir *appleir = usb_get_intfdata (intf);

  usb_set_intfdata (intf, NULL);
  if (appleir)
    {
      input_unregister_device (appleir->dev);
      if (appleir->timer_initted)
        del_timer_sync (&appleir->key_up_timer);
      usb_kill_urb (appleir->urb);
      usb_free_urb (appleir->urb);
      usb_free_coherent (interface_to_usbdev (intf), URB_SIZE, appleir->data,
                       appleir->dma_buf);
      kfree (appleir);
    }
}

static struct usb_driver appleir_driver = {
  .name = "appleir",
  .probe = appleir_probe,
  .disconnect = appleir_disconnect,
  .id_table = appleir_ids,
};

static int __init
appleir_init (void)
{
  int retval;
  retval = usb_register (&appleir_driver);
  if (retval)
    goto out;
  printk (DRIVER_VERSION ":" DRIVER_DESC);
out:
  return retval;
}

static void __exit
appleir_exit (void)
{
  usb_deregister (&appleir_driver);
}

module_init (appleir_init);
module_exit (appleir_exit);
