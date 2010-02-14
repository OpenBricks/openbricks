/*
This is LIRC, Linux Infra-red Remote Control support for CIR Port of chips 
"NCT677x" / "W836x7" of Nuvoton Technology Co. (or Winbond in old time).

It was produced by Nuvoton PS00 group. Website : http://www.nuvoton.com

The whole thing is licensed under the GPL v2 or later.

    This package is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This package is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this package; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

Adapted for usage in GeeXboX by the GeeXboX team.
*/

#define _GNU_SOURCE
#include "lirc_wb677.h"
#include "wb83677hg_ir.h"


/* enter extended function mode */
static inline void cr_enter_ext(void)
{
	outb(0x87, CFG_idx);
	outb(0x87, CFG_idx);

	if (0xff == cr_read(0x20)) {
		CFG_idx = cr_cfg_idx2;
		CFG_dat = cr_cfg_dat2;
		outb(0x87, CFG_idx);
		outb(0x87, CFG_idx);
	}
}


/* exit extended function mode */
static inline void cr_exit_ext(void)
{
	outb(0xaa, CFG_idx);
}

/* select logical device */
static inline void cr_select_log_dev(int cr)
{
	outb(0x07, CFG_idx);
	outb(cr, CFG_dat);
}

static inline void cr_update(int dat, int cr)
{
	outb(cr, CFG_idx);
	outb(dat, CFG_dat);
}

static inline u8 cr_read(int cr)
{
	outb(cr, CFG_idx);
	return inb(CFG_dat);
}

static inline void cr_safe_update(u8 dat, int cr)
{
	cr_update(cr_read(cr) | dat, cr);
}

static inline void cr_safe_clear(u8 dat, int cr)
{
	cr_update(cr_read(cr) & dat, cr);
}


/* read/write cir registers */

static inline void cir_update(u8 dat, int cir)
{
	outb(dat, cir_address + (cir & 0xff));
}

static u8 cir_read(int cir)
{
	u8 val;

	val = inb(cir_address + (cir & 0xff));

	return val;
}

static inline void cir_wake_update(u8 dat, int cir)
{
	outb(dat, cir_wake_address + (cir & 0xff));
}

static u8 cir_wake_read(int cir)
{
	u8 val;

	val = inb(cir_wake_address + (cir & 0xff));

	return val;
}

static void cir_dump_reg(void)
{
	cr_enter_ext();
	printk("Dump CIR CR logical device:\n");
	cr_select_log_dev(CIR_LOG_DEV);
	printk("CR CIR ACTIVE :   0x%x\n", cr_read(0x30));
	printk("CR CIR BASE ADDR: 0x%x\n", (cr_read(0x61) << 8)|cr_read(0x60));
	printk("CR CIR IRQ NUM:   0x%x\n", cr_read(0x70));
	cr_exit_ext();

	printk("Dump CIR Registers\n");
	printk("CIR IRCON:   0x%x\n", cir_read(CIR_IRCON));
	printk("CIR IRSTS:   0x%x\n", cir_read(CIR_IRSTS));
	printk("CIR IREN:    0x%x\n", cir_read(CIR_IREN));
	printk("CIR RXFCONT: 0x%x\n", cir_read(CIR_RXFCONT));
	printk("CIR CP:      0x%x\n", cir_read(CIR_CP));
	printk("CIR CC:      0x%x\n", cir_read(CIR_CC));
	printk("CIR SLCH:    0x%x\n", cir_read(CIR_SLCH));
	printk("CIR SLCL:    0x%x\n", cir_read(CIR_SLCL));
	printk("CIR FIFOCON: 0x%x\n", cir_read(CIR_FIFOCON));
	printk("CIR IRFIFOSTS: 0x%x\n", cir_read(CIR_IRFIFOSTS));
	printk("CIR SRXFIFO: 0x%x\n", cir_read(CIR_SRXFIFO));
	printk("CIR TXFCONT: 0x%x\n", cir_read(CIR_TXFCONT));
	printk("CIR STXFIFO: 0x%x\n", cir_read(CIR_STXFIFO));
	printk("CIR FCCH:    0x%x\n", cir_read(CIR_FCCH));
	printk("CIR FCCL:    0x%x\n", cir_read(CIR_FCCL));
	printk("CIR IRFSM:   0x%x\n", cir_read(CIR_IRFSM));

}

static void cir_wake_dump_reg(void)
{
	u8 i = 0;

	cr_enter_ext();
	printk("Dump CIR WKAE CR logical device:\n");
	cr_select_log_dev(CIR_WAKE_LOG_DEV);
	printk("CR CIR WAKE ACTIVE :   0x%x \n", cr_read(0x30));
	printk("CR CIR WAKE BASE ADDR: 0x%x\n", (cr_read(0x61) << 8)|cr_read(0x60));
	printk("CR CIR WAKE IRQ NUM:   0x%x\n", cr_read(0x70));
	cr_exit_ext();

	printk("Dump CIR WAKE Registers\n");
	printk("CIR WAKE IRCON:   0x%x\n", cir_wake_read(CIR_WAKE_IRCON));
	printk("CIR IRSTS:        0x%x\n", cir_wake_read(CIR_WAKE_IRSTS));
	printk("CIR IREN:         0x%x\n", cir_wake_read(CIR_WAKE_IREN));
	printk("CIR WAKE FIFO CMP DEEP: 0x%x\n", cir_wake_read(CIR_WAKE_FIFO_CMP_DEEP));
	printk("CIR WAKE FIFO CMP TOL:  0x%x\n", cir_wake_read(CIR_WAKE_FIFO_CMP_TOL));
	printk("CIR WAKE FIFO COUNT:    0x%x\n", cir_wake_read(CIR_WAKE_FIFO_COUNT));
	printk("CIR WAKE SLCH:    0x%x\n", cir_wake_read(CIR_WAKE_SLCH));
	printk("CIR WAKE SLCL:    0x%x\n", cir_wake_read(CIR_WAKE_SLCL));
	printk("CIR WAKE FIFOCON: 0x%x\n", cir_wake_read(CIR_WAKE_FIFOCON));
	printk("CIR WAKE SRXFSTS: 0x%x\n", cir_wake_read(CIR_WAKE_SRXFSTS));
	printk("CIR WAKE SAMPLE RX FIFO: 0x%x\n", cir_wake_read(CIR_WAKE_SAMPLE_RX_FIFO));
	printk("CIR WAKE WR FIFO DATA:   0x%x\n", cir_wake_read(CIR_WAKE_WR_FIFO_DATA));
	printk("CIR WAKE RD FIFO ONLY:   0x%x\n", cir_wake_read(CIR_WAKE_RD_FIFO_ONLY));
	printk("CIR WAKE RD FIFO ONLY IDX: 0x%x\n", cir_wake_read(CIR_WAKE_RD_FIFO_ONLY_IDX));
	printk("CIR WAKE FIFO IGNORE:    0x%x\n", cir_wake_read(CIR_WAKE_FIFO_IGNORE));
	printk("CIR WAKE IRFSM:   0x%x\n", cir_wake_read(CIR_WAKE_IRFSM));

	printk("Dump CIR WAKE keys\n");
	printk("%s FIFO count len = %d\n", DRVNAME, cir_wake_read(CIR_WAKE_FIFO_COUNT));
	i = 0;
	for (; i < 67; i ++) {
		printk("%s FIFO = 0x%x\n", DRVNAME, cir_wake_read(CIR_WAKE_RD_FIFO_ONLY));
	}

}


/* 1. */
/* 667 HG Config Registers init */
static int w83667hg_cr_init(void)
{
	int val = 0;

	cr_enter_ext();

	/* Check 667 CHIP ID first */
	val = cr_read(CHIP_ID_HIGH_ADDR);
	if (val != CHIP_ID_HIGH) {
		printk("%s %s: chip id high: 0x%x expect:0x%x\n", DRVNAME, chip_id, val, CHIP_ID_HIGH);
	//	cr_exit_ext();
	//	return -ENODEV;
	}
    else{
		printk("%s %s: chip id high: 0x%x\n", DRVNAME, chip_id, val);
    }
	
	/* now check chip id, LSB */
	val = cr_read(CHIP_ID_LOW_ADDR);
	if (CHIP_ID_LOW == (val & CHIP_ID_LOW)) {
		printk("%s %s: chip id low: 0x%x expect:0x%x\n", DRVNAME, chip_id, val, CHIP_ID_LOW);
	//	cr_exit_ext();
	//	return -ENODEV;
	}
	else{
		printk("%s %s: chip id low: 0x%x\n", DRVNAME, chip_id, val);
	}
	
	/* for multi-function pin selection */
	val = cr_read(0x27);
	val = (val&0xbc) | 0x41;
	cr_update(val, 0x27); //For W83677, CIR TX,RX, CIRWB pin selection
	
		

	// FIXME
	/* set Logical Dev 1: LPT */
	/* not sure really need it or not, find it in 667 wake up windows driver */
	cr_select_log_dev(0x01);
	cr_update(DEACTIVE_LOG_DEV, 0x30);
	cr_update(0, 0x70);

	/* Then set Logical Dev 6: CIR */
	cr_select_log_dev(CIR_LOG_DEV);
	cr_update(ACTIVE_LOG_DEV, 0x30);

	cir_address = CIR_BASE;
	cr_update(((CIR_BASE & 0xff00) >> 8), 0x60);
	cr_update((CIR_BASE & 0xff), 0x61);
#ifdef IR_667_DEBUG
	printk("%s base io port address: 0x%x\n", DRVNAME, cir_address);
#endif

	cr_update(CIR_IRQ_NUM, 0x70);
#ifdef IR_667_DEBUG
	printk("%s irq number: %d\n", DRVNAME, CIR_IRQ_NUM);
#endif


	/* Then set Logical Dev A: ACPI */
	cr_select_log_dev(ACPI_LOG_DEV);

	cr_update(ACTIVE_LOG_DEV, 0x30);

	/* enable cir wake up via PSOUT# (pin 60) */
	cr_safe_update(ENABLE_CIR_WAKE, 0xe0);

	/* enable cir interrupt of mouse IRQ event */
	//cr_safe_update(ENABLE_CIR_INTR_OF_MOUSE_IRQ, 0xf6);

	/* enable pme interrupt of cir password event */
	//cr_safe_update(ENABLE_PME_INTR_OF_CIR_PASS, 0xf7);
	

	/* Then set Logical Dev E: CIR WAKE */
	cr_select_log_dev(CIR_WAKE_LOG_DEV);
	cr_update(ACTIVE_LOG_DEV, 0x30);

	cir_wake_address = CIR_WAKE_BASE;
	cr_update(((CIR_WAKE_BASE & 0xff00) >> 8), 0x60);
	cr_update((CIR_WAKE_BASE & 0xff), 0x61);
#ifdef IR_667_DEBUG
	printk("%s cir wake up base io port address: 0x%x\n", DRVNAME, cir_wake_address);
#endif

#ifdef ENABLE_CIR_WAKE_IRQ
	cr_update(CIR_WAKE_IRQ_NUM, 0x70);
#ifdef IR_667_DEBUG
	printk("%s cir wake up irq number: %d\n", DRVNAME, CIR_WAKE_IRQ_NUM);
#endif
#endif

	cr_exit_ext();

	return 0;
}

static void w83667hg_cr_uninit(void)
{
	cr_enter_ext();

	/* close CIR */
	/* Don't close CIR Wake. When wake-up at power-on, it needs the function. 20091224
	cr_select_log_dev(CIR_WAKE_LOG_DEV);
	cr_update(DEACTIVE_LOG_DEV, 0x30);     
	*/
	

	/* Close Logical Dev A: ACPI */
	//cr_select_log_dev(ACPI_LOG_DEV);

	/* enable cir wake up via PSOUT# (pin 60) */
	//cr_safe_clear(DISABLE_CIR_WAKE, 0xe0);

	/* enable cir interrupt of mouse IRQ event */
	//cr_safe_clear(DISABLE_CIR_INTR_OF_MOUSE_IRQ, 0xf6);

	/* enable pme interrupt of cir password event */
	//cr_safe_clear(DISABLE_PME_INTR_OF_CIR_PASS, 0xf7);


	/* close CIR wake up XXX */
	//cr_select_log_dev(CIR_WAKE_LOG_DEV);
	//cr_update(DEACTIVE_LOG_DEV, 0x30);

	cr_exit_ext();
}

static int w83667hg_input_open(struct input_dev *cur_input_dev)
{
	return 0;
}

static void w83667hg_input_close(struct input_dev *cur_input_dev)
{
}

