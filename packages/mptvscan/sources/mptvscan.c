/* Embedded v4l2 Analog TV Channels Scanner for MPlayer
 * Originally developped for the GeeXboX project.
 * Benjamin Zores (C) 2005 - <ben@geexbox.org>
 * License : GPL
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <linux/videodev2.h>

#include "frequencies.h"

#define MPTVSCAN_ACTION_SCAN 0
#define MPTVSCAN_ACTION_INPUTS 1
#define MPTVSCAN_ACTION_STANDARDS 2

#define MPTVSCAN_DFLT_DEVICE "/dev/video0"
#define MPTVSCAN_DFLT_CHANLIST "all"

#define MPTVSCAN_DEBUG_INPUT "for debug only"

#define BUFSIZE 256

struct channel_t {
  char *chan;
  char *name;
};

struct channels_t {
  int nr_channels;
  struct channel_t **chans;
};

static struct channel_t *
chan_new (char *chan, char *name)
{
  struct channel_t *c;

  c = (struct channel_t *) malloc (sizeof (struct channel_t));
  c->chan = strdup (chan);
  c->name = strdup (name);

  return c;
}

static void
chan_free (struct channel_t *chan)
{
  if (chan->chan)
    free (chan->chan);
  if (chan->name)
    free (chan->name);
  free (chan);
}

static struct channels_t *
channels_table_new (void)
{
  struct channels_t *channels;

  channels = (struct channels_t *) malloc (sizeof (struct channels_t));
  channels->chans = (struct channel_t **) malloc (sizeof (struct channel_t *));
  channels->nr_channels = 0;

  return channels;
}

static void
channels_table_free (struct channels_t *channels)
{
  int i;

  for (i = 0; i < channels->nr_channels; i++)
    chan_free (channels->chans[i]);
  free (channels);
}

static void
channels_table_add_new (struct channels_t *channels, struct channel_t *chan)
{
  channels->chans = realloc (channels->chans, (channels->nr_channels + 1)
                             * sizeof (struct channel_t *));
  channels->chans[channels->nr_channels] = chan;
  channels->nr_channels++;    
}

static char *
channels_table_list (struct channels_t *channels)
{
  char *buffer = NULL, c[BUFSIZE];
  int i;
  
  buffer = (char *) malloc (BUFSIZE * sizeof (char));
  for (i = 0; i < channels->nr_channels; i++)
    {
      sprintf (c, "%s-%s,",
               channels->chans[i]->chan, channels->chans[i]->name);
      buffer = realloc (buffer, strlen (buffer) + strlen (c));
      strcat (buffer, c);
    }
 
  if (channels->nr_channels)
    buffer[strlen (buffer) - 1] = '\0'; /* erase trailing ',' char */

  return buffer;
}

static int
xioctl (int fd, int request, void *arg)
{
  int r;

  do {
    r = ioctl (fd, request, arg);
  } while (r == -1 && errno == EINTR);

  return r;
}

static int
v4l2_open_device (char *dev)
{
  struct stat st; 
  int fd;

  if (stat (dev, &st) == -1)
    {
      fprintf (stderr, "Cannot identify '%s': %d, %s\n",
               dev, errno, strerror (errno));
      exit (EXIT_FAILURE);
    }

  if (!S_ISCHR (st.st_mode))
    {
      fprintf (stderr, "%s is no device\n", dev);
      exit (EXIT_FAILURE);
    }

  fd = open (dev, O_RDWR | O_NONBLOCK, 0);

  if (fd == -1)
    {
      fprintf (stderr, "Cannot open '%s': %d, %s\n",
               dev, errno, strerror (errno));
      exit (EXIT_FAILURE);
    }

  return fd;
}

static void
v4l2_close_device (int fd)
{
  if (close (fd) == -1)
    exit (EXIT_FAILURE);
  
  fd = -1;
}

static void
v4l2_init_device (int fd, char *dev)
{
  struct v4l2_capability cap;

  if (xioctl (fd, VIDIOC_QUERYCAP, &cap) == -1)
    {
      if (errno == EINVAL)
        {
          fprintf (stderr, "%s is no V4L2 device\n", dev);
          exit (EXIT_FAILURE);
        }
      else
        {
          perror ("VIDIOC_QUERYCAP");
          exit (EXIT_FAILURE);
        }
    }

  if (!(cap.capabilities & V4L2_CAP_TUNER))
    {
      fprintf (stderr, "%s is no video tuner device\n", dev);
      exit (EXIT_FAILURE);
    }

}

