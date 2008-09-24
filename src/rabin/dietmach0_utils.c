/* @LICENSE_START@ */
/*
 * Copyright (C) 2008 Alfredo Pesoli <revenge[AT]0xcafebabe.it>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of its
 *    contributors may be used to endorse or promote products derived from 
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/* @LICENSE_END@ */

#include <libc.h>
#include <libgen.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sqlite3.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <mach-o/loader.h>
#include <mach-o/fat.h>
#include <mach-o/swap.h>

#include "dietmach0_errors.h"
#include "dietmach0.h"

/*
 * Gloabal Variables
 */
int fd, fdout;
struct stat sb;
static char *array[256];


void
*dm_allocate (size_t nbytes)
{
  void *pointer;

  if ( !(pointer = malloc(nbytes)) )
    dm_fatal(EMAPFD);

  memset(pointer, '\0', nbytes);

  return pointer;
}

void
dm_fatal (const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);
  (void) vfprintf(stderr, fmt, ap);
  va_end(ap);

  /*
   * Closing all open files
   */
}

void
dm_map_file (char *filename, int fd)
{
  kern_return_t result;

  if ( stat(filename, &sb) == -1 ) {
    close(fd);
    dm_fatal("cannot stat()\n");
    exit(-1);
  }

  /*
   *  Print out information about mach-o file
   */
  printf("\nFilename: %s\n", filename);
  filesize = sb.st_size;
  printf("File size: %d\n\n", filesize);

  if ( (result = map_fd((int)fd, (vm_offset_t)0, (vm_offset_t *)&fileaddr,
                        (boolean_t)TRUE, 
                        (vm_size_t)filesize)) != KERN_SUCCESS )
    {
      dm_fatal("Cannot map file %s\n", filename);
      close(fd);
      exit(-1);
    }

  startaddr = fileaddr;
}

char
*dm_split (char *strmine, const char delim, int size)
{
  int i;
  char *rt = NULL;

  if (strmine[0] == '\0')
    return NULL;

  for (i=0; i < size; i++)
    {
      if (strmine[i] == '\0')
        {
          rt = strmine;
          break;
        }
      else if (strmine[i] == delim)
        {
          strmine[i] = '\0';
          rt = strmine;
          break;
        } 
    }

  return rt;
}

void
Debug (const char *fmt, ...)
{
  va_list ap;

  fprintf(stderr, "DEBUG:\t");
  va_start(ap, fmt);
  (void) vfprintf(stderr, fmt, ap);
  va_end(ap);
}