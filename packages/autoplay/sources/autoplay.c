/* autoplay.c v0.0.6 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/cdrom.h>
#include <dirent.h>
#include <limits.h>
#include <errno.h>
#include <sys/mount.h>
#include <sys/file.h>


#ifndef MNT_DETACH
#define MNT_DETACH	0x00000002	/* Just detach from the tree */
#endif

#define BUF_LEN 50

static char *xcd_exts[] = {"dat", NULL};
static char **exts;
static char *playlist = NULL;
static int playlist_len = 0;

typedef struct _cd_drive
{
  char *dev;
  char *mnt;
  int fd;
  int status;
}*cd_drive;


static char **
get_extensions (char *filename)
{
  char **extensions, ext[32];
  FILE *fp;
  int n = 1;

  if (filename == NULL)
    return NULL;

  fp = fopen (filename, "r");
  if (fp == NULL)
    return NULL;

  extensions = (char **) malloc (sizeof (*extensions));
  *extensions = NULL;

  while (fgets (ext, sizeof (ext), fp))
    {
      char **l, *e;
      int s = strlen (ext);

      if (ext[s-1] == '\n')
        {
          ext[s-1] = '\0';
          s--;
        }
      e = (char *) malloc (s + 1);
      extensions = (char **) realloc (extensions, ++n * sizeof (*extensions));
      extensions = (char **) realloc (extensions, ++n * sizeof (*extensions));
      strcpy (e, ext);
      for (l=extensions; *l; l++);
      *l++ = e;
      *l = NULL;
    }

  fclose (fp);
  return extensions;
}

static void
free_extensions (char **extensions)
{
  if (extensions != NULL)
    {
      char **l = extensions;
      while (*l)
        free (*l++);
      free (extensions);
    }
}

static char *fullname;
static int fullname_maxlen;
static int fullname_len;

static void
escape_playlist (const char *str, int second_level)
{
  int is_playlist_str = (str == playlist);
  char *new_str;
  int new_len, len;

  if (!str)
    return;

  for (new_len = 0, len = 0; *str; str++)
    switch (*str)
    {
    case '\'':
      if (second_level)
        new_len+=6;
    case '\\':
      new_len++;
    default:
      new_len++;
      len++;
      break;
    }

  if (new_len == len && is_playlist_str)
    return;

  if (playlist_len < new_len + 1)
    {
      playlist_len = new_len + 1;
      playlist = (char *) realloc (playlist, playlist_len);
      if (is_playlist_str)
        str = &playlist[len];
    }
  new_str = &playlist[new_len];

  len++; /* for null character */
  while (len--)
    switch (*str)
    {
      case '\'':
        if (second_level)
	  { /* ' -> \'\\\'\' */
	    *new_str-- = '\''; *new_str-- = '\\'; /* \' */
	    *new_str-- = '\''; *new_str-- = '\\'; /* \' */
	    *new_str-- = '\\'; *new_str-- = '\\'; /* \\ */
	  }
      case '\\':
        *new_str-- = *str--;
        *new_str-- = '\\';
	break;
      default:
        *new_str-- = *str--;
	break;
    }
}

static int
is_playable (const struct dirent *dir)
{
  char **str, *ext;
  struct stat st;

  if (*dir->d_name == '.')
    return 0;
  strcpy (fullname+fullname_len, dir->d_name);
  stat (fullname, &st);
  if (S_ISDIR(st.st_mode) && strcmp (dir->d_name, "GEEXBOX"))
    return 1;
  ext = strrchr (dir->d_name, '.');
  if (ext++ == NULL)
    return 0;
  for (str=exts; *str; str++)
    if (!strcasecmp (ext, *str))
      return 1;
  return 0;
}