static void
v4l2_get_input_list (int fd)
{
  struct v4l2_input input;
  
  memset (&input, 0, sizeof (input));
  input.index = 0;
  
  while (xioctl (fd, VIDIOC_ENUMINPUT, &input) == 0)
    {
      if (strcmp ((char *) input.name, MPTVSCAN_DEBUG_INPUT))
        printf ("%s\n", input.name);
      input.index++;
    }

  if (errno != EINVAL || input.index == 0)
    {
      perror ("VIDIOC_ENUMSTD");
      exit (EXIT_FAILURE);
    }
}

static void
v4l2_set_input (int fd, char *input_id)
{
  struct v4l2_input input;
  
  memset (&input, 0, sizeof (input));
  input.index = 0;

  if (!input_id)
    {
      if (xioctl (fd, VIDIOC_S_INPUT, &input) == -1)
        {
          perror ("VIDIOC_S_INPUT");
          exit (EXIT_FAILURE);
        }
    }
  else
    while (xioctl (fd, VIDIOC_ENUMINPUT, &input) == 0)
      {
        if (!strcmp ((char *) input.name, input_id))
          {
            if (xioctl (fd, VIDIOC_S_INPUT, &input) == -1)
              {
                perror ("VIDIOC_S_INPUT");
                exit (EXIT_FAILURE);
              }
            break;
          }
        input.index++;
      }
}

static void
v4l2_set_standard (int fd, char *standard_id)
{
  struct v4l2_standard standard;

  memset (&standard, 0, sizeof (standard));
  standard.index = 0;
  standard.id = V4L2_STD_PAL;

  if (!standard_id)
    {
      if (xioctl (fd, VIDIOC_S_STD, &standard.id) == -1)
        {
          perror ("VIDIOC_S_STD");
          exit (EXIT_FAILURE);
        }
    }
  else
    while (xioctl (fd, VIDIOC_ENUMSTD, &standard) == 0)
      {
        if (!strcmp ((char *) standard.name, standard_id))
          {
            if (xioctl (fd, VIDIOC_S_STD, &standard.id) == -1)
              {
                perror ("VIDIOC_S_STD");
                exit (EXIT_FAILURE);
              }
            break;
          }
        standard.index++;
      }
}

static void
v4l2_get_standard_list (int fd, char *in)
{
  struct v4l2_input input;
  struct v4l2_standard standard;

  memset (&input, 0, sizeof (input));
  input.index = 0;

  if (!in)
    {
      if (xioctl (fd, VIDIOC_ENUMINPUT, &input) == -1)
        {
          perror ("VIDIOC_ENUM_INPUT");
          exit (EXIT_FAILURE);
        }
    }
  else
    while (xioctl (fd, VIDIOC_ENUMINPUT, &input) == 0)
      {
        if (!strcmp ((char *) input.name, in))
          break;
        input.index++;
      }

  memset (&standard, 0, sizeof (standard));
  standard.index = 0;

  while (xioctl (fd, VIDIOC_ENUMSTD, &standard) == 0)
    {
      if (standard.id & input.std)
        printf ("%s\n", standard.name);
      standard.index++;
    }

  if (errno != EINVAL || standard.index == 0)
    {
      perror ("VIDIOC_ENUMSTD");
      exit (EXIT_FAILURE);
    }
}

static void
v4l2_set_tuner_frequence (int fd, unsigned long freq)
{
  struct v4l2_frequency f;

  memset (&f, 0, sizeof (f));
  f.type = V4L2_TUNER_ANALOG_TV;
  f.frequency = freq;

  if (xioctl (fd, VIDIOC_S_FREQUENCY, &f) == -1)
    {
      perror ("VIDIOC_S_FREQUENCY");
      exit (EXIT_FAILURE);
    }
}

static int
v4l2_is_tuned (int fd)
{
  struct v4l2_tuner tuner;

  usleep (10000);
  memset (&tuner, 0, sizeof (tuner));
  if (xioctl (fd, VIDIOC_G_TUNER, &tuner) == -1)
    return 0;
  return tuner.signal ? 1 : 0;
}

static int
v4l2_get_all_channels_list (struct CHANLIST **list)
{
  int i, j, k, len, need_add;

  len = 0;
  for (i = 0; chanlists[i].name; i++)
    {
      for (j = 0; j < chanlists[i].count; j++)
        {
          need_add = 1;
          for (k = 0; k < len; k++)
            {
              if (!strcmp (chanlists[i].list[j].name, list[k]->name))
                {
                  need_add = 0;
                  break; /* channel already present in list */
                }
            }

          if (need_add)
            {
              list[len] = &chanlists[i].list[j];
              len++;
              list = realloc (list, (len + 1) * sizeof (struct CHANLIST *));
            }
        }
    }

  return len;
}

static void
v4l2_scan_channels (int fd, char *list,
                    struct channels_t *channels, int progress)
{
  int tuned, i, j;
  float f;

  if (!list)
    return;

