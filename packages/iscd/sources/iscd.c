/* iscd.c v0.0.2 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/cdrom.h>

int
main (int argc, char **argv)
{
  int fd, capabilites;

  if (argc != 2)
    {
      fprintf (stderr, "usage: %s device\n", argv[0]);
      return 1;
    }

  fd = open (argv[1], O_RDONLY | O_NONBLOCK);
  if (fd < 0)
    {
      perror ("Can't open device");
      return 2;
    }
  if ((capabilites=ioctl (fd, CDROM_GET_CAPABILITY)) < 0)
    {
      perror ("Can't get capabilities");
      return 3;
    }

  if (capabilites & CDC_DVD)
    printf ("DVD\n");
  else
    printf ("CD\n");

  close (fd);
  return 0;
}