static int
build_playlist (char *dir, int fd)
{
  struct dirent **namelist;
  int i, n, len, dirlen, count=0;
  struct stat st;

  if (dir)
    {
      dirlen = strlen (dir);
      if (dirlen + 2 > fullname_maxlen)
        {
          fullname = (char *) realloc (fullname, dirlen + 2);
          fullname_maxlen = dirlen + 2;
        }
      strcpy (fullname, dir);
    }

  strcat (fullname, "/");
  fullname_len = dirlen = strlen (fullname);
  n = scandir (fullname, &namelist, is_playable, alphasort);
  if (n < 0)
    return 0;
  for (i=0; i<n; i++)
    {
      len = dirlen + _D_EXACT_NAMLEN(namelist[i]) + 1;
      if (len + 1 > fullname_maxlen)
        {
          fullname = (char *) realloc (fullname, len + 1);
          fullname_maxlen = len + 1;
        }
      strcpy (fullname+dirlen, namelist[i]->d_name);
      stat (fullname, &st);
      if (S_ISDIR(st.st_mode))
        count += build_playlist (NULL, fd);
      else
        {
          if (fd == -1)
            {
              if (playlist == NULL)
                {
                  playlist_len = strlen (fullname) + 1;
                  playlist = (char *) malloc (playlist_len);
                }
              else if (playlist_len < strlen (fullname) + 1)
                {
                  playlist_len = strlen (fullname) + 1;
                  playlist = (char *) realloc (playlist, playlist_len);
                }
              strcpy (playlist, fullname);
            }
          else
            {
              write (fd, fullname, len-1);
              write (fd, "\n", 1);
            }
          count++;
        }
      free (namelist[i]);
    }
  free (namelist);
  return count;
}

static int
open_device(const char *dev)
{
  int fd = open(dev, O_RDONLY | O_NONBLOCK);
  if (fd >= 0)
    ioctl(fd, CDROM_LOCKDOOR, 0);
  return fd;
}

static int mntlock_fd = -1;

static int
mntlock(void)
{
  if (mntlock_fd != -1 ||
     (mntlock_fd = open("/tmp/mntlock", O_RDONLY | O_CREAT)) != -1)
    {
      if (!flock(mntlock_fd, LOCK_EX))
        return 1;

      close(mntlock_fd); /* locking failed, try to reopen file next time */
      mntlock_fd = -1;
    }

  return 0;
}

static void
mntunlock(void)
{
  if (flock(mntlock_fd, LOCK_UN))
    { /* freeing lock failed, close fd instand */
      close(mntlock_fd);
      mntlock_fd = -1;
    }
}

static int
is_cdrom_mounted(cd_drive drive)
{
  char buf[PATH_MAX];
  size_t devlen;
  FILE *f;

  devlen = strlen(drive->dev);

  f = fopen("/proc/mounts", "r");
  if (f)
    {
      while (!feof(f))
        {
          if (fgets(buf, sizeof(buf), f) &&
              !strncmp(buf, drive->dev, devlen) &&
              buf[devlen] == ' ')
            {
              fclose(f);
              return 1;
            }
        }

      fclose(f);
    }

  return 0;
}

static void
umount_cdrom(cd_drive drive)
{
  if (umount2(drive->mnt, MNT_DETACH) < 0 && errno == EBUSY)
    {
      do
      {
        usleep(1000000);
      } while (umount(drive->mnt) < 0 && errno == EBUSY);
    }
}

static char **fstype_list = NULL;

static void
load_proc_filesystems(void)
{
  char buf[255], *fstype;
  size_t n = 0, len;
  FILE *f;

  f = fopen("/proc/filesystems", "r");
  if (f)
    {
      while (!feof(f))
        {
          if (fgets(buf, sizeof(buf), f) && buf[0] == '\t')
            {
              len = strlen(buf);
              if (buf[len-1] == '\n')
                buf[--len] = '\0';
              fstype = (char*) malloc(len);
              strcpy(fstype, &buf[1]); // skip the first tab char

              fstype_list = (char **) realloc(fstype_list, ++n * sizeof(*fstype_list));
              fstype_list[n-1] = fstype;
            }
        }
      fclose(f);

      if (n)
        {
          fstype_list = (char **)realloc(fstype_list, ++n * sizeof(*fstype_list));
          fstype_list[n-1] = NULL;
        }
    }
}

static int
mount_cdrom(cd_drive drive)
{
  char **fstype;

  if (drive->fd != -1) {
    close(drive->fd);
    drive->fd = -1;
  }

  for (fstype = fstype_list; *fstype; fstype++)
    {
      if (!mount(drive->dev, drive->mnt, *fstype,
                 MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC,
                 NULL))
        return 1;
    }

  return 0;
}

