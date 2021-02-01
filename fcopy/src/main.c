/**
 * Network Testing tools
 *
 * copy - Copy file from one slot to another.
 *
 * Author: Thomas Cherryhomes
 *  <thom.cherryhomes@gmail.com>
 *
 * Released under GPL 3.0
 * See COPYING for details.
 */

#include <atari.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sio.h"
#include "conio.h"
#include "err.h"

unsigned char source_slot, dest_slot;
unsigned char linebuf[128];
unsigned char source_buf[128];
unsigned char dest_buf[128];
unsigned char copy_buf[256];
unsigned char i=0;

void copy()
{
  OS.dcb.ddevic=0x70;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd=0xD8;
  OS.dcb.dstats=0x80;
  OS.dcb.dbuf=copy_buf;
  OS.dcb.dtimlo=0xfe;
  OS.dcb.dbyt=256;
  OS.dcb.daux1=source_slot;
  OS.dcb.daux2=dest_slot;
  siov();

  if (OS.dcb.dstats!=1)
    {
      err_sio();
      exit(OS.dcb.dstats);
    }
}

bool parse_cmdline(char* argv[])
{
  char *p;
  unsigned char i=1;
  
  while (argv[i]!=NULL)
    {
      strcat(linebuf, argv[i]);
      if (argv[i+1] != NULL)
	strcat(linebuf, " ");
    }

  p = strtok(linebuf, ",");
  
  if (p == NULL)
    return false;
  else
    source_slot = atoi(p);
  
  p=strtok(NULL,",");

  if (p == NULL)
    return false;
  else
    strcpy(source_buf,p);
  
  p=strtok(NULL,",");

  if (p == NULL)
    return false;
  else
    dest_slot = atoi(p);

  p=strtok(NULL,",");

  if (p == NULL)
    return false;
  else
    strcpy(dest_buf,p);

  return true;
}

int main(int argc, char* argv[])
{
  char tmp[4];
  
  OS.lmargn=2;

  if (_is_cmdline_dos())
    {
      if (argc<2)
	{
	  print("fcopy <src slot>,<path>,<dst slot>,<path>\x9b");
	  return 1;
	}
      else
	{
	  if (parse_cmdline(argv) == false)
	    return 1;
	}
    }
  else
    {
      // TODO: Add validation.
      print("FUJI COPY--SOURCE HOST SLOT?\x9b");
      get_line(tmp,4);
      source_slot = atoi(tmp);
      print("\x9bSOURCE PATH?\x9b");
      get_line(source_buf,128);
      print("\x9b" "DEST HOST SLOT?\x9b");
      get_line(tmp,4);
      dest_slot = atoi(tmp);
      print("\x9b" "DEST PATH?\x9b");
      get_line(dest_buf,128);
    }

  strcat(copy_buf,source_buf);
  strcat(copy_buf,"|");
  strcat(copy_buf,dest_buf);

  print("Copying...");
  
  copy();

  print("\x9b");
  
  return 0;
}
