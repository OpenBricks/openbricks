/*
 * UDEV based volume automounter for GeeXboX
 *
 * Copyright (C) 2009 Benjamin Zores
 *
 * This file is part of GeeXboX.
 *
 * GeeXboX is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * GeeXboX is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with GeeXboX; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mntent.h>
#include <sys/select.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <libudev.h>

#define MTAB_FILE          "/etc/mtab"
#define FILTER_SUBSYTEM    "block"
#define FILTER_BLK_LOOP    "block/loop"
#define FILTER_BLK_RAM     "block/ram"

#define M_FREE(p) if (p) free (p)

//#define DEBUG

typedef struct volume_s {
  char *syspath;
  char *device;
  char *name;
  char *unique_name;
  char *type;
  char *fstype;
  int mounted;
  int unique_id;
} volume_t;

typedef struct volume_list_s {
  volume_t **volumes;
  int count;
} volume_list_t;

static const struct {
  const char *name;
  const char *bus;
} drv_bus_mapping[] = {
  { "SATA",                   "scsi"                     },
  { "SATA",                   "ata"                     },
  { "IDE",                    "ide"                     },
  { "USB",                    "usb"                     },
  { "FireWire",               "ieee1394"                     },
  { "CCW",                    "ccw"                     },
  { NULL,                     0                                         }
};

static const struct {
  const char *name;
  const char *type;
} drv_type_mapping[] = {
  { "Disk",                   "disk"                    },
  { "CD-ROM",                 "cdrom"                   },
  { "Floppy",                 "floppy"                  },
  { "Tape",                   "tape"                    },
  { "CompactFlash",           "compact_flash"           },
  { "MemoryStick",            "memory_stick"            },
  { "SmartMedia",             "smart_media"             },
  { "SD/MMC",                 "sd_mmc"                  },
  { "ZIP",                    "zip"                     },
  { "JAZ",                    "jaz"                     },
  { "FlashKey",               "flashkey"                },
  { "MagnetoOptical",         "optical"                      },
  { NULL,                     0                                         }
};


static const struct {
  const char *name;
  const char *property;
} vol_disc_mapping[] = {
  { "BLURAY",                 "ID_CDROM_MEDIA_BD"                   },
  { "CDDA",                   "ID_CDROM_MEDIA_TRACK_COUNT_AUDIO"    },
  { "VCD",                    "ID_CDROM_MEDIA_VCD"                  },
  { "SVCD",                   "ID_CDROM_MEDIA_SDVD"                 },
  { "DVD",                    "ID_CDROM_MEDIA_DVD"                  },
  { "CD",                     "ID_CDROM_MEDIA_CD"                   },
  { NULL,                     NULL                                  }
};

static int udev_exit = 0;
static volume_list_t *vlist = NULL;
static pthread_mutex_t vlist_mutex = PTHREAD_MUTEX_INITIALIZER;

static volume_t *
volume_new (void)
{
  volume_t *v;

  v = calloc (1, sizeof (volume_t));

  return v;
}

static void
volume_free (volume_t *v)
{
  if (!v)
    return;

  M_FREE (v->syspath);
  M_FREE (v->device);
  M_FREE (v->name);
  M_FREE (v->unique_name);
  M_FREE (v->type);
  M_FREE (v->fstype);
  M_FREE (v);
}

static void
volume_display (volume_t *v)
{
#ifdef DEBUG
  if (!v)
    return;

  printf ("Volume Syspath: %s\n", v->syspath);
  printf ("Volume Device:  %s\n", v->device);
  printf ("Volume Name:    %s\n", v->name);
  printf ("Volume Type:    %s\n", v->type);
  printf ("Volume FS Type: %s\n", v->fstype);
  printf ("Volume Mounted: %s\n", v->mounted ? "yes" : "no");
  printf ("\n");
#endif /* DEBUG */
}

static void
volume_append_name (volume_t *v, const char *str)
{
  if (!v || !str)
    return;

  if (!v->name)
    v->name = strdup (str);
  else
  {
    char mp[1024] = { 0 };

    snprintf (mp, sizeof (mp), "%s %s", v->name, str);
    free (v->name);
    v->name = strdup (mp);
  }
}