/* 2. */
/* init linux input structure */
static struct input_dev *w83667hg_input_init(void)
{
	int i;
	struct input_dev *cur_input_dev = NULL;

	cur_input_dev = input_allocate_device();
	if (NULL == cur_input_dev) {
		printk("alloc input device error\n");
		return NULL;
	}

	cur_input_dev->name = INPUTNAME;
	cur_input_dev->phys = DRVNAME;
	memcpy(&cur_input_dev->id, &w83667hg_input_id, sizeof(struct input_id));

	cur_input_dev->open = w83667hg_input_open;
	cur_input_dev->close = w83667hg_input_close;

	//cur_input_dev->evbit[0] = BIT(EV_KEY) | BIT(EV_REL);
	//cur_input_dev->relbit[0] = BIT(REL_X) | BIT(REL_Y);
	cur_input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REL);
	cur_input_dev->relbit[0] = BIT_MASK(REL_X) | BIT_MASK(REL_Y);
	for (i=0; i<256; i++) {
		set_bit(usb_kbd_keycode[i], cur_input_dev->keybit);
	}
	clear_bit(0, cur_input_dev->keybit);
//	cur_input_dev->keybit[LONG(BTN_MOUSE)] = BIT(BTN_LEFT) | BIT(BTN_RIGHT);
	cur_input_dev->keybit[BIT_WORD(BTN_MOUSE)] = BIT_MASK(BTN_LEFT) | BIT_MASK(BTN_RIGHT);


	if (input_register_device(cur_input_dev)) {
		printk("register input device error\n");
		return NULL;
	}

	return cur_input_dev;
}

static void w83667hg_input_uninit(struct input_dev *cur_input_dev)
{
	input_get_device(cur_input_dev);
	input_unregister_device(cur_input_dev);
	input_free_device(cur_input_dev);
	cur_input_dev = NULL;
}


/* internal call for register lirc */
static int lirc_set_use_inc(void *data)
{
	struct irctl *ir = data;

	if (!ir) {
		printk("%s: set_use_inc called with no context\n", DRVNAME);
		return -EIO;
	}
#ifdef IR_667_DEBUG
	printk("%s : set use inc\n", DRVNAME);
#endif
	spin_lock(&ir->lock);
	ir->ctrl_fix_head = 1;
	spin_unlock(&ir->lock);


	MOD_INC_USE_COUNT;

	return 0;
}

static void lirc_set_use_dec(void *data)
{
	struct irctl *ir = data;

	if (!ir) {
		printk("%s: set_use_dec called with no context\n", DRVNAME);
		return;
	}
#ifdef IR_667_DEBUG
	printk("%s : set use dec\n", DRVNAME);
#endif

	MOD_DEC_USE_COUNT;
}


static int lirc_ioctl(struct inode *node, struct file *filep, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	void __user *uptr = (void __user *)arg;

	int mode = 0;
	struct irctl *ir = &w83667hg_irctl;
	struct ir_recv_t *ir_recv = &w83667hg_ir_recv;

#ifdef ALLOW_DEBUG_IOCTL
	printk("%s: IO Ctrl Code:%d\n", DRVNAME, cmd);
#endif

	switch(cmd)
	{
		/* lirc ioctl commands */
		case LIRC_GET_FEATURES:
			ret = put_user(w83667hg_lirc_plugin->features, (unsigned long*)arg);
			break;
		case LIRC_GET_SEND_MODE:
			if(!(w83667hg_lirc_plugin->features & LIRC_CAN_REC_MASK))
				return(-ENOSYS);
			
			ret = put_user(LIRC_REC2MODE
					  (w83667hg_lirc_plugin->features & LIRC_CAN_REC_MASK),
					  (unsigned long*)arg);
			if (ret) 
				return ret;
			break;
		case LIRC_SET_SEND_MODE:
			ret = get_user(mode, (unsigned long*)arg);
			if (ret) 
				return ret;
			if(mode != (LIRC_MODE_PULSE&LIRC_CAN_SEND_MASK)) 
				return -EINVAL;
			break;
		case LIRC_GET_LENGTH:
			ret = put_user((unsigned long)w83667hg_lirc_plugin->code_length, 
					  (unsigned long *)arg);
			break;

		/* ioctl commands for lirc_wb667 */
		case IR_DUMPCIRREG:
			cir_dump_reg();
			break;
		case IR_DUMPWAKEREG:
			cir_wake_dump_reg();
			break;
		case IR_IOLEARNWAKE:
			spin_lock(&ir_recv->lock);
			if (ir_recv->wake_state) {
				spin_unlock(&ir_recv->lock);
				ret = -EFAULT;
				break;
			}
			ir_recv->wake_state = ST_WAKE_START;
			spin_unlock(&ir_recv->lock);

			/* close cir first */
			cr_enter_ext();
			cr_select_log_dev(CIR_LOG_DEV);
			cr_update(DEACTIVE_LOG_DEV, 0x30);
			cr_exit_ext();
			//cir_update(0, CIR_IREN);
			cir_update(0xff, CIR_IRSTS);

			/* set cir wake */
			cir_wake_update(CIR_WAKE_IRCON_RXEN | CIR_WAKE_IRCON_RXINV | CIR_WAKE_IRCON_SAMPLE_PERIOD_SEL, CIR_WAKE_IRCON);
			cir_wake_update(0xff, CIR_WAKE_IRSTS);
			cir_wake_update(0xff, CIR_WAKE_FIFOCON);
			cir_wake_update(0, CIR_WAKE_FIFOCON);
#ifdef ALLOW_DEBUG_WAKE
			printk("%s FIFO count len = %d\n", DRVNAME, cir_wake_read(CIR_WAKE_FIFO_COUNT));
#endif
			cir_wake_update(CIR_WAKE_IREN_PE, CIR_WAKE_IREN);

			wait_event(ir_recv->queue, ir_recv->wake_state == ST_WAKE_FINISH);
			while ((cir_wake_read(CIR_WAKE_RD_FIFO_ONLY_IDX)) != 0) {
#ifdef ALLOW_DEBUG_WAKE
				printk("%s setting wake up key: 0x%x\n", DRVNAME, cir_wake_read(CIR_WAKE_RD_FIFO_ONLY));
#else
				cir_wake_read(CIR_WAKE_RD_FIFO_ONLY);
#endif
			}

			/* learn wake up complete */
			spin_lock(&ir_recv->lock);
			ir_recv->wake_state = ST_WAKE_NONE;
			spin_unlock(&ir_recv->lock);

#ifdef ALLOW_DEBUG_WAKE
			ret = 0;
			for (; ret < 67; ret ++) {
				printk("%s FIFO count = 0x%x\n", DRVNAME, cir_wake_read(CIR_WAKE_RD_FIFO_ONLY));
			}
			printk("%s FIFO count len = %d\n", DRVNAME, cir_wake_read(CIR_WAKE_FIFO_COUNT));
#endif

			/* cir wake interrupt stop, resume cir */
			cir_wake_update(CIR_WAKE_IRCON_MODE0 | CIR_WAKE_IRCON_RXEN | CIR_WAKE_IRCON_R | CIR_WAKE_IRCON_RXINV | CIR_WAKE_IRCON_SAMPLE_PERIOD_SEL, CIR_WAKE_IRCON);
			cir_wake_update(0xff, CIR_WAKE_IRSTS);
			cir_wake_update(0, CIR_WAKE_IREN);
			cr_enter_ext();
			cr_select_log_dev(CIR_LOG_DEV);
			cr_update(ACTIVE_LOG_DEV, 0x30);
			cr_exit_ext();
			ret = 0;

			break;
		case IR_IOUNSETWAKE:
			cir_wake_update(0, CIR_WAKE_IRCON);
			cir_wake_update(0, CIR_WAKE_IREN);
			cir_wake_update(0xFF, CIR_WAKE_IRSTS);
			cir_wake_update(0, CIR_WAKE_IRSTS);
			cr_enter_ext();
			cr_select_log_dev(CIR_WAKE_LOG_DEV);
			cr_update(DEACTIVE_LOG_DEV, 0x30);
			cr_exit_ext();
			//cir_wake_update(CIR_WAKE_IRCON_MODE0 | CIR_WAKE_IRCON_RXEN | CIR_WAKE_IRCON_R | CIR_WAKE_IRCON_RXINV | CIR_WAKE_IRCON_SAMPLE_PERIOD_SEL, CIR_WAKE_IRCON);
			break;
		case IR_IOSETWAKE:
			cr_enter_ext();
			cr_select_log_dev(CIR_WAKE_LOG_DEV);
			cr_update(ACTIVE_LOG_DEV, 0x30);
			cr_exit_ext();
			cir_wake_update(CIR_WAKE_IRCON_MODE0 | CIR_WAKE_IRCON_RXEN | CIR_WAKE_IRCON_R | CIR_WAKE_IRCON_RXINV | CIR_WAKE_IRCON_SAMPLE_PERIOD_SEL, CIR_WAKE_IRCON);
			cir_wake_update(0xFF, CIR_WAKE_IRSTS);
			cir_wake_update(0, CIR_WAKE_IRSTS);
			cir_wake_update(0, CIR_WAKE_IREN);
			break;
		case IR_IOCLEANDATA:
			w83667hg_clean_data(&w83667hg_ir_recv, &w83667hg_irctl);
			break;
		case IR_IOSTARTSTUDY:
			spin_lock(&ir->lock);
			ir->study_state = ST_STUDY_START;
			spin_unlock(&ir->lock);
			//cir_update(cir_read(CIR_IRCON) | CIR_IRCON_WIREN, CIR_IRCON);
			cir_update(CIR_IRCON_WIREN | CIR_IRCON_RXEN | CIR_IRCON_SAMPLE_PERIOD_SEL, CIR_IRCON);

/* for WRX RTR
			cir_update(CIR_FIFOCON_RX_TRIGGER_LEV_8, CIR_FIFOCON);
			//cir_update(CIR_IREN_RTR, CIR_IREN);
			cir_update(0, CIR_IREN);
*/
			break;
		case IR_IOSTOPSTUDY:
			cir_update(CIR_IRCON_TXEN | CIR_IRCON_RXEN | CIR_IRCON_RXINV | CIR_IRCON_SAMPLE_PERIOD_SEL, CIR_IRCON);
			spin_lock(&ir->lock);
			ir->study_state = ST_STUDY_NONE;
			ir->buf_count = 0;
			ir->cur_buf_num = 0;
			wake_up(&ir->queue);
			spin_unlock(&ir->lock);
			break;
		case IR_IOGETCARRIER:
			spin_lock(&ir->lock);
			if (!(ir->study_state > ST_STUDY_NONE)) {
				ret = -EFAULT;
				spin_unlock(&ir->lock);
				break;
			}
			//w83667hg_record_study_carrier(ir);
#ifdef ALLOW_DEBUG_IOCTL
			printk("%s: current get carrier: %d\n", DRVNAME, ir->carrier);
#endif
			if (copy_to_user(uptr, &ir->carrier, sizeof(unsigned int))) {
				ret = -EFAULT;
			}
			spin_unlock(&ir->lock);
			break;
		case IR_IOSETCARRIER:
			spin_lock(&ir->lock);
			/* you can set carrier at any time */
			/*
			if (~(ir->send_state)) {
				ret = -EFAULT;
				up(&ir->lock);
				break;
			}
			*/
			if (copy_from_user(&ir->carrier, uptr, sizeof(unsigned int))) {
				ret = -EFAULT;
				spin_unlock(&ir->lock);
				break;
			}
#ifdef ALLOW_DEBUG_IOCTL
			printk("%s: current set carrier: %x\n", DRVNAME, ir->carrier);
#endif
			w83667hg_set_carrier(&ir->carrier);
			spin_unlock(&ir->lock);
			break;
		case IR_IOSTUDYLEN:
			spin_lock(&ir->lock);
			if (ir->study_state == ST_STUDY_NONE) {
				spin_unlock(&ir->lock);
#ifdef ALLOW_DEBUG_IOCTL
				printk("%s: open STUDY first\n", DRVNAME);
#endif
				ret = -EFAULT;
				break;
			}
			ir->study_state = ST_STUDY_START;
			spin_unlock(&ir->lock);
			wait_event(ir->queue, ir->study_state == ST_STUDY_ALL_RECV);
			spin_lock(&ir->lock);

			/* in STUDY, copy data buf len to user space for read() */
			ir->cur_buf_num = 0;
			if (ir->buf_count == 0) {
				ret = -EFAULT;
			}
			else if (copy_to_user(uptr, &ir->buf_count, sizeof(unsigned int))) {
				ret = -EFAULT;
			}
			/* copy data to lirc plugin buffer, ready copy to user space */
			spin_unlock(&ir->lock);
			break;
		case IR_IOSTUDYBUF:
			spin_lock(&ir->lock);
			if (ir->study_state != ST_STUDY_ALL_RECV) {
				spin_unlock(&ir->lock);
				ret = -EFAULT;
				break;
			}
			if (copy_to_user(uptr, &ir->buf[ir->cur_buf_num], sizeof(unsigned char))) {
				spin_unlock(&ir->lock);
				ret = -EFAULT;
				break;
			}
			if (ir->cur_buf_num < ir->buf_count) {
				ir->cur_buf_num ++;
			}
			spin_unlock(&ir->lock);
			break;
		case IR_IORESTUDYBUF:
			spin_lock(&ir->lock);
				ir->cur_buf_num = 0;
			spin_unlock(&ir->lock);
			break;
		case IR_CHECKCHIP:
			if (copy_to_user(uptr, chip_id, sizeof(unsigned long long))) {
				ret = -EFAULT;
			}
			break;
		default:
			return (-ENOIOCTLCMD);
	}

	return ret;
}

