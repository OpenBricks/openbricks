/*
 * minimal Snowball bootable image builder
 *
 * Copyright (C) 2011 Davide Cavalca
 *
 * This file is part of OpenBricks.
 *
 * OpenBricks is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * OpenBricks is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with OpenBricks; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* strncpy */
#include <string.h>

/* stat */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* Boot ROM looks for a boot table of contents (TOC) at 0x20000 */
const int SNOWBALL_LOADER_START = 0x20000;
const int TOC_SIZE = 0x200; /* 512 */

/*
  TOC format is defined in 
  http://igloocommunity.org/support/index.php/ConfigPartitionOverview
*/
#define NAME_LEN 12

struct toc_entry_s {
  uint32_t offset; /* Location of the module relative to the start of the TOC */
  uint32_t size; /* Size of the module. */
  uint32_t flags; /* reserved, set to 0. */
  uint32_t align; /* reserved, set to 0. */
  uint32_t load_address; /* reserved, set to 0 */
  char name[NAME_LEN]; /* The name of the module, null-terminated */
};

const int BUFSIZE = 1024;
const int LINE_LEN = 80;
const int WORD_LEN = 30;

int main(int argc, char** argv) {  
  struct toc_entry_s *toc = calloc(6, sizeof(struct toc_entry_s));
  char **files = calloc(6, sizeof(char *));
  int i = 0;

  FILE *fp;
  char line[LINE_LEN];

  if (argc != 3) {
    printf("Usage: mksnowball <cfg> <out>\n");
    return EXIT_FAILURE;
  }

  fp = fopen(argv[1], "r");
  while (fgets(line, LINE_LEN, fp) != NULL) {
    uint32_t offset;
    uint32_t align;
    uint32_t load_address;
    char *section = malloc(WORD_LEN);
    char *file_name = malloc(WORD_LEN);
    struct stat st;

    if(line[0]=='#') continue;
    sscanf(line, "%s %s %u %x %u", section, file_name, &align, &offset, &load_address);

    toc[i].offset = offset ? offset : (TOC_SIZE + (i>0 ? toc[i-1].size : 0));
    toc[i].align = align;
    toc[i].load_address = load_address;
    strncpy(toc[i].name, section, sizeof(toc[i].name));
    toc[i].flags = 0;
    stat(file_name, &st);
    toc[i].size = st.st_size;

    files[i] = strdup(file_name);

    i++;
    free(section);
    free(file_name);
  }
  fclose(fp);

  fp = fopen(argv[2], "w");

  /* write toc */
  fseek(fp, SNOWBALL_LOADER_START, SEEK_SET);
  /* fwrite(toc, sizeof(struct toc_entry_s), 6, fp);
     won't work as the compiler adds padding to the structure */
  for(i=0; i<6; i++) {
    fwrite(&toc[i].offset, sizeof(uint32_t), 1, fp);
    fwrite(&toc[i].size, sizeof(uint32_t), 1, fp);
    fwrite(&toc[i].flags, sizeof(uint32_t), 1, fp);
    fwrite(&toc[i].align, sizeof(uint32_t), 1, fp);
    fwrite(&toc[i].load_address, sizeof(uint32_t), 1, fp);
    fwrite(toc[i].name, sizeof(char), NAME_LEN, fp);
  }

  /* write files */
  for(i=0; i<6; i++) {
    char buf[BUFSIZE];
    size_t len;
    FILE *in = fopen(files[i], "r");

    fseek(fp, SNOWBALL_LOADER_START + toc[i].offset, SEEK_SET);
    while ((len = fread(buf, 1, BUFSIZE, in)) > 0) {
      fwrite(buf, 1, len, fp);
    }
    fclose(in);
  }
  fclose(fp);

  return EXIT_SUCCESS;
}
