/* fixcodec.c v0.0.1 */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int
main (int argc, char **argv)
{
  char *match = "libc.so.6";
  char *buffer, *p, *end;
  struct stat st;
  int in, out;
  int i = 0;

  if (argc < 3)
    return 1;

  in = open (argv[1], O_RDONLY);
  if (in == -1)
    return 2;

  if (fstat(in, &st) == -1)
    return 3;

  buffer = malloc (st.st_size);
  if (buffer == NULL)
    return 4;

  if (read(in, buffer, st.st_size) != st.st_size)
    return 5;

  p = buffer;
  end = buffer + st.st_size;
  while (p < end)
    {
      if (*p != match[i++])
        i = 0;
      else if (i == 9)
        *p = '0';
      p++;
    }

  out = open (argv[2], O_CREAT|O_WRONLY, S_IRUSR|S_IWUSR | S_IRGRP | S_IROTH);
  if (out == -1)
    return 6;

  write (out, buffer, st.st_size);

  close (out);
  close (in);
  return 0;
}