static int w83667hg_set_carrier(unsigned int *carrier)
{
	u16 val;

	cir_update(1, CIR_CP);
	val = 3000000 / (*carrier) - 1;
	cir_update(val & 0xff, CIR_CC);

#ifdef ALLOW_DEBUG_STUDY
	printk("cp: 0x%x   cc: 0x%x\n", cir_read(CIR_CP), cir_read(CIR_CC));
#endif
	return 0;
}

static ssize_t lirc_write(struct file *file, const char *buf, size_t n, loff_t * ppos)
{
	size_t cur_count;
	struct ir_send_t *ir_send = &w83667hg_ir_send;
	unsigned int i;
	ssize_t ret;
	
	spin_lock(&ir_send->lock);

	if (n >= IRCTL_BUF_LIMIT) {
		ir_send->buf_count = cur_count = IRCTL_BUF_LIMIT;
		ret = IRCTL_BUF_LIMIT;
	} else {
		ir_send->buf_count = cur_count = n;
		ret = n;
	}

	/* the first copy from user and open interrupt for TX */
	if (copy_from_user(ir_send->buf, buf, ir_send->buf_count)) {
		ir_send->buf_count = 0;
		spin_unlock(&ir_send->lock);
		return (-EFAULT);
	}

	ir_send->cur_buf_num = 0;
	
	/* for safety, close RX while TX */
	cir_update(CIR_IREN_TFU | CIR_IREN_TTR, CIR_IREN);

	ir_send->send_state = ST_SEND_REPLY;

	cir_update(CIR_FIFOCON_TX_TRIGGER_LEV_8 | CIR_FIFOCON_RXFIFOCLR, CIR_FIFOCON);
	/* turn on TTR interrupt, it's ugly */
	i = 0;
	for (; i < 9; i ++) {
		cir_update(0x01, CIR_STXFIFO);
	}
	spin_unlock(&ir_send->lock);

	wait_event(ir_send->queue, ir_send->send_state == ST_SEND_REQUEST);

	spin_lock(&ir_send->lock);
	ir_send->send_state = ST_SEND_NONE;
	spin_unlock(&ir_send->lock);

	cir_update(CIR_IREN_RDR | CIR_IREN_PE, CIR_IREN);

	return ret;
}

static void w83667hg_clean_data(struct ir_recv_t *ir_recv, struct irctl *ir)
{
	spin_lock(&ir_recv->lock);
	ir_recv->buf_count = 0;
	//ir_recv->wake_state = ST_WAKE_NONE;
	wake_up(&ir_recv->queue);
	spin_unlock(&ir_recv->lock);

	spin_lock(&ir->lock);
	ir->lircdata = 0;
	/*
	for (i = 0; i < IRCTL_BUF_LIMIT; i ++) {
		ir->buf = 0;
	}
	*/
	ir->pressed_keycode = 0;
	ir->pressed_shiftmask = 0;
	ir->buf_count = 0;
	ir->cur_buf_num = 0;
	/*
	ir->study_state = ST_STUDY_NONE;
	ir->send_state = ST_SEND_NONE;
	*/
	wake_up(&ir->queue);
	spin_unlock(&ir->lock);

	cir_update(cir_read(CIR_FIFOCON) | 0x88, CIR_FIFOCON);
	cir_wake_update(cir_wake_read(CIR_WAKE_FIFOCON) | 0x8, CIR_WAKE_FIFOCON);

}

/* 3. */
static void w83667hg_ir_recv_init(struct ir_recv_t *ir_recv)
{
	ir_recv->buf_count = 0;
	spin_lock_init(&ir_recv->lock);
	ir_recv->wake_state = ST_WAKE_NONE;
	init_waitqueue_head(&ir_recv->queue);
	ir_recv->recv_state = ST_RECV_WAIT_7F;
}

static void w83667hg_ir_send_init(struct ir_send_t *ir_send)
{
	ir_send->buf_count = 0;
	spin_lock_init(&ir_send->lock);
	ir_send->send_state = ST_SEND_NONE;
	init_waitqueue_head(&ir_send->queue);
}


/* lirc_fops 
 *
 * 1) it's LIRC's fops, so NOT allow add owner in it
 * 2) define lirc functions at here will replace the lirc original fops functions, so lirc_write() in lirc_dev will not work
 * 3) lirc has its own ioctl, lirc_ioctl() in current file not register in lirc_fops, so it only work for lirc_wb667
 *    lirc_ioctl() in current file register in w83667hg_irctl_init() as lirc_plugin's ioctl while initial driver, and it does not effect on lirc_dev
 */
static struct file_operations lirc_fops = {
	write:  lirc_write,
	ioctl: lirc_ioctl,
};


/* init lirc buffer, register, irctl */
static int w83667hg_irctl_init(struct irctl *ir)
{
	int err = 0, minor = -1;

	w83667hg_lirc_buffer = kmalloc(sizeof(struct lirc_buffer), GFP_KERNEL);
	if (!w83667hg_lirc_buffer) {
		err = -ENOMEM;
		goto out_lirc_buffer;
	}
	memset(w83667hg_lirc_buffer, 0, sizeof(struct lirc_buffer));

	if (lirc_buffer_init(w83667hg_lirc_buffer, sizeof(lirc_t), LIRCBUF_SIZE)) {
		err = -ENOMEM;
		goto out_lirc_buffer;
	}

	w83667hg_lirc_plugin = kmalloc(sizeof(struct lirc_driver), GFP_KERNEL);
	if (!w83667hg_lirc_plugin) {
		err = -ENOMEM;
		goto out_lirc_plugin;
	}
	memset(w83667hg_lirc_plugin, 0, sizeof(struct lirc_driver));

	ir->lirc_plugin = w83667hg_lirc_plugin;

	strcpy(w83667hg_lirc_plugin->name, DRVNAME);
	w83667hg_lirc_plugin->minor = -1;
	w83667hg_lirc_plugin->features = LIRC_CAN_SEND_PULSE |
	                       LIRC_CAN_SET_TRANSMITTER_MASK |
                         LIRC_CAN_REC_MODE2;
	                      // LIRC_CAN_SET_SEND_DUTY_CYCLE |
	                      // LIRC_CAN_SET_SEND_CARRIER;
	w83667hg_lirc_plugin->data = &w83667hg_irctl;
	w83667hg_lirc_plugin->rbuf = w83667hg_lirc_buffer;
	w83667hg_lirc_plugin->set_use_inc = &lirc_set_use_inc;
	w83667hg_lirc_plugin->set_use_dec = &lirc_set_use_dec;
	w83667hg_lirc_plugin->code_length = sizeof(lirc_t) * 8;
//	w83667hg_lirc_plugin->ioctl = lirc_ioctl;
  w83667hg_lirc_plugin->fops  = &lirc_fops;
  w83667hg_lirc_plugin->dev   = NULL;
	w83667hg_lirc_plugin->owner = THIS_MODULE;

	//if ((minor = lirc_register_plugin(w83667hg_lirc_plugin)) < 0) {
	if ((minor = lirc_register_driver(w83667hg_lirc_plugin)) < 0) {
		err = -ENOMEM;
		goto out_lirc_plugin_register;
	}
#ifdef IR_667_DEBUG
	printk("%s register lirc minor: %d\n", DRVNAME, minor);
#endif

	w83667hg_lirc_plugin->minor = minor;
	ir->lirc_plugin = w83667hg_lirc_plugin;

	/* init irctl */
	ir->pressed_keycode = 0;
	ir->pressed_shiftmask = 0;
	ir->buf_count = 0;
	ir->ctrl_fix_head = 1;
	spin_lock_init(&ir->lock);
	ir->study_state = ST_STUDY_NONE;
	init_waitqueue_head(&ir->queue);
	
	goto out;

 out_lirc_plugin_register:
	kfree(w83667hg_lirc_plugin);
 out_lirc_plugin:
	lirc_buffer_free(w83667hg_lirc_buffer);
 out_lirc_buffer:
	kfree(w83667hg_lirc_buffer);
 out:
	return err;
}

static void w83667hg_irctl_uninit(struct irctl *ir)
{
	//lirc_unregister_plugin(w83667hg_lirc_plugin->minor);
    lirc_unregister_driver(w83667hg_lirc_plugin->minor);
	kfree(w83667hg_lirc_plugin);
	ir->lirc_plugin = NULL;
	lirc_buffer_free(w83667hg_lirc_buffer);
	kfree(w83667hg_lirc_buffer);
	ir->lirc_buffer = NULL;
}

#ifdef ALLOW_DEBUG_PRINT_PULSE
void debug_print_pulse(void)
{
	u8 count, i;

	count = cir_read(CIR_RXFCONT);
	for (i = 0; i < count; i ++) {
		printk("%s current cir pluse: 0x%x\n", DRVNAME, cir_read(CIR_SRXFIFO));
	}
}

void debug_print_wake_pulse(void)
{
	u8 count, i;

	count = cir_wake_read(CIR_WAKE_FIFO_COUNT);
	for (i = 0; i < count; i ++) {
		printk("%s current cir wake pluse: 0x%x\n", DRVNAME, cir_wake_read(CIR_WAKE_SRXFSTS));
	}
}
#endif

static void w83667hg_study_recv(struct ir_recv_t *ir_recv, struct irctl *ir)
{
	unsigned int i = 0;
	unsigned int packet_on_dur = 0;

	spin_lock(&ir_recv->lock);
	ir_recv->buf[ir_recv->buf_count] = cir_read(CIR_SRXFIFO);
	if (ir_recv->buf[ir_recv->buf_count] == 0x80) {
		/* close interrupt now */
		//cir_wake_update(0, CIR_WAKE_IREN);

		spin_lock(&ir->lock);
		
		/* 1. get carrier */
		ir->carrier = cir_read(CIR_FCCL);
		ir->carrier |= cir_read(CIR_FCCH) << 8;
	
		if (ir->carrier == 0) {
			printk("%s: get carrier error!\n", DRVNAME);
		} 

		i = 0;
		for (; i < ir_recv->buf_count; i ++) {
			if(ir_recv->buf[i] & BIT_PULSE_MASK) {
				packet_on_dur += ir_recv->buf[i] & BIT_LEN;
			}
		}
		packet_on_dur *= MCE_TIME_UNIT;

#ifdef ALLOW_DEBUG_STUDY
	printk("%s: carrier count: 0x%x\n", DRVNAME, ir->carrier);
	printk("%s: packet on duration: %u\n", DRVNAME, packet_on_dur);
#endif
		ir->carrier *= 1000000;
		ir->carrier /= packet_on_dur;
#ifdef ALLOW_DEBUG_STUDY
	printk("%s: final carrier frequency: %u\n", DRVNAME, ir->carrier);
#endif
		if ((ir->carrier > MAX_CARRIER) || (ir->carrier < MIN_CARRIER)) {
		/* carrier is too large or too small */
#ifdef ALLOW_DEBUG_STUDY
			printk("%s: current received carrier is too large or too small\n", DRVNAME);
#endif
			ir_recv->buf_count = 0;
			spin_unlock(&ir_recv->lock);

			ir->buf_count = 0;
			ir->study_state = ST_STUDY_ALL_RECV;
			wake_up(&ir->queue);
			spin_unlock(&ir->lock);
			return;
		}
		
		/* 2. get pulse */
		i = 0;
		/* 1) find sync head */
		while (!((ir_recv->buf[i] & BIT_PULSE_MASK) & 0x80)) {
			i ++;
		}
		/* 2) find 0x7f */
		// FIXME, the silent part of infrared signal may change by protocol or sample period. but current value fits MCE RC-6
		while ((ir_recv->buf[i] != 0x7f) ||
		       (ir_recv->buf[i + 1] != 0x7f) ||
		       (ir_recv->buf[i + 2] != 0x7f))	{
			i ++;
		}
		/* 3) find next head */
		while (!((ir_recv->buf[i] & BIT_PULSE_MASK) & 0x80)) {
			i ++;
		}
		//ir->buf_count = --i;
		/* now buf_count direct to next pulse sync head */
		ir->buf_count = i;
		/* 4) copy pluse from recv to ir */ 
		i = 0;
		for (; i <= ir->buf_count; i ++) {
			ir->buf[i] = ir_recv->buf[i];
		}


		ir_recv->buf_count = 0;
		spin_unlock(&ir_recv->lock);

		ir->study_state = ST_STUDY_ALL_RECV;
		wake_up(&ir->queue);
		spin_unlock(&ir->lock);
	} else {
		ir_recv->buf_count ++;
		spin_unlock(&ir_recv->lock);
	}
}