static void
volume_list_add (volume_t *v)
{
  if (!vlist)
    vlist = calloc (1, sizeof (volume_list_t));

  if (v)
  {
    vlist->count++;
    vlist->volumes = realloc (vlist->volumes,
                              vlist->count * sizeof (volume_t *));
    vlist->volumes[vlist->count - 1] = v;
  }
}

static void
volume_list_remove (int n)
{
  if (!vlist)
    return;

  if (n >= vlist->count)
    return;

  if (vlist->volumes[n])
  {
    int i;

    volume_free (vlist->volumes[n]);
    for (i = n ; i < vlist->count - 1 ; i++)
      vlist->volumes[i] = vlist->volumes[i+1];
    vlist->count--;
    vlist->volumes[vlist->count] = NULL;
  }
}

static void
volume_list_free (void)
{
  if (!vlist)
    return;

  if (vlist->volumes)
  {
    int i;

    for (i = 0 ; i < vlist->count ; i++)
      volume_list_remove (i);
    free (vlist->volumes);
  }
  free (vlist);
}

static int
volume_list_exists (const char *syspath)
{
  int i;

  if (!vlist || !syspath)
    return 0;

  for (i = 0 ; i < vlist->count ; i++)
    if (vlist->volumes[i]->syspath &&
        !strcmp (vlist->volumes[i]->syspath, syspath))
    {
      return 1;
    }

  return 0;
}

static int
volume_list_get (const char *syspath)
{
  int i;

  if (!vlist || !syspath)
    return -1;

  for (i = 0 ; i < vlist->count ; i++)
    if (vlist->volumes[i]->syspath &&
        !strcmp (vlist->volumes[i]->syspath, syspath))
    {
      return i;
    }

  return -1;
}

static int
disk_get_number (int major, int minor)
{
  switch (major)
  {
  case 3: /* Primary IDE interface */
    return (minor <= 63) ? 1 : 2;
  case 8: /* SCSI disk devices */
    return ((minor / 16) + 1);
  case 11: /* SCSI CD-ROM devices */
    return (minor + 1);
  case 22: /* Secondary IDE interface */
    return (minor <= 63) ? 3 : 4;
  default:
    break;
  }

  return 0;
}

static int
mtab_is_mounted (const char *devname)
{
  struct mntent *mnt;
  FILE *fp;

  if (!devname)
    return 0;

  fp = fopen (MTAB_FILE, "r");
  if (!fp)
    return 0;

  while ((mnt = getmntent (fp)))
  {
    if (!strcmp (mnt->mnt_fsname, devname))
    {
      endmntent (fp);
      return 1;
    }
  }

  endmntent (fp);
  return 0;
}

static int
get_property_value_int (struct udev_device *dev, const char *property)
{
  const char *value;

  value = udev_device_get_property_value (dev, property);
  if (!value)
    return 0;

  return atoi (value);
}

static int 
volume_set_unique_name(volume_t *v)
{
  int i, changed=0;

  if (!vlist || !v || !v->name)
    return -1;

  for (i = 0 ; i < vlist->count ; i++)
    if (vlist->volumes[i] != v &&
        vlist->volumes[i]->name &&
        !strcmp (vlist->volumes[i]->name, v->name))
    {
      if (vlist->volumes[i]->unique_id >= v->unique_id)
        v->unique_id=vlist->volumes[i]->unique_id+1;
      changed=1;
    }

  if (changed)
  {
    char mp[1024] = { 0 };
    snprintf(mp, sizeof(mp), "%s #%i", v->name, v->unique_id);
    v->unique_name=strdup(mp);
  }

  return changed;
}

