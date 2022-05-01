/**
 * FujiNet Tools for CLI
 *
 * flh - list host slots
 *
 * usage:
 *  flh
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

unsigned char buf[40];

union
{
  char host[8][32];
  unsigned char rawData[256];
} hostSlots;

/**
 * Read host slots
 */
unsigned char host_read(void)
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
    }
  return OS.dcb.dstats;
}

/**
 * main
 */
int main(void)
{
  unsigned char i=0;

  OS.lmargn=2;
  
  // Read in host and device slots from FujiNet
  host_read();

  print("\x9b");
  
  for (i=0;i<8;i++)
    {
      unsigned char n=i+0x31;
      
      if (hostSlots.host[i][0]!=0x00)
	{
	  printc(&n);
	  print(": ");
	  print(hostSlots.host[i]);
	  print("\x9b");
	}
      else
	{
	  printc(&n);
	  print(": ");
	  print("Empty\x9b");
	}
    }
  
  print("\x9b");
  
  if (!_is_cmdline_dos())
    {
      print("PRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
      get_line(buf,sizeof(buf));
    }  
  
  return 0;
}