static void w83667hg_recv(struct ir_recv_t *ir_recv, struct irctl *ir)
{
	u8 buf;
	unsigned int i = 0, rlc = 0;

	spin_lock(&ir_recv->lock);
	
	ir_recv->buf[ir_recv->buf_count] = cir_read(CIR_SRXFIFO);
	if (ir_recv->buf[ir_recv->buf_count] == 0x7f) {
		if (ir_recv->recv_state & ST_RECV_WAIT_7F)	{
			ir_recv->recv_state = ST_RECV_WAIT_HEAD;
			ir_recv->buf_count ++;
			{ // decode begin
				spin_lock(&ir->lock);

				ir->buf_count = ir_recv->buf_count;
				for (; i < ir->buf_count; i ++) {
					ir->buf[i] = ir_recv->buf[i];
				}
				ir->cur_buf_num = 0;

				//ir_recv->buf_count ++;
				spin_unlock(&ir_recv->lock);

				i = 0;
				while (i < ir->buf_count) {
					rlc += ir->buf[i] & BIT_LEN;
					i ++;
				}
#ifdef ALLOW_DEBUG_DECODE
	printk("\n%s cur rlc len: %d\n", DRVNAME, rlc);
#endif
				
				if (rlc >= CONTROLLER_BUF_LEN_MIN) { 
				// lirc controller
					w83667hg_submit_controller(ir);
				} else if ((rlc >= KEYBOARD_BUF_LEN_MIN) && 
				           (rlc < KEYBOARD_BUF_LEN_MAX)) {
					w83667hg_submit_key(ir);
					input_sync(ir->input_dev);
				} else if ((rlc >= MOUSE_BUF_LEN_MIN) && 
	    			       (rlc < KEYBOARD_BUF_LEN_MIN)) {
					w83667hg_submit_mouse(ir);
					input_sync(ir->input_dev);
				}
	
				ir->buf_count = 0;
				spin_unlock(&ir->lock);
			} // decode end
		} else {
			ir_recv->buf_count ++;
			spin_unlock(&ir_recv->lock);
		}
	} else { // normal recv
		if (ir_recv->recv_state & ST_RECV_WAIT_HEAD) {
			if (ir_recv->buf[ir_recv->buf_count] & BIT_PULSE_MASK) {
				ir_recv->recv_state = ST_RECV_WAIT_7F;
				buf = ir_recv->buf[ir_recv->buf_count];
				ir_recv->buf_count = 0;
				ir_recv->buf[0] = buf;
			}
		}
		ir_recv->buf_count ++;
		spin_unlock(&ir_recv->lock);
	}
}


static void w83667hg_set_key(u8 *set_frame, u8 val, u8 *keycode, u8 *shiftmask)
{
#ifdef ALLOW_DEBUG_DECODE
	printk("%s set key: %d\n", DRVNAME, val);
#endif

	if (*set_frame <= KEY_SUBMIT_KEYCODE_FRAME_NUM + 1) {
		*keycode <<= 1;
		*keycode |= val;
#ifdef ALLOW_DEBUG_DECODE
	printk("%s set key keycode:0x%x\n", DRVNAME, *keycode);
	printk("set frame: %d\n", *set_frame);
#endif
	} else {
		*shiftmask <<= 1;
		*shiftmask |= val;
#ifdef ALLOW_DEBUG_DECODE
	printk("%s set key mask:0x%x\n", DRVNAME, *shiftmask);
	printk("set frame: %d\n", *set_frame);
#endif
	}
	(*set_frame) ++;
}


static int w83667hg_jump_frame(struct irctl *ir, u8 frame_num, unsigned int *buf_num, u8 *out_bit, u8 *decode_status)
{
	unsigned int rlc = 0, cur_buf_num = *buf_num;
	u8 bit = *out_bit, buf;
	u8 cur_decode_status = *decode_status;
	u8 cur_frame_num;

	cur_frame_num = 1;
	for (; cur_frame_num <= frame_num; cur_buf_num ++) {
		buf = ir->buf[cur_buf_num];
		if (cur_buf_num > ir->buf_count) {
#ifdef ALLOW_DEBUG_DECODE
			printk("%s: jump frame error\n", DRVNAME);
			printk("current buf num: %d\n", cur_buf_num);
			printk("current jumped frame: %d\n", cur_frame_num);
			printk("current frame length: %d\n", rlc);
			printk("total cir signal:\n");
			for (cur_buf_num = 0; cur_buf_num < ir->buf_count; cur_buf_num ++) {
				printk("0x%x ", ir->buf[cur_buf_num]);
				if (cur_buf_num % 6 == 5) {
					printk("\n");
				}
			}
#endif
			return (-1);
		}
		if (bit == (buf & BIT_PULSE_MASK)) {
			rlc += buf & BIT_LEN;
		} else {

		// decode
			switch (cur_decode_status) {
			case ST_DECODE_NEW:
				if ((rlc > ONE_PULSE_LEN_LOW) &&
				    (rlc < ONE_PULSE_LEN_HIGH)) {
					if (bit & BIT_PULSE_MASK) {
						cur_decode_status = ST_DECODE_ONE;
					} else {
						cur_decode_status = ST_DECODE_ZERO;
					}
				} 
				else if ((rlc > TWO_PULSE_LEN_LOW) &&
				         (rlc < TWO_PULSE_LEN_HIGH)) {
					/* error occur, just ignore */
					cur_decode_status = ST_DECODE_NEW;
					cur_frame_num ++;
				}
				break;
			case ST_DECODE_ZERO:
				if ((rlc > ONE_PULSE_LEN_LOW) &&
				    (rlc < ONE_PULSE_LEN_HIGH)) {
					if (bit & BIT_PULSE_MASK) {
						/* "01" => 0 */
						cur_decode_status = ST_DECODE_NEW;
						cur_frame_num ++;
					}	else {
						/* error */
						cur_decode_status = ST_DECODE_NEW;
						cur_frame_num ++;
					}
				} 
				else if ((rlc > TWO_PULSE_LEN_LOW) &&
				         (rlc < TWO_PULSE_LEN_HIGH)) {
					if (bit & BIT_PULSE_MASK) {
						/* "01" => 0 */
						cur_decode_status = ST_DECODE_ONE;
						cur_frame_num ++;
					}	else {
						/* error */
						cur_decode_status = ST_DECODE_ZERO;
						cur_frame_num ++;
					}
				}
				break;
			case ST_DECODE_ONE:
				if ((rlc > ONE_PULSE_LEN_LOW) &&
				    (rlc < ONE_PULSE_LEN_HIGH)) {
					if (bit & BIT_PULSE_MASK) {
						/* "11" => error */
						cur_decode_status = ST_DECODE_NEW;
						cur_frame_num ++;
					}	else {
						/* "10" => 1 */
						cur_decode_status = ST_DECODE_NEW;
						cur_frame_num ++;
					}
				} 
				else if (rlc > TWO_PULSE_LEN_LOW) {
					if (bit & BIT_PULSE_MASK) {
						/* "11" => error */
						cur_decode_status = ST_DECODE_ONE;
						cur_frame_num ++;
					}	else {
						/* "10" => 1 */
						cur_decode_status = ST_DECODE_ZERO;
						cur_frame_num ++;
					}
				}
				break;
		}// switch

			bit = buf & BIT_PULSE_MASK;
			rlc = buf & BIT_LEN;
		}
	} // for decode

/*
	bit = ir->buf[cur_buf_num] & BIT_MASK;
	if (cur_decode_status & ST_DECODE_NEW) {
		if (bit) {
			*decode_status = ST_DECODE_ONE;
		} else {
			*decode_status = ST_DECODE_ZERO;
		}
	} else {
		*decode_status = ST_DECODE_NEW;
	}
	*out_bit = bit;
*/

	/* Fix problem 1 */
	if ((ir->buf[cur_buf_num] & BIT_LEN) > TWO_PULSE_LEN_LOW) {
#ifdef ALLOW_DEBUG_DECODE
		printk("%s: fix problem 1 in jump_frame()\n", DRVNAME);
#endif
#ifdef ALLOW_DEBUG_DECODE
		if (cur_frame_num > frame_num + 1) {
			printk("%s error: jump_frame() get a too long frame\n", DRVNAME);
		}
#endif
		cur_buf_num --;
	}

	/* copy from jump_iden */
	if (cur_decode_status & ST_DECODE_NEW) {
#ifdef ALLOW_DEBUG_DECODE
		printk("hey man, it's NEW in jump_frame\n");
printk("cur buf: 0x%x\n", ir->buf[cur_buf_num]);
printk("cur state: 0x%x\n", cur_decode_status);
#endif
		*buf_num = -- cur_buf_num;
		//cur_buf_num --;
		*out_bit = ir->buf[cur_buf_num] & BIT_PULSE_MASK;
		if (*out_bit) {
			*decode_status = ST_DECODE_ONE;
		} else {
			*decode_status = ST_DECODE_ZERO;
		}
	} else {
#ifdef ALLOW_DEBUG_DECODE
printk("cur buf: 0x%x\n", ir->buf[cur_buf_num]);
printk("cur state: 0x%x\n", cur_decode_status);
#endif
		*buf_num = cur_buf_num;
		*out_bit = ir->buf[cur_buf_num] & BIT_PULSE_MASK;
		*decode_status = ST_DECODE_NEW;
/*
		if (*out_bit) {
			*decode_status = ST_DECODE_ONE;
		} else {
			*decode_status = ST_DECODE_ZERO;
		}
*/
	}

	return 0;
}

static int w83667hg_jump_head(struct irctl *ir, unsigned int *buf_num)
{
	unsigned int i, rlc = 0, max_buf_count = ir->buf_count;
	u8 bit = BIT_PULSE_MASK, buf;

	for (i = 0; i < max_buf_count; i ++) {
		buf = ir->buf[i];
		if (bit == (buf & BIT_PULSE_MASK)) {
			rlc += buf & BIT_LEN;
		} else {
			if ((rlc > HEAD_SYNC_LEN_LOW) &&
			    (rlc < HEAD_SYNC_LEN_HIGH) &&
					(bit & BIT_PULSE_MASK)) {
				break;
			} 
			bit = buf & BIT_PULSE_MASK;
			rlc = buf & BIT_LEN;
		}
	} // for decode

	if (i >= max_buf_count) {
//#ifdef ALLOW_DEBUG_DECODE
#if 0
		printk("%s jump pulse error\n", DRVNAME);
		printk("current buf num: %d\n", *buf_num);
		printk("current jumped pulse: %d\n", i);
		printk("current pulse length: %d\n", rlc);
		printk("total cir signal:\n");
		for (i = 0; i < max_buf_count; i ++) {
			printk("0x%x ", ir->buf[i]);
			if (i % 6 == 5) {
				printk("\n");
			}
		}
#endif
		return (-1);
	}

	*buf_num = i;

	return 0;
}


static int w83667hg_check_identification(u8 val, u8 identification, u8 *cur_iden)
{
	*cur_iden <<= 1;
	*cur_iden |= val ;
#ifdef ALLOW_DEBUG_DECODE
	printk("%s current check iden recv: %d\n", DRVNAME, val);
	printk("%s current iden value: 0x%x\n", DRVNAME, *cur_iden);
#endif

	if (identification == (*cur_iden & IDENTIFICATION_CHECK_BIT)) {
#ifdef ALLOW_DEBUG_DECODE
	printk("%s find identification\n\n", DRVNAME);
#endif
		return 0;
	} else {
		return (-1);
	}
}