static volume_t *
handle_cdrom (struct udev_device *dev)
{
  volume_t *v;

  int major, minor, disk, media;
  const char *bus, *fs, *label, *vendor, *model, *device;
  char dsk[8] = { 0 }, part[8] = { 0 };
  const char *bus_type = NULL;
  int i;

  /* ensure a CD has been inserted */
  media = get_property_value_int (dev, "ID_CDROM_MEDIA");
  if (media  != 1)
    return NULL;

  v = volume_new ();

  device    = udev_device_get_devnode (dev);
  major     = get_property_value_int (dev, "MAJOR");
  minor     = get_property_value_int (dev, "MINOR");

  bus       = udev_device_get_property_value (dev, "ID_BUS");
  fs        = udev_device_get_property_value (dev, "ID_FS_TYPE");

  vendor    = udev_device_get_property_value (dev, "ID_VENDOR");
  model     = udev_device_get_property_value (dev, "ID_MODEL");
  label     = udev_device_get_property_value (dev, "ID_FS_LABEL");

  disk = disk_get_number (major, minor);
  snprintf (dsk,  sizeof (dsk),  "#%d", disk);

  v->syspath = strdup (udev_device_get_syspath (dev));
  v->device  = strdup (device ? device : "Unknown");
  v->fstype  = strdup (fs ? fs : "Unknown");
  v->mounted = mtab_is_mounted (device);

  /* check for disc property: CDDA, VCD, SVCD, DVD, Data CD/DVD */
  for (i = 0; vol_disc_mapping[i].name; i++)
    if (udev_device_get_property_value (dev, vol_disc_mapping[i].property))
    {
      v->type = strdup (vol_disc_mapping[i].name);
      break;
    }

  /* if no type has been found, consider it's data */
  if (!v->type)
    v->type = strdup ("CD");

  /* get bus' type */
  for (i = 0; drv_bus_mapping[i].name; i++)
    if (bus && !strcmp (bus, drv_bus_mapping[i].bus))
    {
      bus_type = drv_bus_mapping[i].name;
      break;
    }
  if (!bus_type)
    bus_type = "Unknown";

  volume_append_name (v, bus_type);
  volume_append_name (v, v->type);
  volume_append_name (v, dsk);

  if (label)
    volume_append_name (v, label);
  else
  {
    if (vendor)
    volume_append_name (v, vendor);
    if (model)
      volume_append_name (v, model);
  }
  volume_append_name (v, part);

  volume_set_unique_name(v);

  return v;
}

static volume_t *
handle_dm_disk (struct udev_device *dev)
{
  int major, minor;
  const char *usage, *fs;
  const char *devname, *dm_name, *label, *device;
  volume_t *v;
  const char *drive_type = NULL;

  /* ensure we're dealing with a proper filesystem on partition */
  usage = udev_device_get_property_value (dev, "ID_FS_USAGE");
  if (!usage || strcmp (usage, "filesystem"))
    return NULL;;

  v = volume_new ();

  device    = udev_device_get_devnode (dev);
  major     = get_property_value_int (dev, "MAJOR");
  minor     = get_property_value_int (dev, "MINOR");

  fs        = udev_device_get_property_value (dev, "ID_FS_TYPE");
  devname   = udev_device_get_property_value (dev, "DEVNAME");

  dm_name    = udev_device_get_property_value (dev, "DM_NAME");
  label     = udev_device_get_property_value (dev, "ID_FS_LABEL");

  v->syspath = strdup (udev_device_get_syspath (dev));
  v->device  = strdup (device ? device : "Unknown");
  v->type    = strdup ("HDD");
  v->fstype  = strdup (fs ? fs : "Unknown");
  v->mounted = mtab_is_mounted (device);

  drive_type = "DM";

  volume_append_name (v, drive_type);
  volume_append_name (v, dm_name);
  if (label)
    volume_append_name (v, label);

  volume_set_unique_name (v);

  return v;
}