  if (!strcmp (list, MPTVSCAN_DFLT_CHANLIST))
    {
      struct CHANLIST **clist = NULL;
      int len;

      clist = (struct CHANLIST **) malloc (sizeof (struct CHANLIST *));
      len = v4l2_get_all_channels_list (clist);
      if (!clist)
        return;
      
      for (j = 0; j < len; j++)
        {
          f = clist[j]->freq / 62.5;
          v4l2_set_tuner_frequence (fd, f);
          usleep (200000);
          tuned = v4l2_is_tuned (fd);

          if (progress)
            {
              printf ("%d\n", (int) (100 * j / len));
              fflush (stdout);
            }

          if (tuned) /* Detected a valid TV channel */
            {
              struct channel_t *chan;
              char chan_name[128];

              sprintf (chan_name, "Channel_%s", clist[j]->name);
              chan = chan_new (clist[j]->name, chan_name);
              channels_table_add_new (channels, chan);
            }
        }
    }
  else
    {
      for (i = 0; chanlists[i].name; i++)
        if (!strcmp (list, chanlists[i].name))
          for (j = 0; j < chanlists[i].count; j++)
            {
              f = chanlists[i].list[j].freq / 62.5;
              v4l2_set_tuner_frequence (fd, f);
              usleep (200000);
              tuned = v4l2_is_tuned (fd);
              
              if (progress)
                {
                  printf ("%d\n", (int) (100 * (j + 1) / chanlists[i].count));
                  fflush (stdout);
                }

              if (tuned) /* Detected a valid TV channel */
                {
                  struct channel_t *chan;
                  char chan_name[128];
                  
                  sprintf (chan_name, "Channel_%s", chanlists[i].list[j].name);
                  chan = chan_new (chanlists[i].list[j].name, chan_name);
                  channels_table_add_new (channels, chan);
                }
            }
    }
}

static char *
set_chanlist (char *chanlist_id)
{
  int i;

  if (chanlist_id)
    for (i=0; chanlists[i].name; i++)
      if (!strcmp (chanlists[i].name, chanlist_id))
        return chanlists[i].name;
  return MPTVSCAN_DFLT_CHANLIST;
}

static void
display_chanlist (void)
{
  int i;

  for (i=0; chanlists[i].name; i++)
    printf ("%s\n", chanlists[i].name);
}

static void
print_usage (void)
{
  printf ("MPlayer Analog TV Channels scanner\n");
  printf ("-h : Display this help message\n");
  printf ("-p : Display scan progression (interface with dialog)\n");
  printf ("-d device : Video device to use\n");
  printf ("-i[input] : Use specified input X (default is 0)\n");
  printf ("-s[standard] : Set to specified standard X\n");
  printf ("-c[chanlist] : Use specified chanlist X (default is all)\n");
}

int
main (int argc, char **argv)
{
  int fd, c, action = MPTVSCAN_ACTION_SCAN;
  char *dev = MPTVSCAN_DFLT_DEVICE;
  char *list;
  char *std = NULL, *input = NULL, *list_id = NULL;
  struct channels_t *channels = NULL;
  char *channels_list = NULL;
  int done = 0, progress = 0;

  while (1)
    {
      c = getopt (argc, argv, "hd:i::s::c::p");
      if (c == -1)
        break;

      switch (c)
        {
        case 'h':
          print_usage ();
          return 0;

        case 'd':
          dev = optarg;
          break;

        case 'i':
          if (!optarg)
            action = MPTVSCAN_ACTION_INPUTS;
          else
            input = optarg;
          break;

        case 's':
          if (!optarg)
            action = MPTVSCAN_ACTION_STANDARDS;
          else
            std = optarg;
          break;

        case 'c':
          if (!optarg)
            {
              display_chanlist ();
              done = 1;
            }
          else
            list_id = optarg;
          break;

        case 'p':
          progress = 1;

        case '?':
          break;

        default:
          break;
        }
    }

  if (done)
    return 0;

  fd = v4l2_open_device (dev);
  v4l2_init_device (fd, dev);

  switch (action)
    {
    case MPTVSCAN_ACTION_SCAN:
      v4l2_set_input (fd, input);
      v4l2_set_standard (fd, std);
      channels = channels_table_new ();
      list = set_chanlist (list_id);
      v4l2_scan_channels (fd, list, channels, progress);
      channels_list = channels_table_list (channels);
      if (channels_list && *channels_list)
        fprintf (progress ? stderr : stdout, "channels=%s\n", channels_list);
      break;

    case MPTVSCAN_ACTION_INPUTS:
      v4l2_get_input_list (fd);
      break;

    case MPTVSCAN_ACTION_STANDARDS:
      v4l2_get_standard_list (fd, input);
      break;

    default:
      break;
    }

  if (channels_list)
    free (channels_list);
  if (channels)
    channels_table_free (channels);
  v4l2_close_device (fd);

  return 0;
}