static int w83667hg_jump_identification(struct irctl *ir, u8 identification, unsigned int *buf_num, u8 *out_bit, u8 *decode_status)
{
	unsigned int rlc = 0, i = 1;
	u8 bit = *out_bit, buf;
	u8 cur_iden = 0;
	u8 cur_decode_status = *decode_status;
	
	bit = BIT_PULSE_MASK;
	cur_decode_status = ST_DECODE_NEW;

	for (; i < ir->buf_count; i ++) {
		buf = ir->buf[i];
#ifdef ALLOW_DEBUG_DECODE
printk("buf: 0x%x\n", buf);
#endif
		if (bit == (buf & BIT_PULSE_MASK)) {
			rlc += buf & BIT_LEN;
		} else {
		// decode
			switch (cur_decode_status) {
			case ST_DECODE_NEW:
				if ((rlc > ONE_PULSE_LEN_LOW) &&
				    (rlc < ONE_PULSE_LEN_HIGH)) {
					if (bit & BIT_PULSE_MASK) {
						cur_decode_status = ST_DECODE_ONE;
					} else {
						cur_decode_status = ST_DECODE_ZERO;
					}
				} 
				else if ((rlc > TWO_PULSE_LEN_LOW) &&
				         (rlc < TWO_PULSE_LEN_HIGH)) {
					/* error occur, just ignore */
					cur_decode_status = ST_DECODE_NEW;
					if (!w83667hg_check_identification(0, identification, &cur_iden)) {
						goto find_iden;
					}
				}
				break;
			case ST_DECODE_ZERO:
				if ((rlc > ONE_PULSE_LEN_LOW) &&
				    (rlc < ONE_PULSE_LEN_HIGH)) {
					if (bit & BIT_PULSE_MASK) {
						/* "01" => 0 */
						cur_decode_status = ST_DECODE_NEW;
						if (!w83667hg_check_identification(0, identification, &cur_iden)) {
							goto find_iden;
						}
					}	else {
						/* error */
						cur_decode_status = ST_DECODE_NEW;
						if (!w83667hg_check_identification(0, identification, &cur_iden)) {
							goto find_iden;
						}
					}
				} 
				else if ((rlc > TWO_PULSE_LEN_LOW) &&
				         (rlc < TWO_PULSE_LEN_HIGH)) {
					if (bit & BIT_PULSE_MASK) {
						/* "01" => 0 */
						cur_decode_status = ST_DECODE_ONE;
						if (!w83667hg_check_identification(0, identification, &cur_iden)) {
							goto find_iden;
						}
					}	else {
						/* error */
						cur_decode_status = ST_DECODE_ZERO;
						if (!w83667hg_check_identification(0, identification, &cur_iden)) {
							goto find_iden;
						}
					}
				}
				break;
			case ST_DECODE_ONE:
				if ((rlc > ONE_PULSE_LEN_LOW) &&
				    (rlc < ONE_PULSE_LEN_HIGH)) {
					if (bit & BIT_PULSE_MASK) {
						/* "11" => error */
						cur_decode_status = ST_DECODE_NEW;
						if (!w83667hg_check_identification(0, identification, &cur_iden)) {
							goto find_iden;
						}
					}	else {
						/* "10" => 1 */
						cur_decode_status = ST_DECODE_NEW;
						if (!w83667hg_check_identification(1, identification, &cur_iden)) {
							goto find_iden;
						}
					}
				} 
				else if (rlc > TWO_PULSE_LEN_LOW) {
					if (bit & BIT_PULSE_MASK) {
						/* "11" => error */
						cur_decode_status = ST_DECODE_ONE;
						if (!w83667hg_check_identification(0, identification, &cur_iden)) {
							goto find_iden;
						}
					}	else {
						/* "10" => 1 */
						cur_decode_status = ST_DECODE_ZERO;
						if (!w83667hg_check_identification(1, identification, &cur_iden)) {
							goto find_iden;
						}
					}
				}
				break;
		}// switch
			bit = buf & BIT_PULSE_MASK;
			rlc = buf & BIT_LEN;
		}
	}

	if (i >= ir->buf_count) {
#ifdef ALLOW_DEBUG_DECODE
		printk("%s jump iden error\n", DRVNAME);
		printk("current buf num: %d\n", i);
		printk("current pulse length: %d\n", rlc);
		printk("total cir signal:\n");
		for (i = 0; i < ir->buf_count; i ++) {
			printk("0x%x ", ir->buf[i]);
			if (i % 6 == 5) {
				printk("\n");
			}
		}
#endif
		return (-1);
	}

 find_iden:
#ifdef ALLOW_DEBUG_DECODE
		printk("current buf num after iden: %d\n", i);
		printk("current pulse length: %d\n", rlc);
#endif
	/* try fix problem 1 */
	if ((rlc & BIT_LEN) >= TWO_PULSE_LEN_LOW) {
#ifdef ALLOW_DEBUG_DECODE
		printk("%s fix problem 1 in jump iden\n", DRVNAME);
		printk("cur pulse len: %d\n", ir->buf[i] & BIT_LEN);
#endif
		*buf_num = i + 1;
		*out_bit = ir->buf[i] & BIT_PULSE_MASK;
		if (*out_bit) {
			*decode_status = ST_DECODE_ZERO;
		} else {
			*decode_status = ST_DECODE_ONE;
		}

		return 0;
	}

	/* now find identification successful! */
	{
		*buf_num = i;
		i --;
		*out_bit = ir->buf[i] & BIT_PULSE_MASK;
		if (bit) {
			*decode_status = ST_DECODE_ONE;
		} else {
			*decode_status = ST_DECODE_ZERO;
		}
	}
#ifdef ALLOW_DEBUG_DECODE
		printk("total cir signal:\n");
		for (i = 0; i < ir->buf_count; i ++) {
			printk("0x%x ", ir->buf[i]);
			if (i % 7 == 6) {
				printk("\n");
			}
		}
		printk("\n");
#endif

	return 0;
}

static void w83667hg_submit_key(struct irctl *ir)
{
	unsigned int rlc = 0, buf_num = 0, i;
	/* current usb keyboard key code setting, usb_kbd_keycode[keycode] */
	unsigned int input_code;
	u8 bit, buf;
	/* keycode and shiftmask parts in mce keyboard protocol*/
	u8 keycode = 0, shiftmask = 0;
	u8 decode_status;
	u8 frame_num, set_frame;

	/* 1) jump head */
	if (w83667hg_jump_head(ir, &buf_num)) {
#ifdef ALLOW_DEBUG_DECODE
		printk("%s: decode key jump head error\n", DRVNAME);
#endif
		return;
	}
#ifdef ALLOW_DEBUG_DECODE
	printk("%s buf_num after jump head: %d\n", DRVNAME, buf_num);
#endif

	/* 2) jump identification */
	if (w83667hg_jump_identification(ir, KEY_IDENTIFICATION, &buf_num, &bit, &decode_status)) {
#ifdef ALLOW_DEBUG_DECODE
		printk("%s: decode key jump identification error\n", DRVNAME);
#endif
		return;
	}

#ifdef ALLOW_DEBUG_DECODE
	printk("%s jump iden success\n\n", DRVNAME);
	printk("%s buf_num after jump iden: %d\n", DRVNAME, buf_num);
	printk("decode status: 0x%x, bit: 0x%x\n", decode_status, bit);
#endif

	/* 3) jump "unknown", "C" and "CodeSet" parts in mce keyboard signal */
	if (w83667hg_jump_frame(ir, KEY_KEYCODE_FRAME_AFTER_I_START, &buf_num, &bit, &decode_status)) {
#ifdef ALLOW_DEBUG_DECODE
		printk("%s: decode key jump pulse error\n", DRVNAME);
#endif
		return;
	}

#ifdef ALLOW_DEBUG_DECODE
	printk("%s buf_num after jump pulse: %d\n", DRVNAME, buf_num);
	printk("decode status: 0x%x, bit: 0x%x\n", decode_status, bit);
	frame_num = buf_num;
	for (; frame_num < ir->buf_count; frame_num ++) {
		printk("0x%x\t", ir->buf[frame_num]);
		if (frame_num % 7 == 6) {
			printk("\n");
		}
	}
	printk("\n");
#endif
#ifdef ALLOW_DEBUG_DECODE
	printk("%s: decode status before check key: %x\n", DRVNAME, decode_status);
#endif


	/* 4) deocde "KeyCode" and "ShiftMask" parts in mce key signal */
	frame_num = 1, set_frame = 1;
	for (; frame_num < KEY_FRAME_AFTER_JUMP + 2; buf_num ++) {
		if (bit != (ir->buf[buf_num] & BIT_PULSE_MASK)) {
			// decode
			if (rlc > PULSE_SILENT_LEN_LOW) {
#ifdef ALLOW_DEBUG_DECODE
				printk("%s error: cir signal end before received all key pulses\n", DRVNAME);
				printk("bit: %d ", bit);
				printk("rlc: %d\n", rlc);
				printk("current pulse number: %d\n", frame_num);
#endif
				if (decode_status == ST_DECODE_ONE) {
				/* must set the last one */
					w83667hg_set_key(&set_frame, 1, &keycode, &shiftmask);
				}
				break;
			}
		switch (decode_status) {
			case ST_DECODE_NEW:
				if ((rlc > ONE_PULSE_LEN_LOW) &&
				    (rlc < ONE_PULSE_LEN_HIGH)) {
					if (bit & BIT_PULSE_MASK) {
						decode_status = ST_DECODE_ONE;
					} else {
						decode_status = ST_DECODE_ZERO;
					}
				} 
				else if ((rlc > TWO_PULSE_LEN_LOW) &&
				         (rlc < TWO_PULSE_LEN_HIGH)) {
					/* error occur, just ignore */
					decode_status = ST_DECODE_NEW;
					w83667hg_set_key(&set_frame, 0, &keycode, &shiftmask);
					frame_num += 1;
				}
#ifdef ALLOW_DEBUG_DECODE
				printk("%s cur state: %x\n", DRVNAME, decode_status);
#endif
				break;
			case ST_DECODE_ZERO:
				if ((rlc > ONE_PULSE_LEN_LOW) &&
				    (rlc < ONE_PULSE_LEN_HIGH)) {
					if (bit & BIT_PULSE_MASK) {
						/* "01" => 0 */
						decode_status = ST_DECODE_NEW;
						w83667hg_set_key(&set_frame, 0, &keycode, &shiftmask);
						frame_num += 1;
					}	else {
						/* error */
						decode_status = ST_DECODE_NEW;
						w83667hg_set_key(&set_frame, 0, &keycode, &shiftmask);
					}
				} 
				else if ((rlc > TWO_PULSE_LEN_LOW) &&
				         (rlc < TWO_PULSE_LEN_HIGH)) {
					if (bit & BIT_PULSE_MASK) {
						/* "01" => 0 */
						decode_status = ST_DECODE_ONE;
						w83667hg_set_key(&set_frame, 0, &keycode, &shiftmask);
						frame_num += 1;
					}	else {
						/* error */
						decode_status = ST_DECODE_ZERO;
						w83667hg_set_key(&set_frame, 0, &keycode, &shiftmask);
					}
				}
#ifdef ALLOW_DEBUG_DECODE
				printk("%s cur state: %x\n", DRVNAME, decode_status);
#endif
				break;
			case ST_DECODE_ONE:
				if ((rlc > ONE_PULSE_LEN_LOW) &&
				    (rlc < ONE_PULSE_LEN_HIGH)) {
					if (bit & BIT_PULSE_MASK) {
						/* "11" => error */
						decode_status = ST_DECODE_NEW;
						w83667hg_set_key(&set_frame, 0, &keycode, &shiftmask);
					}	else {
						/* "10" => 1 */
						decode_status = ST_DECODE_NEW;
						w83667hg_set_key(&set_frame, 1, &keycode, &shiftmask);
						frame_num += 1;
					}
				} 
				else if (rlc > TWO_PULSE_LEN_LOW) {
					if (bit & BIT_PULSE_MASK) {
						/* "11" => error */
						decode_status = ST_DECODE_ONE;
						w83667hg_set_key(&set_frame, 0, &keycode, &shiftmask);
					}	else {
						/* "10" => 1 */
						decode_status = ST_DECODE_ZERO;
						w83667hg_set_key(&set_frame, 1, &keycode, &shiftmask);
						frame_num += 1;
					}
				}
#ifdef ALLOW_DEBUG_DECODE
				printk("%s cur state %x\n", DRVNAME, decode_status);
#endif
				break;
		}// switch
			bit = ir->buf[buf_num] & BIT_PULSE_MASK;
			rlc = ir->buf[buf_num] & BIT_LEN;
#ifdef ALLOW_DEBUG_DECODE
	printk("%s: decode keyboard buf: 0x%x\n", DRVNAME, ir->buf[buf_num]);
#endif

		} else {
			// continue last pulse
			rlc += ir->buf[buf_num] & BIT_LEN;
		}
		
	} // for

#ifdef ALLOW_DEBUG_DECODE
	printk("%s: after decode keyboard:\n", DRVNAME);
	printk("keycode: 0x%x  ", keycode);
	printk("shiftmask: 0x%x\n\n", shiftmask);
#endif

	/* 5) submit keycode to input */

	if (keycode != ir->pressed_keycode) {
		if (ir->pressed_keycode) {
			input_code = (unsigned int)usb_kbd_keycode[ir->pressed_keycode];
			input_report_key(ir->input_dev, input_code, 0);
		}
		ir->pressed_keycode = keycode;
		if (keycode) {
			input_code = (unsigned int)usb_kbd_keycode[keycode];
			input_report_key(ir->input_dev, input_code, 1);
		}
	}

	/* 6) submit shiftmask to input */

	if (shiftmask != ir->pressed_shiftmask) {
		for (i = 0; i < 7; i ++) {
			buf = 1<<i;
			if ((buf & ir->pressed_shiftmask) && (!(buf & shiftmask))) {
				input_code = (unsigned int)usb_kbd_keycode[0xE0 + i];
				input_report_key(ir->input_dev, input_code, 0);
			}
			else if (!(buf & ir->pressed_shiftmask) && (buf & shiftmask)) {
				input_code = (unsigned int)usb_kbd_keycode[0xE0 + i];
				input_report_key(ir->input_dev, input_code, 1);
			}
		} // for
		ir->pressed_shiftmask = shiftmask;
	}

}