static volume_t *
handle_partition (struct udev_device *dev)
{
  int major, minor, disk, partition;
  const char *usage, *type, *bus, *fs;
  const char *devname, *label, *vendor, *model, *device;
  volume_t *v;
  char dsk[8] = { 0 }, part[8] = { 0 };
  const char *bus_type = NULL, *drive_type = NULL;
  int i;

  /* ensure we're dealing with a proper filesystem on partition */
  usage = udev_device_get_property_value (dev, "ID_FS_USAGE");
  if (!usage || strcmp (usage, "filesystem"))
    return NULL;;

  v = volume_new ();

  device    = udev_device_get_devnode (dev);
  major     = get_property_value_int (dev, "MAJOR");
  minor     = get_property_value_int (dev, "MINOR");
  partition = get_property_value_int (dev, "DKD_PARTITION_NUMBER");

  type      = udev_device_get_property_value (dev, "ID_TYPE");
  bus       = udev_device_get_property_value (dev, "ID_BUS");
  fs        = udev_device_get_property_value (dev, "ID_FS_TYPE");
  devname   = udev_device_get_property_value (dev, "DEVNAME");

  vendor    = udev_device_get_property_value (dev, "ID_VENDOR");
  model     = udev_device_get_property_value (dev, "ID_MODEL");
  label     = udev_device_get_property_value (dev, "ID_FS_LABEL");

  disk = disk_get_number (major, minor);
  snprintf (dsk,  sizeof (dsk),  "#%d", disk);
  snprintf (part, sizeof (part), "(%d)", partition);

  v->syspath = strdup (udev_device_get_syspath (dev));
  v->device  = strdup (device ? device : "Unknown");
  v->type    = strdup ("HDD");
  v->fstype  = strdup (fs ? fs : "Unknown");
  v->mounted = mtab_is_mounted (device);

  /* get bus' type */
  for (i = 0; drv_bus_mapping[i].name; i++)
    if (bus && !strcmp (bus, drv_bus_mapping[i].bus))
    {
      bus_type = drv_bus_mapping[i].name;
      break;
    }
  if (!bus_type)
    bus_type = "Unknown";

  /* get drive's type */
  for (i = 0; drv_type_mapping[i].name; i++)
    if (type && !strcmp (type, drv_type_mapping[i].type))
    {
      drive_type = drv_type_mapping[i].name;
      break;
    }
  if (!drive_type)
    drive_type = "Unknown";

  volume_append_name (v, bus_type);
  volume_append_name (v, drive_type);
  volume_append_name (v, dsk);
  if (label)
    volume_append_name (v, label);
  else
  {
    if (vendor)
    volume_append_name (v, vendor);
    if (model)
      volume_append_name (v, model);
  }
  volume_append_name (v, part);

  volume_set_unique_name (v);

  return v;
}

static void
scan_device_properties (struct udev_device *dev)
{
#ifdef DEBUG
  struct udev_list_entry *list, *p;

  if (!dev)
    return;

  list = udev_device_get_properties_list_entry (dev);
  udev_list_entry_foreach (p, list)
  {
    printf ("Property Name: %s - %s\n",
            udev_list_entry_get_name (p),
            udev_device_get_property_value (dev, udev_list_entry_get_name (p))
      );
  }
#endif /* DEBUG */
}

static void
add_device (struct udev_device *dev)
{
  const char *syspath;
  const char *devtype;
  const char *disktype;
  volume_t *v = NULL;

  /* retrieve the device's syspath */
  syspath = udev_device_get_syspath (dev);
  if (!syspath)
    return;

  /* ensure the device does not already exists in our table */
  if (volume_list_exists (syspath))
    return;

  devtype = udev_device_get_property_value (dev, "DEVTYPE");
  if (!devtype)
    return;

  if (!strcmp (devtype, "disk"))
  {
    disktype = udev_device_get_property_value (dev, "DM_NAME");
    if (disktype) 
      v = handle_dm_disk (dev);
    else 
    { 
      disktype = udev_device_get_property_value (dev, "ID_TYPE");
      /* only care about CDs */
      if (disktype && strcmp(disktype, "cd"))
        v = handle_cdrom (dev);
    }
  }
  else if (!strcmp (devtype, "partition"))
    v = handle_partition (dev);

  if (v)
  {
    char cmd[1024] = { 0 };

    volume_display  (v);
    volume_list_add (v);

    snprintf (cmd, sizeof (cmd), "/usr/bin/hmount '%s' '%s' '%s' '%s'",
              v->type, v->device, v->unique_name?v->unique_name:v->name, v->fstype);

    printf ("[automountd] Executing: %s\n", cmd);
    system (cmd);
  }
}

static void
remove_device (struct udev_device *dev)
{
  char cmd[1024] = { 0 };
  const char *syspath;
  volume_t *v;
  int c;

  /* retrieve the device's syspath */
  syspath = udev_device_get_syspath (dev);
  if (!syspath)
    return;

  c = volume_list_get (syspath);
  if (c == -1)
    return;

  v = vlist->volumes[c];
  if (!v)
    return;

  snprintf (cmd, sizeof (cmd), "/usr/bin/humount '%s' '%s' '%s' '%s'",
            v->type, v->device, v->name, v->fstype);

  printf ("[automountd] Executing: %s\n", cmd);
  system (cmd);
  volume_list_remove (c);

}