static cd_drive *
load_mnts(int init)
{
  cd_drive drive, *drives;
  char buf[PATH_MAX], *tmp;
  int n;
  FILE *f;

  if (!mntlock())
    return NULL;

  drives = NULL;
  n = 0;

  f = fopen("/etc/mnts", "r");
  if (f)
    {
      while (!feof(f))
        {
          if (!fgets(buf, sizeof(buf), f))
            continue;
          if ((tmp = strchr(buf, '\n')))
            *tmp = '\0';
          if (!(tmp = strchr(buf, '\t')))
            continue;
          *tmp++ = '\0';
          if (strncmp(buf, "/dev/cdrom", sizeof("/dev/cdrom")-1))
            continue;
          drive = (cd_drive) malloc(sizeof(*drive));
          drive->dev = malloc (strlen (buf) + 1);
          strcpy (drive->dev, buf);
          drive->mnt = malloc (strlen (tmp) + 1);
          strcpy(drive->mnt, tmp);
          drive->fd = -1;

          if (init)
            {
              if (is_cdrom_mounted(drive))
                umount_cdrom(drive);
              drive->status = CDS_NO_DISC;
            }
          else
            {
              drive->status = is_cdrom_mounted(drive) ? CDS_DISC_OK : CDS_NO_DISC;
            }

          drives = (cd_drive *)realloc(drives, ++n * sizeof(*drives));
          drives[n-1] = drive;
        }
      fclose(f);

      if (n)
        {
          drives = (cd_drive *)realloc(drives, ++n * sizeof(*drives));
          drives[n-1] = NULL;
        }
    }

  mntunlock();

  return drives;
}