static void	w83667hg_set_mouse(u8 *set_frame, u8 val, u8 *deltay, u8 *deltax, u8 *clickrl)
{
#ifdef ALLOW_DEBUG_DECODE
	printk("%s set mouse val:%d\n", DRVNAME, val);
#endif
	if (*set_frame <= MOUSE_SUBMIT_DELTAY_FRAME_NUM + 1) {
		*deltay <<= 1;
		*deltay |= val;
#ifdef ALLOW_DEBUG_DECODE
	printk("%s set mouse delta y:0x%x\n", DRVNAME, *deltay);
#endif
	} else if (*set_frame <= MOUSE_SUBMIT_DELTAX_FRAME_NUM) {
		*deltax <<= 1;
		*deltax |= val;
#ifdef ALLOW_DEBUG_DECODE
	printk("%s set mouse delta x:0x%x\n", DRVNAME, *deltax);
#endif
	} else if (*set_frame <= MOUSE_SUBMIT_L_FRAME_NUM) {
		/* move right key, then set left key */
		*clickrl <<= 1;
		*clickrl |= val;
#ifdef ALLOW_DEBUG_DECODE
	printk("%s set mouse l:0x%x\n", DRVNAME, *clickrl);
	printk("set pulse: %d\n", *set_frame);
#endif
	}
	(*set_frame) ++;
}

static void w83667hg_submit_mouse(struct irctl *ir)
{
	unsigned int rlc = 0, buf_num = 0;
	u8 bit;
	/* deltax, deltay and clickrl parts in mce mouse protocol*/
	u8 deltax = 0, deltay = 0, clickrl = 0;
	signed char x, y;
	u8 decode_status;
	u8 frame_num, set_frame;

	/* 1) jump head */
	if (w83667hg_jump_head(ir, &buf_num)) {
#ifdef ALLOW_DEBUG_DECODE
		printk("%s: decode mouse jump head error\n", DRVNAME);
#endif
		return;
	}

	/* 2) jump identification */
	if (w83667hg_jump_identification(ir, MOUSE_IDENTIFICATION, &buf_num, &bit, &decode_status)) {
#ifdef ALLOW_DEBUG_DECODE
		printk("%s: decode mouse jump identification error\n", DRVNAME);
#endif
		return;
	}
#ifdef ALLOW_DEBUG_DECODE
	printk("%s: after iden mouse:\n", DRVNAME);
	printk("decode status: 0x%x, bit: 0x%x\n", decode_status, bit);
	frame_num = buf_num;
	for (; frame_num < ir->buf_count; frame_num ++) {
		printk("0x%x ", ir->buf[frame_num]);
		if (frame_num % 7 == 6) {
			printk("\n");
		}
	}
	printk("\n");
#endif

	/* 3) jump "unknown" part in mce mouse signal */
	if (w83667hg_jump_frame(ir, MOUSE_DELTAY_PULSE_AFTER_I_START, &buf_num, &bit, &decode_status)) {
#ifdef ALLOW_DEBUG_DECODE
		printk("%s: decode mouse jump pulse error\n", DRVNAME);
#endif
		return;
	}
#ifdef ALLOW_DEBUG_DECODE
	printk("%s: after jump pulse buf num: %d\n", DRVNAME, buf_num);
	printk("decode status: 0x%x, bit: 0x%x\n", decode_status, bit);
	frame_num = buf_num;
	for (; frame_num < ir->buf_count; frame_num ++) {
		printk("0x%x ", ir->buf[frame_num]);
		if (frame_num % 7 == 6) {
			printk("\n");
		}
	}
	printk("\n");
#endif

	/* 4) deocde "deltay", "deltax", "r" and "l" parts in mce mouse signal */
	frame_num = 1, set_frame = 1;
	for (; frame_num < MOUSE_CHECKSUM_FRAME_AFTER_JUMP + 1; buf_num ++) {
#ifdef ALLOW_DEBUG_DECODE
	printk("%s: cur buf: 0x%x, pulse num: %d\n", DRVNAME, ir->buf[buf_num], frame_num);
	printk("decode status: 0x%x\n", decode_status);
#endif
		if (bit != (ir->buf[buf_num] & BIT_PULSE_MASK)) {
			// decode
			if (rlc > PULSE_SILENT_LEN_LOW) {
#ifdef ALLOW_DEBUG_DECODE
				printk("%s error: cir signal end before received all mouse pulses\n", DRVNAME);
				printk("bit: %d ", bit);
				printk("rlc: %d\n", rlc);
#endif
				break;
			}
		switch (decode_status) {
			case ST_DECODE_NEW:
				if ((rlc > ONE_PULSE_LEN_LOW) &&
				    (rlc < ONE_PULSE_LEN_HIGH)) {
					if (bit & BIT_PULSE_MASK) {
						decode_status = ST_DECODE_ONE;
					} else {
						decode_status = ST_DECODE_ZERO;
					}
				} else
				if ((rlc > TWO_PULSE_LEN_LOW) &&
				    (rlc < TWO_PULSE_LEN_HIGH)) {
					/* error occur, just ignore */
					decode_status = ST_DECODE_NEW;
					w83667hg_set_mouse(&set_frame, 0, &deltay, &deltax, &clickrl);
					frame_num += 1;
				}
				break;
			case ST_DECODE_ZERO:
				if ((rlc > ONE_PULSE_LEN_LOW) &&
				    (rlc < ONE_PULSE_LEN_HIGH)) {
					if (bit & BIT_PULSE_MASK) {
						/* "01" => 0 */
						decode_status = ST_DECODE_NEW;
						w83667hg_set_mouse(&set_frame, 0, &deltay, &deltax, &clickrl);
						frame_num += 1;
					}	else {
						/* error */
						decode_status = ST_DECODE_NEW;
						w83667hg_set_mouse(&set_frame, 0, &deltay, &deltax, &clickrl);
					}
				} 
				else if ((rlc > TWO_PULSE_LEN_LOW) &&
				         (rlc < TWO_PULSE_LEN_HIGH)) {
					if (bit & BIT_PULSE_MASK) {
						/* "01" => 0 */
						decode_status = ST_DECODE_ONE;
						w83667hg_set_mouse(&set_frame, 0, &deltay, &deltax, &clickrl);
						frame_num += 1;
					}	else {
						/* error */
						decode_status = ST_DECODE_ZERO;
						w83667hg_set_mouse(&set_frame, 0, &deltay, &deltax, &clickrl);
					}
				}
				break;
			case ST_DECODE_ONE:
				if ((rlc > ONE_PULSE_LEN_LOW) &&
				    (rlc < ONE_PULSE_LEN_HIGH)) {
					if (bit & BIT_PULSE_MASK) {
						/* "11" => error */
						decode_status = ST_DECODE_NEW;
						w83667hg_set_mouse(&set_frame, 0, &deltay, &deltax, &clickrl);
					}	else {
						/* "10" => 1 */
						decode_status = ST_DECODE_NEW;
						w83667hg_set_mouse(&set_frame, 1, &deltay, &deltax, &clickrl);
						frame_num += 1;
					}
				} 
/*
				else if ((rlc > TWO_PULSE_LEN_LOW) &&
				         (rlc < TWO_PULSE_LEN_HIGH)) 
*/
				else if (rlc > TWO_PULSE_LEN_LOW) {
					if (bit & BIT_PULSE_MASK) {
						/* "11" => error */
						decode_status = ST_DECODE_ONE;
						w83667hg_set_mouse(&set_frame, 0, &deltay, &deltax, &clickrl);
						frame_num += 1;
					}	else {
						/* "10" => 1 */
						decode_status = ST_DECODE_ZERO;
						w83667hg_set_mouse(&set_frame, 1, &deltay, &deltax, &clickrl);
						frame_num += 1;
					}
				}
				break;
		}// switch
			bit = ir->buf[buf_num] & BIT_PULSE_MASK;
			rlc = ir->buf[buf_num] & BIT_LEN;
		} else {
			// continue last pulse
			rlc += ir->buf[buf_num] & BIT_LEN;
		}
		
	} // for

#ifdef ALLOW_DEBUG_DECODE
	printk("%s: after decode mouse:\n", DRVNAME);
	frame_num = buf_num;
	for (; frame_num < ir->buf_count; frame_num ++) {
		printk("0x%x ", ir->buf[frame_num]);
		if (frame_num % 7 == 6) {
			printk("\n");
		}
	}
	printk("\n");
#endif
	
	/* fix deltax lost problem */
	if (deltax) {
		deltax <<= 1;
		deltax |= 1;
	}

#ifdef ALLOW_DEBUG_DECODE
	printk("%s: after decode mouse:\n", DRVNAME);
	printk("deltay: 0x%x ", deltay);
	printk("deltax: 0x%x  ", deltax);
	printk("click rl: 0x%x  \n\n", clickrl);
#endif
	if (deltay & 0x40) {
		y = -((~deltay & 0x7f) + 1);
	} else {
		y = deltay;
	}
	if (deltax & 0x40) {
		x = -((~deltax & 0x7f) + 1);
	} else {
		x = deltax;
	}

	/* 5) send to input */

#ifdef ALLOW_DEBUG_DECODE
	printk("%s: after decode mouse and rebuild:\n", DRVNAME);
	printk("y: %d ", y);
	printk("x: %d ", x);
	printk("click rl: 0x%x  \n\n", clickrl);
#endif

	input_report_rel(ir->input_dev, REL_X, x);
	input_report_rel(ir->input_dev, REL_Y, y);

	input_report_key(ir->input_dev, BTN_LEFT, clickrl & 1);
	input_report_key(ir->input_dev, BTN_RIGHT, (clickrl >> 1) & 1);
}

static void w83667hg_send_packet_to_lirc_1(struct irctl *ir, lirc_t *val)
{
#ifdef ALLOW_DEBUG_DECODE
	printk("%s: send data to lirc : 0x%x\n", DRVNAME, (*val));
#endif
//	lirc_buffer_write_1(ir->lirc_plugin->rbuf, (char *)val);
	lirc_buffer_write(ir->lirc_plugin->rbuf, (char *)val);
	wake_up(&ir->lirc_plugin->rbuf->wait_poll);
}

