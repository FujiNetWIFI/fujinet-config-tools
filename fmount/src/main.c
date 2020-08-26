/**
 * FujiNet Tools for CLI
 *
 * fmount - Mount Disk Image
 *
 * usage:
 *  feject <ds#> <hs#> <R|W> <FNAME>
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

unsigned char buf[128];

union
{
  char host[8][32];
  unsigned char rawData[256];
} hostSlots;

union
{
  struct
  {
    unsigned char hostSlot;
    unsigned char mode;
    char file[36];
  } slot[8];
  unsigned char rawData[304];
} deviceSlots;

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
      exit(OS.dcb.dstats);
    }
}

/**
 * Read Device Slots
 */
void disk_read(void)
{
  // Read Drive Tables
  OS.dcb.ddevic=0x70;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd=0xF2;
  OS.dcb.dstats=0x40;
  OS.dcb.dbuf=&deviceSlots.rawData;
  OS.dcb.dtimlo=0x0f;
  OS.dcb.dbyt=sizeof(deviceSlots.rawData);
  OS.dcb.daux=0;
  siov();

  if (OS.dcb.dstats!=1)
    {
      err_sio();
      exit(OS.dcb.dstats);
    }
}

/**
 * Write device slots
 */
void disk_write(void)
{
  OS.dcb.ddevic=0x70;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd=0xF1;
  OS.dcb.dstats=0x80;
  OS.dcb.dbuf=&deviceSlots.rawData;
  OS.dcb.dtimlo=0x0f;
  OS.dcb.dbyt=sizeof(deviceSlots.rawData);
  OS.dcb.daux=0;
  siov();

  if (OS.dcb.dstats!=1)
    {
      err_sio();
      exit(OS.dcb.dstats);
    }
}

/**
 * Set filename for device slot
 */
void set_filename(char* filename, unsigned char slot)
{
  OS.dcb.ddevic=0x70;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd=0xE2;
  OS.dcb.dstats=0x80;
  OS.dcb.dbuf=filename;
  OS.dcb.dtimlo=0x0F;
  OS.dcb.dbyt=256;
  OS.dcb.daux1=slot;
  OS.dcb.daux2=0;
  siov();

  if (OS.dcb.dstats!=1)
    {
      err_sio();
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
	  exit(OS.dcb.dstats);
	}
    }
}

/**
 * Mount device slot
 */
void disk_mount(unsigned char c, unsigned char o)
{
  OS.dcb.ddevic=0x70;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd=0xF8;
  OS.dcb.dstats=0x00;
  OS.dcb.dbuf=NULL;
  OS.dcb.dtimlo=0x01;
  OS.dcb.dbyt=0;
  OS.dcb.daux1=c;
  OS.dcb.daux2=o;
  siov();
  if (OS.dcb.dstats!=1)
    {
      err_sio();
      exit(OS.dcb.dstats);
    }
}

/**
 * show options
 */
void opts(char* argv[])
{
  print(argv[0]);
  print(" <DS#> <HS#> <R|W> <FNAME>\x9b\x9b");
  print("<DS#>   - device slot (1-8)\x9b");
  print("<HS#>   - host slot (1-8)\x9b");
  print("<R|W>   - Read / Write\x9b");
  print("<FNAME> - filename\x9b");
}

/**
 * main
 */
int main(int argc, char* argv[])
{
  unsigned char ds,hs,dsa,hsa,o;
  char* tokens[4]; // Token cursors.
  unsigned char i; // Loop indices
  
  OS.lmargn=2;
  
  if (_is_cmdline_dos())
    {
      if (argc<2)
	{
	  opts(argv);
	  return(1);
	}
      else
	{
	  unsigned char i;
	  for (i=1;i<argc;i++)
	    {
	      strcat(buf,argv[i]);
	      if (i<argc-1)
		strcat(buf," ");
	    }
	}
    }
  else
    {
      // DOS 2.0
      print("MOUNT--DRIVE, HOST SLOT, R/W, FILE?\x9b");
      get_line(buf,sizeof(buf));
    }

  // Tokenize string.
  tokens[0]=strtok(buf,",");
  tokens[1]=strtok(NULL,",");
  tokens[2]=strtok(NULL,",");
  tokens[3]=strtok(NULL,",");

  if (tokens[0]==NULL ||
      tokens[1]==NULL ||
      tokens[2]==NULL ||
      tokens[3]==NULL)
    {
      print("ALL ARGUMENTS REQUIRED\x9b");
      return(1);
    }

  // Trim whitespace.
  for (i=0;i<4;i++)
    while (*tokens[i]==' ')
      tokens[i]++;

  // Catch D: and Dx:, turn into device slot #
  if (tokens[0][0]=='D')
    {
      if (tokens[0][1]==':')
	dsa='1'; // D: is slot 1
      else
	dsa=tokens[0][1];
    }
  else
    dsa=tokens[0][0];

  ds=dsa-'0';
  
  if (ds<1 || ds>8)
    {
      print("INVALID DRIVE SLOT NUMBER\x9b");
      return(1);
    }

  hsa=*tokens[1];
  hs=hsa-'0';
  
  if (hs<1 || hs>8)
    {
      print("INVALID HOST SLOT NUMBER\x9b");
      return(1);
    }

  // Set mode appropriately.
  switch (*tokens[2])
    {
    case 'w':
    case 'W':
      o=2;
      break;
    case 'r':
    case 'R':
      o=1;
      break;
    default:
      print("INVALID MODE\x9b");
      exit(1);
    }
  
  hs-=1;
  ds-=1;

  // Read in existing data from FujiNet
  host_read();
  disk_read();

  // Mount desired host
  host_mount(hs);

  // Set desired slot filename/mode
  deviceSlots.slot[ds].mode=o;
  deviceSlots.slot[ds].hostSlot=hs;

  // Write out disk slot
  disk_write();

  // Write out the filename
  set_filename(tokens[3],ds);

  // Mount the disk.
  disk_mount(ds,o);

  print("D");
  printc(&dsa);
  print(": ");
  print("(");
  printc(&hsa);
  print(") ");
  print("(");
  print((o==0x02 ? "W" : "R"));
  print(") ");
  print(tokens[3]);
  print("\x9b\x9b");

  return(0);
}
