/**
 * FujiNet Tools for CLI
 *
 * finfo - show disk geometry for a disk slot.
 *
 * usage:
 *  finfo
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

char buf[40];

union
{
  struct
  {
    unsigned char num_tracks;
    unsigned char step_rate;
    unsigned char sptH;
    unsigned char sptL;
    unsigned char num_sides;
    unsigned char density;
    unsigned char sector_sizeH;
    unsigned char sector_sizeL;
    unsigned char drive_present;
    unsigned char res1;
    unsigned char res2;
    unsigned char res3;
  } block;
  unsigned char rawData[12];
} percomBlock;

/**
 * Read Device Slots
 */
unsigned char percom_read(unsigned char dunit)
{
  // Read Drive Tables
  OS.dcb.ddevic=0x31;
  OS.dcb.dunit=dunit;
  OS.dcb.dcomnd='N';
  OS.dcb.dstats=0x40;
  OS.dcb.dbuf=&percomBlock.rawData;
  OS.dcb.dtimlo=0x0f;
  OS.dcb.dbyt=sizeof(percomBlock.rawData);
  OS.dcb.daux=0;
  siov();

  if (OS.dcb.dstats!=1)
    {
      err_sio();
    }
  return OS.dcb.dstats;
}

/**
 * Show options
 */
void opts(void)
{
  print("\x9b" "FINFO <ds>\x9b\x9b  <ds> Device Slot\x9b\x9b");
}

/**
 * Print an 8 bit quantity
 */
void print_num8(unsigned char n)
{
  char tmp[4] = {0,0,0,0};

  utoa(n,tmp,10);

  print(tmp);
}

/**
 * Print a 16 bit quantity
 */
void print_num16(unsigned char nl, unsigned char nh)
{
  char tmp[6] = {0,0,0,0,0,0};

  utoa((nh*256+nl),tmp,10);

  print(tmp);
}

/**
 * Print verbose disk type
 */
void print_disk_type(void)
{
  print("Disk Type: ");
  
  if (percomBlock.block.num_tracks == 40 &&
      percomBlock.block.sptL == 18 &&
      percomBlock.block.sector_sizeL == 128)
    {
      print("90K SS/SD (ATARI 810)");
    }
  else if (percomBlock.block.num_tracks == 40 &&
	   percomBlock.block.sptL == 26 &&
	   percomBlock.block.sector_sizeL == 128)
    {
      print("140K SS/ED (ATARI 1050)");
    }
  else if (percomBlock.block.num_tracks == 40 &&
	   percomBlock.block.sptL == 18)
    {
      if (percomBlock.block.num_sides == 0)
	print("180K SS/DD");
      else
	print("360K DS/DD");
    }
  else if (percomBlock.block.num_tracks == 80 &&
	   percomBlock.block.sptL == 18)
    {
      if (percomBlock.block.num_sides == 0)
	print("360K SS/QD");
      else
	print("720K DS/QD");     
    }
  else if (percomBlock.block.num_tracks == 77)
    {
      if (percomBlock.block.sector_sizeL == 128)
	{
	  if (percomBlock.block.num_sides == 0)
	    print("256K 8 inch SS/SD");
	  else
	    print("512K 8 inch DS/SD");
	}
      else
	{
	  if (percomBlock.block.num_sides == 0)
	    print("512K 8 inch SS/DD");
	  else
	    print("1M 8 inch DS/DD");
	}
    }
  else if (percomBlock.block.density == 8)
    {
      print("1.44M 3 1/2\" DS/HD");
    }
  else if (percomBlock.block.num_tracks == 1)
    {
      long size;
      long hsize;
      char tmp[8];

      size = ((long)percomBlock.block.sptH*(long)256+(long)percomBlock.block.sptL);
      size *= ((long)percomBlock.block.sector_sizeH*(long)256+(long)percomBlock.block.sector_sizeL);
      
      print("Large disk ");

      if (size >= 1048576)
	{
	  hsize = size / 1048576;
	  hsize += 1;
	  ltoa(hsize,tmp,10);
	  print(tmp);
	  print("M");
	}
      else
	{
	  hsize = size / 1024;
	  hsize += 1;
	  ltoa(hsize,tmp,10);
	  print(tmp);
	  print("K");
	}
    }

  print("\x9b\x9b");
}

/**
 * Show geometry info for slot
 */
void finfo(void)
{

  print("\x9bNumber of Tracks: ");
  print_num8(percomBlock.block.num_tracks);
  print("\x9b");

  print("Sectors per Track: ");
  print_num16(percomBlock.block.sptL,percomBlock.block.sptH);
  print("\x9b");

  print("Number of Sides: ");
  print_num8(percomBlock.block.num_sides+1);
  print("\x9b");

  print("Sector Size: ");
  print_num16(percomBlock.block.sector_sizeL,percomBlock.block.sector_sizeH);
  print("\x9b");

  print("\x9b");
  print_disk_type();
}

/**
 * main
 */
int main(int argc, char* argv[])
{
  unsigned char dunit=0;
  unsigned char err=0;

  OS.lmargn=2;

  if (_is_cmdline_dos())
    {
      if (argc<2)
	{
	  opts();
	  return(1);
	}
      dunit=argv[1][0]-'0';
    }
  else
    {
      print("DISK INFO--DEVICE SLOT NUMBER?\x9b");
      get_line(buf,sizeof(buf));

      if (buf[0]==0x9b)
	dunit=1;
      else
	dunit=buf[0]-'0';
    }

  err=percom_read(dunit);

  if (err==1)
    finfo();

  if (!_is_cmdline_dos())
    {
      print("\x9bPRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
      get_line(buf,sizeof(buf));
    }
  
  return err==1 ? 0 : err;
}