static void
change_device (struct udev_device *dev)
{
  remove_device (dev);
  add_device (dev);
}

static void
handle_device (struct udev_device *dev, const char *action)
{
  scan_device_properties (dev);

  pthread_mutex_lock (&vlist_mutex);

  /* check whether we need to add or remove the device from our list */
  if (!action || !strcmp (action, "add"))
    add_device (dev);
  else if (!strcmp (action, "change"))
    change_device (dev);
  else
    remove_device (dev);

  pthread_mutex_unlock (&vlist_mutex);
}

static void
sig_handler (int signum)
{
  if (signum == SIGINT || signum == SIGTERM)
    udev_exit = 1;
}

int
main (int argc, char **argv)
{
  struct udev *udev;
  struct udev_enumerate *en;
  struct udev_monitor *monitor;
  struct udev_list_entry *device_list, *device_p;

  struct sigaction act;
  sigset_t mask;
  fd_set fds;
  int err;

  /* do we need to daemonize ? */
  if (argc >= 2 && !strcmp (argv[1], "-d"))
    daemon (0, 0);

  /* udev listeners */
  udev = udev_new ();
  if (!udev)
    goto err_udev;

  en = udev_enumerate_new (udev);
  if (!en)
    goto err_enumerator;

  monitor = udev_monitor_new_from_netlink (udev, "udev");
  if (!monitor)
    goto err_monitor;

  err = udev_monitor_enable_receiving (monitor);
  if (err)
    goto err_receiver;

  err =
    udev_monitor_filter_add_match_subsystem_devtype (monitor,
                                                     FILTER_SUBSYTEM, NULL);
  if (err)
    goto err_filter;

  /* set signal handlers */
  act.sa_handler = sig_handler;
  act.sa_flags   = SA_RESTART;

  sigemptyset (&act.sa_mask);
  sigaction   (SIGINT, &act, NULL);
  sigaction   (SIGTERM, &act, NULL);
  sigemptyset (&mask);
  sigaddset   (&mask, SIGINT);
  sigaddset   (&mask, SIGTERM);
  sigprocmask (SIG_UNBLOCK, &mask, NULL);

  /* we only care about block devices */
  udev_enumerate_add_match_subsystem (en, FILTER_SUBSYTEM);

  /* scan for available devices */
  udev_enumerate_scan_devices (en);
  device_list = udev_enumerate_get_list_entry (en);

  udev_list_entry_foreach (device_p, device_list)
  {
    struct udev_device *dev;
    const char *syspath;

    /* based on sys path, discard irrelevant ones */
    syspath = udev_list_entry_get_name (device_p);
    if (strstr (syspath, FILTER_BLK_LOOP) ||
        strstr (syspath, FILTER_BLK_RAM))
      continue;

    dev = udev_device_new_from_syspath (udev, syspath);
    if (!dev)
      continue;

    handle_device (dev, NULL);
    udev_device_unref (dev);
  }

  /* listen to all further udev events */
  while (!udev_exit)
  {
    int count;

    FD_ZERO (&fds);
    FD_SET  (udev_monitor_get_fd (monitor), &fds);

    count = select (udev_monitor_get_fd (monitor) + 1, &fds, NULL, NULL, NULL);
    if (count < 0)
      continue;

    if (FD_ISSET (udev_monitor_get_fd (monitor), &fds))
    {
      struct udev_device *dev;
      const char *syspath;
      const char *action;

      dev = udev_monitor_receive_device (monitor);
      if (!dev)
        continue;

      action = udev_device_get_action (dev);
      if (!action)
        goto err_no_action;

      syspath = udev_device_get_syspath (dev);
      if (strstr (syspath, FILTER_BLK_LOOP) ||
          strstr (syspath, FILTER_BLK_RAM))
        goto err_no_action;

      if (!strcmp (action, "add") ||
          !strcmp (action, "remove") ||
          !strcmp (action, "change"))
        handle_device (dev, action);

    err_no_action:
      udev_device_unref (dev);
    }
  }

  /* erase the volumes list */
  pthread_mutex_destroy (&vlist_mutex);
  volume_list_free ();

err_filter:
err_receiver:
  udev_monitor_unref (monitor);
err_monitor:
  udev_enumerate_unref (en);
err_enumerator:
  udev_unref (udev);
err_udev:

  return 0;
}