static void w83667hg_submit_controller(struct irctl *ir)
{
	unsigned int buf_num;
	u8 bit;

	/* fix controller head not sync problem */
	/* lirc doesn't response DK MCE controller signal until press several times, not find same problem on M$ MCE controller keyboard */
	if (ir->ctrl_fix_head) {
		ir->lircdata = 50000;
		w83667hg_send_packet_to_lirc_1(ir, &ir->lircdata);
		ir->ctrl_fix_head = 0;
	}

	buf_num = 0;
	bit = BIT_PULSE_MASK;
	ir->lircdata = 0;
	for (; buf_num < ir->buf_count; buf_num ++) {
		if (bit == (ir->buf[buf_num] & BIT_PULSE_MASK)) {
			ir->lircdata += (ir->buf[buf_num] & BIT_LEN) * MCE_TIME_UNIT;
		} else {
			if (bit) {
				ir->lircdata |= PULSE_BIT;
			}
			w83667hg_send_packet_to_lirc_1(ir, &ir->lircdata);
			bit = ir->buf[buf_num] & BIT_PULSE_MASK;
			ir->lircdata = (ir->buf[buf_num] & BIT_LEN) * MCE_TIME_UNIT;
		}
	}
	/* for final silent pulse */
	ir->lircdata = 50000;
	w83667hg_send_packet_to_lirc_1(ir, &ir->lircdata);

#ifdef ALLOW_DEBUG_DECODE
		printk("\n");
#endif
}

static irqreturn_t w83667hg_interrupt_handler(int irq, void *dev)
{
	u8 tmp;
	struct irctl *ir = (struct irctl *)dev;
	struct ir_send_t *ir_send = &w83667hg_ir_send;


	//Because interrupt is shared, check IREN first. 
	tmp = cir_read(CIR_IREN);
	if (!tmp) {
		return IRQ_RETVAL(IRQ_NONE);
	}


	tmp = cir_read(CIR_IRSTS);
	cir_update(0xff, CIR_IRSTS);
	if (!tmp) {
		return IRQ_NONE;
	} 
	if (tmp & CIR_IRSTS_RDR) {
#ifdef ALLOW_DEBUG_INTERRUPT
		printk("get CIR_IRSTS_RDR\n");
#endif
#ifdef ALLOW_DEBUG_PRINT_PULSE
		debug_print_pulse();
#else
		spin_lock(&ir_send->lock);
		if (ir_send->send_state == ST_SEND_NONE) {
			spin_unlock(&ir_send->lock);
			if (ir->study_state & ST_STUDY_START) {
				w83667hg_study_recv(&w83667hg_ir_recv, &w83667hg_irctl);
			} else {
				w83667hg_recv(&w83667hg_ir_recv, &w83667hg_irctl);
			}
		} else {
			spin_unlock(&ir_send->lock);
		}
#endif
	}
	if (tmp & CIR_IRSTS_RTR) {
#ifdef ALLOW_DEBUG_INTERRUPT
		printk("get CIR_IRSTS_RTR\n");
#endif
	}
	if (tmp & CIR_IRSTS_PE) {
#ifdef ALLOW_DEBUG_INTERRUPT
		printk("get CIR_IRSTS_PE\n");
#endif
#ifdef ALLOW_DEBUG_PRINT_PULSE
		printk("\n now get interrupt PE\n\n");
		debug_print_pulse();
#else
		if (ir->study_state == ST_STUDY_NONE) {
			w83667hg_clean_data(&w83667hg_ir_recv, &w83667hg_irctl);
		}
#endif
	}
	if (tmp & CIR_IRSTS_RFO) {
#ifdef ALLOW_DEBUG_INTERRUPT
		printk("get CIR_IRSTS_RFO\n");
#endif
	}
	if (tmp & CIR_IRSTS_TE) {
#ifdef ALLOW_DEBUG_INTERRUPT
		printk("get CIR_IRSTS_TE\n");
#endif
	}
	if (tmp & CIR_IRSTS_TTR) {
#ifdef ALLOW_DEBUG_INTERRUPT
		printk("get CIR_IRSTS_TTR\n");
#endif
		
		spin_lock(&ir_send->lock);
		if (ir_send->cur_buf_num < ir_send->buf_count) {
			cir_update(ir_send->buf[ir_send->cur_buf_num++], CIR_STXFIFO);
		} else {
			cir_update(cir_read(CIR_IREN) & (~CIR_IREN_TTR), CIR_IREN);
		}
		spin_unlock(&ir_send->lock);
		
	}
	if (tmp & CIR_IRSTS_TFU) {
#ifdef ALLOW_DEBUG_INTERRUPT
		printk("get CIR_IRSTS_TFU\n");
#endif
		spin_lock(&ir_send->lock);
		if (ST_SEND_REPLY == ir_send->send_state) {
			ir_send->send_state = ST_SEND_REQUEST;
			wake_up(&ir_send->queue);
		}
		spin_unlock(&ir_send->lock);
	}
	if (tmp & CIR_IRSTS_GH) {
#ifdef ALLOW_DEBUG_INTERRUPT
		printk("get CIR_IRSTS_GH\n");
#endif
	}

	return IRQ_HANDLED;
}

static irqreturn_t w83667hg_wake_interrupt_handler(int irq, void *dev)
{
	u8 tmp;
	struct ir_recv_t *ir_recv = (struct ir_recv_t *)dev;


	//Because interrupt is shared, check IREN first. 
	tmp = cir_wake_read(CIR_WAKE_IREN);
	if (!tmp) {
		return IRQ_RETVAL(IRQ_NONE);
	}

	tmp = cir_wake_read(CIR_WAKE_IRSTS);
	if (!tmp) {
		return IRQ_NONE;
	} 
	cir_wake_update(0xff, CIR_WAKE_IRSTS);

	if (tmp & CIR_WAKE_IRSTS_RDR) {
#ifdef ALLOW_DEBUG_INTERRUPT
		printk("get CIR_WAKE_IRSTS_RDR\n");
#endif
#ifdef ALLOW_DEBUG_PRINT_PULSE
		debug_print_wake_pulse();
#endif
	}
	if (tmp & CIR_WAKE_IRSTS_RTR) {
#ifdef ALLOW_DEBUG_INTERRUPT
		printk("get CIR_WAKE_IRSTS_RTR\n");
#endif
	}
	if ((tmp & CIR_WAKE_IRSTS_PE) &&
	    (ST_WAKE_START == ir_recv->wake_state)) {
#ifdef ALLOW_DEBUG_INTERRUPT
		printk("get CIR_WAKE_IRSTS_PE\n");
#endif
#ifdef ALLOW_DEBUG_PRINT_PULSE
		printk("\n now get interrupt PE\n\n");
		debug_print_wake_pulse();
#else
		while ((cir_wake_read(CIR_WAKE_RD_FIFO_ONLY_IDX)) != 0) {
#ifdef ALLOW_DEBUG_WAKE
			printk("%s setting wake up key: 0x%x\n", DRVNAME, cir_wake_read(CIR_WAKE_RD_FIFO_ONLY));
#else
			cir_wake_read(CIR_WAKE_RD_FIFO_ONLY);
#endif
		}

		cir_wake_update(0, CIR_WAKE_IREN);
		spin_lock(&ir_recv->lock);
		ir_recv->wake_state = ST_WAKE_FINISH;
		wake_up(&ir_recv->queue);
		spin_unlock(&ir_recv->lock);
#endif
	}
	if (tmp & CIR_WAKE_IRSTS_RFO) {
#ifdef ALLOW_DEBUG_INTERRUPT
		printk("get CIR_WAKE_IRSTS_RFO\n");
#endif
	}
	if (tmp & CIR_WAKE_IRSTS_GH) {
#ifdef ALLOW_DEBUG_INTERRUPT
		printk("get CIR_WAKE_IRSTS_GH\n");
#endif
	}

	return IRQ_HANDLED;
}

/* 4. */
/* init 667 cir dev, req_region, req_irq */
static int w83667hg_cir_probe(void)
{
	int err = 0;

	if(!request_region(cir_address, CIR_IOREG_LENGTH, DRVNAME)) {
		err = -EBUSY;
#ifdef IR_667_DEBUG
		printk("%s request 667 cir io port error! \n", DRVNAME);
#endif
		goto exit;
	}

	cir_update(CIR_IRCON_RXINV | CIR_IRCON_SAMPLE_PERIOD_SEL, CIR_IRCON);
	cir_update(0xFF, CIR_IRSTS);
	cir_update(CIR_RX_LIMIT_COUNT >> 8, CIR_SLCH);
	cir_update(CIR_RX_LIMIT_COUNT & 0xff, CIR_SLCL);
	cir_update(CIR_FIFOCON_TXFIFOCLR | CIR_FIFOCON_TX_TRIGGER_LEV_24 | CIR_FIFOCON_RXFIFOCLR, CIR_FIFOCON);
	cir_update(CIR_IRCON_RECV | CIR_IRCON_RXINV | CIR_IRCON_SAMPLE_PERIOD_SEL, CIR_IRCON);
	cir_update(CIR_IRCON_TXEN | CIR_IRCON_RXEN | CIR_IRCON_RXINV | CIR_IRCON_SAMPLE_PERIOD_SEL, CIR_IRCON);

	if(0 != request_irq(CIR_IRQ_NUM, w83667hg_interrupt_handler, IRQF_SHARED,
		DRVNAME, &w83667hg_irctl)) {
		err = -EINTR;
#ifdef IR_667_DEBUG
		printk("%s : request cir irq error\n", DRVNAME);
#endif
		goto rel_irq_exit;
	}
	/* open interrupt */
	cir_update(CIR_IREN_RDR | CIR_IREN_PE, CIR_IREN);

	if(!request_region(cir_wake_address, CIR_IOREG_LENGTH, DRVNAME)) {
		err = -EBUSY;
#ifdef IR_667_DEBUG
		printk("%s request 667 cir wake io port error! \n", DRVNAME);
#endif
		goto rel_wake_exit;
	}

	cir_wake_update(0xff, CIR_WAKE_IRSTS);
	/* Modify it as more safe values: 	CIR_WAKE_FIFO_CMP_DEEP reg:0x41, 
										CIR_WAKE_FIFO_CMP_TOL reg: 0x05.   20091224
	*/
	cir_wake_update(0x41, CIR_WAKE_FIFO_CMP_DEEP); /* 0x41 = 65 */
	cir_wake_update(0x05, CIR_WAKE_FIFO_CMP_TOL);
	cir_wake_update(CIR_RX_LIMIT_COUNT >> 8, CIR_WAKE_SLCH);
	cir_wake_update(CIR_RX_LIMIT_COUNT & 0xff, CIR_WAKE_SLCL);
	cir_wake_update(0xff, CIR_WAKE_FIFOCON);
	cir_wake_update(0, CIR_WAKE_FIFOCON);
	cir_wake_update(CIR_WAKE_IRCON_MODE0 | CIR_WAKE_IRCON_RXEN | CIR_WAKE_IRCON_R | CIR_WAKE_IRCON_RXINV | CIR_WAKE_IRCON_SAMPLE_PERIOD_SEL, CIR_WAKE_IRCON);

	/* cir wake has irq_handler, open interrupt after received ioctl IR_IOLEARNWAKE */
	if(0 != request_irq(CIR_WAKE_IRQ_NUM, w83667hg_wake_interrupt_handler, IRQF_SHARED,
		DRVNAME, &w83667hg_ir_recv)) {
		err = -EINTR;
#ifdef IR_667_DEBUG
		printk("%s : request cir wake irq error\n", DRVNAME);
#endif
		goto rel_wake_irq_exit;
	}

#ifdef IR_667_DEBUG
	printk("%s : init cir success\n", DRVNAME);
#endif
	goto exit;

	/* error exit */
 rel_wake_irq_exit:
	release_region(cir_wake_address, CIR_IOREG_LENGTH);
 rel_wake_exit:
 	cir_update(0, CIR_IREN);
	free_irq(CIR_IRQ_NUM, &w83667hg_irctl);
 rel_irq_exit:
	release_region(cir_address, CIR_IOREG_LENGTH);

	/* final exit */
 exit:
	return err;
}

static void w83667hg_cir_remove(void)
{
	/* Don't clean CIR_WAKE_IRCON. When wake-up at power-on, it needs the function. 20091224
	cir_wake_update(0, CIR_WAKE_IRCON);
	*/
	free_irq(CIR_WAKE_IRQ_NUM, &w83667hg_ir_recv);
	release_region(cir_wake_address, CIR_IOREG_LENGTH);
	cir_update(0, CIR_IRCON);
	free_irq(CIR_IRQ_NUM, &w83667hg_irctl);
	release_region(cir_address, CIR_IOREG_LENGTH);
}

