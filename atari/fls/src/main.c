/**
 * FujiNet Tools for CLI
 *
 * feject - Eject disk in device slot
 *
 * usage:
 *  feject <ds#>
 *
 * Author: Thomas Cherryhomes
 *  <thom.cherryhomes@gmail.com>
 *
 * Released under GPL, see COPYING
 * for details
 */

#include <atari.h>
#include <string.h>
#include <stdlib.h>
#include <peekpoke.h>
#include "sio.h"
#include "conio.h"
#include "err.h"

unsigned char buf[256];

union
{
  char host[8][32];
  unsigned char rawData[256];
} hostSlots;

/**
 * Wait for keypress
 */
void pause(void)
{
  if (!_is_cmdline_dos())
    {
      print("\x9bPRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
      get_line(buf,sizeof(buf));
    }
}

/**
 * Read host slots
 */
void host_read(void)
{
  // Query for host slots
  OS.dcb.ddevic=0x70;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd=0xF4; // Get host slots
  OS.dcb.dstats=0x40;
  OS.dcb.dbuf=&hostSlots.rawData;
  OS.dcb.dtimlo=0x0f;
  OS.dcb.dbyt=256;
  OS.dcb.daux=0;
  siov();

  if (OS.dcb.dstats!=1)
    {
      err_sio();
      pause();
      exit(OS.dcb.dstats);
    }
}

/**
 * Mount a host slot
 */
void host_mount(unsigned char c)
{
  if (hostSlots.host[c][0]!=0x00)
    {
      OS.dcb.ddevic=0x70;
      OS.dcb.dunit=1;
      OS.dcb.dcomnd=0xF9;
      OS.dcb.dstats=0x00;
      OS.dcb.dbuf=NULL;
      OS.dcb.dtimlo=0x01;
      OS.dcb.dbyt=0;
      OS.dcb.daux=c;
      siov();
      if (OS.dcb.dstats!=1)
	{
	  err_sio();
	  pause();
	  exit(OS.dcb.dstats);
	}
    }
}

/**
 * Open TNFS directory on host slot
 */
void directory_open(unsigned char hs, char* b)
{
  // Open TNFS directory
  OS.dcb.ddevic=0x70;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd=0xF7;
  OS.dcb.dstats=0x80;
  OS.dcb.dbuf=b;
  OS.dcb.dtimlo=0x0F;
  OS.dcb.dbyt=256;
  OS.dcb.daux=hs;
  siov();

  if (OS.dcb.dstats!=1)
    {
      err_sio();
      pause();
      exit(OS.dcb.dstats);
    }
}

/**
 * Read TNFS directory entry in host slot
 * and of specific length
 */
void directory_read(unsigned char hs, unsigned char len)
{
  OS.dcb.dcomnd=0xF6;
  OS.dcb.dstats=0x40;
  OS.dcb.dbuf=&buf;
  OS.dcb.dbyt=len;
  OS.dcb.daux1=len;
  OS.dcb.daux2=hs;
  siov();
  
  if (OS.dcb.dstats!=1)
    {
      err_sio();
      pause();
      exit(OS.dcb.dstats);
    }
}

/**
 * Close TNFS Directory
 */
void directory_close(unsigned char hs)
{
  // Close directory
  OS.dcb.dcomnd=0xF5;
  OS.dcb.dstats=0x00;
  OS.dcb.dbuf=NULL;
  OS.dcb.dtimlo=0x01;
  OS.dcb.dbyt=0;
  OS.dcb.daux=hs;
  siov();

  if (OS.dcb.dstats!=1)
    {
      err_sio();
      pause();
      exit(OS.dcb.dstats);
    }
}

/**
 * Set current directory cursor
 */
void set_directory_position(unsigned short pos)
{
  OS.dcb.dcomnd = 0xE4;
  OS.dcb.dstats = 0x00;
  OS.dcb.dbuf = NULL;
  OS.dcb.dbyt = 0;
  OS.dcb.daux = pos;
  siov();
}

/**
 * show options
 */
void opts(char* argv[])
{
  print(argv[0]);
  print(" <hs#>,[path]:[filter]\x9b\x9b");
  print("<hs#> - host slot (1-8)\x9b");
  print("[path] - Path or '/' if not specified\x9b");
  print("[filter] - Optional wildcard.");
}

/**
 * main
 */
int main(int argc, char* argv[])
{
  unsigned char slot=0;
  char* comma_pos=NULL;
  char* path_pos=NULL;
  char* filter_pos=NULL;
  unsigned char i;
  unsigned short dir_pos = 0;
 
  memset(buf,0,sizeof(buf));
  
  OS.lmargn=2;

  if (_is_cmdline_dos())
    {
      if (argc<2)
	{
	  opts(argv);
	  return 1;
	}
      else
	{
	  for (i=1;i<argc;i++)
	    {
	      strcat(buf,argv[i]);
	      if (i<argc-1)
		strcat(buf, " ");
	    }
	}
    }
  else // DOS 2.0
    {
      print("HOST DIRECTORY--HOST SLOT, PATH?\x9b");
      get_line(buf,128);
    }

  slot = buf[0];

  if ((slot<0x31) || (slot>0x38))
    {
      print("INVALID SLOT NUMBER\x9b"); 
      pause();
      return 1;
    }

  slot-=0x31;
  
  comma_pos=strchr(buf,',');

  if (comma_pos == NULL)
    {
      print("NO COMMA.\x9b");
      pause();
      return 1;
    }

  path_pos=comma_pos+1;

  // Seperate filter from path
  filter_pos = strrchr(buf,'/');
  *filter_pos = 0x00;
  filter_pos++;
  
  host_read();
  host_mount(slot);
  directory_open(slot,path_pos);
  
  while (buf[0]!=0x7F)
    {      
      memset(buf,0,sizeof(buf));
      buf[0]=0x7f;
      
      directory_read(slot,36); // show 36 chars max
      
      if (strlen(buf)==35)
	{
	  set_directory_position(dir_pos);
	  directory_read(slot,128);
	}
      
      if (buf[0]=='.')
	continue;
      else if (buf[0]==0x7F)
	break;
      else
	{
	  print(buf);
	  print("\x9b");
	}
      dir_pos++;
    }

  directory_close(slot);
  pause();
}