int
main (int argc, char **argv)
{
  cd_drive drive, *drives, *ptr;
  char **file_exts, **playlist_exts, **img_exts;
  char *filename;
  char *play_dvd_cmd;
  struct stat st;
  int n, status, speed=0, off_silent=0;
  time_t last_mtime = 0;
  char system_buf[BUF_LEN];

  filename = (char *) malloc (PATH_MAX + 10);

  fullname_maxlen = 1024;
  fullname = (char *) malloc (fullname_maxlen);

  if (!(argc&1))
    speed = atoi (argv[argc-1]);

  load_proc_filesystems ();
  file_exts = get_extensions ("/etc/file_ext");
  playlist_exts = get_extensions ("/etc/list_ext");
  img_exts = get_extensions ("/etc/img_ext");
  if (!fstype_list || !file_exts || !playlist_exts || !img_exts)
    return 2;

  drives = load_mnts(1);
  if (!drives)
    return 3;

  while (1)
    {
      usleep(1000000);

      if (!stat ("/var/use_dxr3", &st) && S_ISREG (st.st_mode))
        play_dvd_cmd = "set_option dvd-device %s \nquit 167\n";
      else
        {
          if (!stat ("/var/dvdnav", &st) && S_ISREG (st.st_mode))
            play_dvd_cmd = "menu hide\nloadfile dvdnav://\n";
          else
            play_dvd_cmd = "menu hide\nloadfile dvd://\n";
        }

      if (!stat("/var/autoplay", &st))
        off_silent=0;
      else
        off_silent=1;

      if (!stat("/etc/mnts", &st) && st.st_mtime != last_mtime)
        {
          if (drives)
            {
              for (ptr=drives, drive=*ptr; drive; ptr++, drive=*ptr)
                {
                  if (drive->fd >= 0)
                    close(drive->fd);
                  free(drive->dev);
                  free(drive->mnt);
                  free(drive);
                }
              free(drives);
            }

          drives = load_mnts(0);
          if (!drives)
            continue;
          last_mtime = st.st_mtime;
        }

      for (ptr=drives, drive=*ptr; drive; ptr++, drive=*ptr)
        {
          if (drive->status == CDS_NO_INFO)
            continue;

          if (drive->fd < 0 && (drive->fd = open_device(drive->dev)) < 0)
            continue;

          status = ioctl (drive->fd, CDROM_DRIVE_STATUS, CDSL_CURRENT);
          switch (status)
            {
            case CDS_DISC_OK:
              if (drive->status != status)
                {
                  /* a media as been inserted */
                  drive->status = status;
                  if (speed > 1 && speed < 100)
                    ioctl (drive->fd, CDROM_SELECT_SPEED, speed);
                  status = ioctl (drive->fd, CDROM_DISC_STATUS, CDSL_CURRENT);
                  switch (status)
                    {
                      case CDS_AUDIO:
                        /* it's an audio CD */
                        sprintf(system_buf, "mp_set_option cdrom-device %s", drive->dev);
                        system(system_buf);
                        printf("set_option cdrom-device %s\n", drive->dev);
                        if (!off_silent)
                          printf("menu hide\nloadfile cdda://\n");
                        break;
                      case CDS_NO_INFO: /* no information, but try to mount and detect */
                      case CDS_DATA_1:
                      case CDS_DATA_2:
                        /* it's a data CD */
                        if (!mount_cdrom(drive))
                          break;

                        sprintf (filename, "%s/video_ts", drive->mnt);
                        if (!stat (filename, &st) && S_ISDIR (st.st_mode))
                          {
                            sprintf(system_buf, "mp_set_option dvd-device %s", drive->dev);
                            system(system_buf);
                            printf("set_option dvd-device %s\n", drive->dev);
                            if (!off_silent)
                              printf(play_dvd_cmd, drive->dev);
                            break;
                          }
                        sprintf (filename, "%s/VIDEO_TS", drive->mnt);
                        if (!stat (filename, &st) && S_ISDIR (st.st_mode))
                          {
                            sprintf(system_buf, "mp_set_option dvd-device %s", drive->dev);
                            system(system_buf);
                            printf("set_option dvd-device %s\n", drive->dev);
                            if (!off_silent)
                              printf(play_dvd_cmd, drive->dev);
                            break;
                          }
                        sprintf (filename, "%s/vcd", drive->mnt);
                        if (!stat (filename, &st) && S_ISDIR (st.st_mode))
                          {
                            sprintf(system_buf, "mp_set_option cdrom-device %s", drive->dev);
                            system(system_buf);
                            printf("set_option cdrom-device %s\n", drive->dev);
                            if (!off_silent)
                              printf("menu hide\nloadfile vcd://\n");
                            break;
                          }
                        sprintf (filename, "%s/svcd", drive->mnt);
                        if (!stat (filename, &st) && S_ISDIR (st.st_mode))
                          {
                            sprintf(system_buf, "mp_set_option cdrom-device %s", drive->dev);
                            system(system_buf);
                            printf("set_option cdrom-device %s\n", drive->dev);
                            if (!off_silent)
                              printf("menu hide\nloadfile vcd://\n");
                            break;
                          }
                        exts=playlist_exts;
                        if (build_playlist (drive->mnt, -1) == 1)
                          {
                            escape_playlist(playlist, 0);
                            if (!off_silent)
                              printf ("menu hide\nloadlist '%s'\n", playlist);
                          }
                        else
                          {
                            char *playlist = "/tmp/playlist.pls";
                            int fd = open (playlist, O_CREAT|O_TRUNC|O_WRONLY,
                                           S_IRUSR|S_IWUSR);
                            if (fd < 0)
                              return 1;
                            exts=file_exts;
                            n = build_playlist (drive->mnt, fd);
                            if (n > 0)
                              if (!off_silent)
                                printf ("menu hide\nloadlist %s\n", playlist);
                            close (fd);
                            if (n <= 0)
                              {
                                exts=img_exts;
                                if (build_playlist (drive->mnt, -1) > 0)
                                  {
                                    escape_playlist(drive->mnt, 1);
                                    if (!off_silent)
                                      printf ("run 'view_img -r \\'%s\\''\n",
                                            playlist);
                                  }
                                else
                                  {
                                    exts=xcd_exts;
                                    if (build_playlist (drive->mnt, -1) >= 1)
                                    {
                                      sprintf(system_buf, "mp_set_option cdrom-device %s", drive->dev);
                                      system(system_buf);
                                      printf ("set_option cdrom-device %s\n", drive->dev);
                                      if (!off_silent)
                                        printf ("menu hide\nloadfile vcd://\n");
                                    }
                                  }
                              }
                          }
                        break;
                      case CDS_MIXED:
                        sprintf(system_buf, "mp_set_option cdrom-device %s", drive->dev);
                        system(system_buf);
                        printf("set_option cdrom-device %s\n", drive->dev);
                        if (!off_silent)
                          printf("menu hide\nloadfile cdda://\n");
                      case CDS_XA_2_1:
                      case CDS_XA_2_2:
                        /* it's a special CD */
                        mount_cdrom(drive);
                        break;
                      case CDS_NO_DISC: /* no disc, refresh the drive. */
                      default:
                        drive->status = CDS_NO_DISC;
                        close(drive->fd);
                        drive->fd = -1;
                        break;
                    }
                  fflush (stdout);
                }
              break;

            case -1: /* close the fd on ioctl failure, should never happen */
            case CDS_NO_INFO: /* Drive do not support status request :-( */
              close(drive->fd);
              drive->fd = -1;
            default:
              /* the media as been ejected */
              if (drive->status != status)
                {
                  drive->status = status;
                  umount_cdrom(drive);
                }
            }
        }
    }

  free_extensions (file_exts);
  free_extensions (playlist_exts);
  return 0;
}