static int lirc_wb667_init(void)
{
	int err = 0;

	/* 1. init cr */
	if (w83667hg_cr_init()) {
		printk("%s: Unable to init device.\n", DRVNAME);
		err = -ENODEV;
		goto out;
	}

	/* 2. init input */
	w83667hg_input_dev = w83667hg_input_init();
	if (!w83667hg_input_dev) {
		printk("%s: Unable to register input device.\n", DRVNAME);
		err = -ENODEV;
		goto out_input;
	}
	w83667hg_irctl.input_dev = w83667hg_input_dev;

	/* 3. init lirc buffer, register, irctl */
	w83667hg_ir_recv_init(&w83667hg_ir_recv);
	w83667hg_ir_send_init(&w83667hg_ir_send);
	err = w83667hg_irctl_init(&w83667hg_irctl);
	if (err) {
		printk("%s: Unable to register lirc.\n", DRVNAME);
		goto out_irctl;
	}

	/* 4. init 667 cir dev, req_region, req_irq */
	err = w83667hg_cir_probe();
	if (err) {
		printk("%s: Unable to probe cir device.\n", DRVNAME);
		goto out_cir_probe;
	}

	goto out;

	/* error exit */
 out_cir_probe:
	w83667hg_irctl_uninit(&w83667hg_irctl);
	w83667hg_irctl.input_dev = NULL;
 out_irctl:
	w83667hg_input_uninit(w83667hg_input_dev);
 out_input:
	w83667hg_cr_uninit();

	/* final exit */
 out:
	return err;
}

void lirc_wb667_uninit(void)
{
	w83667hg_cir_remove();
	w83667hg_irctl_uninit(&w83667hg_irctl);
	w83667hg_irctl.input_dev = NULL;
	w83667hg_input_uninit(w83667hg_input_dev);
	w83667hg_cr_uninit();

}

static struct platform_driver lirc_wb667_driver = {
	.probe		= lirc_wb667_probe,
	.remove	= 	__devexit_p(lirc_wb667_remove),
	.suspend	= lirc_wb667_suspend,
	.resume		= lirc_wb667_resume,
	.driver		= {
		.name	= PLATNAME,
		.owner	= THIS_MODULE,
	},
};


static int __devinit lirc_wb667_probe(struct platform_device *dev)
{
	return 0;
}

static int __devexit lirc_wb667_remove(struct platform_device *dev)
{
	return 0;
}

static int lirc_wb667_suspend(struct platform_device *dev, pm_message_t state)
{
	struct irctl *ir = &w83667hg_irctl;
	struct ir_recv_t *ir_recv = &w83667hg_ir_recv;
	struct ir_send_t *ir_send = &w83667hg_ir_send;

#ifdef IR_667_DEBUG
	printk("%s receive suspend\n", DRVNAME);
#endif

	/* 1. */
	spin_lock(&ir->lock);
	ir->study_state = ST_STUDY_NONE;
	spin_unlock(&ir->lock);

	spin_lock(&ir_recv->lock);
	ir_recv->wake_state = ST_WAKE_NONE;
	spin_unlock(&ir_recv->lock);

	spin_lock(&ir_send->lock);
	ir_send->send_state = ST_SEND_NONE;
	spin_unlock(&ir_send->lock);
	
	/* 2. */
	cir_update(0, CIR_IREN);
	cir_wake_update(0, CIR_WAKE_IREN);
	
	/* 3. */
	cr_enter_ext();
	cr_select_log_dev(CIR_LOG_DEV);
	cr_update(DEACTIVE_LOG_DEV, 0x30);
	
	/* Don't close CIR Wake. When wake-up at power-on, it needs the function.  20091224
	cr_select_log_dev(CIR_WAKE_LOG_DEV);
	cr_update(DEACTIVE_LOG_DEV, 0x30);
	*/
	cr_exit_ext();

	/* 4. */
	lirc_wb667_uninit();

	return 0;
}

static int lirc_wb667_resume(struct platform_device *dev)
{
	int ret;

#ifdef IR_667_DEBUG
	printk("%s receive suspend\n", DRVNAME);
#endif

	/* resume */
	ret = lirc_wb667_init();

	return ret;
}


#ifdef CONFIG_PNP

/* CIR and CIR WAKE */

static struct pnp_driver lirc_wb667_pnp_driver = {
	.name		= PLATNAME,
	.id_table	= pnp_dev_table,
	.probe		= lirc_wb667_pnp_probe,
	.remove		= __devexit_p(lirc_wb667_pnp_remove),
	.suspend	= lirc_wb667_pnp_suspend,
	.resume		= lirc_wb667_pnp_resume,
};

static int __devinit lirc_wb667_pnp_probe(struct pnp_dev *dev, const struct pnp_device_id *dev_id)
{
	/* CIR */
	
	if (!pnp_port_valid(dev, 0))
		return -ENODEV;
	CIR_BASE = (unsigned int)pnp_port_start(dev, 0);
	CIR_IRQ_NUM = (unsigned short)pnp_irq(dev, 0);


	/* CIR WAKE*/
	
	if (!pnp_port_valid(dev, 1))
		return -ENODEV;
	CIR_WAKE_BASE = (unsigned int)pnp_port_start(dev, 1);
	CIR_WAKE_IRQ_NUM = (unsigned short)pnp_irq(dev, 0); // share the same irq with CIR device.
	
	return 0;
}

static void __devexit lirc_wb667_pnp_remove(struct pnp_dev *dev)
{
}



static int lirc_wb667_pnp_suspend(struct pnp_dev *dev, pm_message_t state)
{
	struct irctl *ir = &w83667hg_irctl;
	struct ir_recv_t *ir_recv = &w83667hg_ir_recv;
	struct ir_send_t *ir_send = &w83667hg_ir_send;

#ifdef IR_667_DEBUG
	printk("%s receive suspend\n", DRVNAME);
#endif

	/* 1. */
	spin_lock(&ir->lock);
	ir->study_state = ST_STUDY_NONE;
	spin_unlock(&ir->lock);

	spin_lock(&ir_recv->lock);
	ir_recv->wake_state = ST_WAKE_NONE;
	spin_unlock(&ir_recv->lock);

	spin_lock(&ir_send->lock);
	ir_send->send_state = ST_SEND_NONE;
	spin_unlock(&ir_send->lock);
	
	/* 2. */
	cir_update(0, CIR_IREN);
	cir_wake_update(0, CIR_WAKE_IREN);
	
	/* 3. */
	cr_enter_ext();
	cr_select_log_dev(CIR_LOG_DEV);
	cr_update(DEACTIVE_LOG_DEV, 0x30);
	
	/* Don't close CIR Wake. When wake-up at power-on, it needs the function. 20091224
	cr_select_log_dev(CIR_WAKE_LOG_DEV);
	cr_update(DEACTIVE_LOG_DEV, 0x30);
	*/
	cr_exit_ext();

	/* 4. */
	lirc_wb667_uninit();

	return 0;
}

static int lirc_wb667_pnp_resume(struct pnp_dev *dev)
{
	int ret;

#ifdef IR_667_DEBUG
	printk("%s receive suspend\n", DRVNAME);
#endif

	/* resume */
	ret = lirc_wb667_init();

	return ret;
}




	#if 0


/* CIR */

static struct pnp_driver lirc_wb667_pnp_cir_driver = {
	.name		= PLATNAME,
	.id_table	= pnp_dev_cir_table,
	.probe		= lirc_wb667_pnp_cir_probe,
	.remove		= __devexit_p(lirc_wb667_pnp_cir_remove),
	.suspend	= lirc_wb667_pnp_cir_suspend,
	.resume		= lirc_wb667_pnp_cir_resume,
};

static int __devinit lirc_wb667_pnp_cir_probe(struct pnp_dev *dev, const struct pnp_device_id *dev_id)
{
	CIR_BASE = (unsigned int)pnp_port_start(dev, 0);
	CIR_IRQ_NUM = (unsigned short)pnp_irq(dev, 0);	
	return 0;
}

static void __devexit lirc_wb667_pnp_cir_remove(struct pnp_dev *dev)
{
}

static int lirc_wb667_pnp_cir_suspend(struct pnp_dev *dev, pm_message_t state)
{
	struct irctl *ir = &w83667hg_irctl;
	struct ir_recv_t *ir_recv = &w83667hg_ir_recv;
	struct ir_send_t *ir_send = &w83667hg_ir_send;

#ifdef IR_667_DEBUG
	printk("%s receive suspend\n", DRVNAME);
#endif

	/* 1. */
	spin_lock(&ir->lock);
	ir->study_state = ST_STUDY_NONE;
	spin_unlock(&ir->lock);

	spin_lock(&ir_recv->lock);
	ir_recv->wake_state = ST_WAKE_NONE;
	spin_unlock(&ir_recv->lock);

	spin_lock(&ir_send->lock);
	ir_send->send_state = ST_SEND_NONE;
	spin_unlock(&ir_send->lock);
	
	/* 2. */
	cir_update(0, CIR_IREN);
	cir_wake_update(0, CIR_WAKE_IREN);
	
	/* 3. */
	cr_enter_ext();
	cr_select_log_dev(CIR_LOG_DEV);
	cr_update(DEACTIVE_LOG_DEV, 0x30);

	cr_select_log_dev(CIR_WAKE_LOG_DEV);
	cr_update(DEACTIVE_LOG_DEV, 0x30);
	cr_exit_ext();

	/* 4. */
	lirc_wb667_uninit();

	return 0;
}

static int lirc_wb667_pnp_cir_resume(struct pnp_dev *dev)
{
	int ret;

#ifdef IR_667_DEBUG
	printk("%s receive suspend\n", DRVNAME);
#endif

	/* resume */
	ret = lirc_wb667_init();

	return ret;
}


/* CIR WAKE */

static struct pnp_driver lirc_wb667_pnp_cir_wake_driver = {
	.name		= PLATNAME_W,
	.id_table	= pnp_dev_cir_wake_table,
	.probe		= lirc_wb667_pnp_cir_wake_probe,
	.remove		= __devexit_p(lirc_wb667_pnp_cir_wake_remove),
};

static int __devinit lirc_wb667_pnp_cir_wake_probe(struct pnp_dev *dev, const struct pnp_device_id *dev_id)
{
	CIR_WAKE_BASE = (unsigned int)pnp_port_start(dev, 0);
	CIR_WAKE_IRQ_NUM = (unsigned short)pnp_irq(dev, 0);
	if (CIR_WAKE_IRQ_NUM > 15) {
		CIR_WAKE_IRQ_NUM = CIR_IRQ_NUM;
	}
	return 0;
}

static void __devexit lirc_wb667_pnp_cir_wake_remove(struct pnp_dev *dev)
{
}
	#endif

	
#endif


//int init_module(void)
int init_module_wb667(void)
{
	int ret;

#ifdef CONFIG_PNP
	if (!nopnp) {

	ret = pnp_register_driver(&lirc_wb667_pnp_driver);
		if (ret < 0)
			return ret;
		
	#if 0		
		ret = pnp_register_driver(&lirc_wb667_pnp_cir_driver);
		if (ret < 0)
			return ret;
		ret = pnp_register_driver(&lirc_wb667_pnp_cir_wake_driver);
		if (ret < 0)
			return ret;
	#endif		
	
	}
#endif /* CONFIG_PNP */

	if (nopnp) {
		lirc_wb667_dev = platform_device_alloc(PLATNAME, 0);
		if (!lirc_wb667_dev)
			return -ENOMEM;
		ret = platform_device_add(lirc_wb667_dev);
		if (ret) {
			platform_device_put(lirc_wb667_dev);
			return ret;
		}
		ret = platform_driver_register(&lirc_wb667_driver);
		if (ret) {
			printk("%s platform register returned %d\n", DRVNAME, ret);
			platform_device_del(lirc_wb667_dev);
			platform_device_put(lirc_wb667_dev);
			return ret;
		}
	}

	ret = lirc_wb667_init();
	if (ret) {
		return ret;
	}

	return 0;
}

//void cleanup_module(void)
void cleanup_module_wb667(void)
{
	struct platform_device *pdev = lirc_wb667_dev;
	lirc_wb667_dev = NULL;

#ifdef CONFIG_PNP
	if (!nopnp) {
		pnp_unregister_driver(&lirc_wb667_pnp_driver);
	#if 0		
		pnp_unregister_driver(&lirc_wb667_pnp_cir_driver);
		pnp_unregister_driver(&lirc_wb667_pnp_cir_wake_driver);
	#endif		
	}
#endif /* CONFIG_PNP */

	if (nopnp) {
		platform_driver_unregister(&lirc_wb667_driver);
		platform_device_unregister(pdev);
	}

	lirc_wb667_uninit();
}

module_init(init_module_wb667);
module_exit(cleanup_module_wb667);


