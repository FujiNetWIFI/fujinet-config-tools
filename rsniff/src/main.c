/**
 * FujiNet Tools for CLI
 *
 * rsniff - enable/disable modem sniffer
 *
 * usage:
 *  rsniff <0|1>
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
#include "sio.h"
#include "conio.h"
#include "err.h"

unsigned char buf[40];
unsigned char enable;

/**
 * SIO command to set R: sniffer enable
 */
unsigned char dump_enable(unsigned short enable)
{
  OS.dcb.ddevic=0x50;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd='D';
  OS.dcb.dstats=0x00;
  OS.dcb.dbuf=NULL;
  OS.dcb.dtimlo=0x01;
  OS.dcb.dbyt=0;
  OS.dcb.daux1=enable;

  siov();

  if (OS.dcb.dstats!=1)
    err_sio();

  return OS.dcb.dstats;
}

/**
 * show options
 */
void opts(char* argv[])
{
  print(argv[0]);
  print(" <0|1>\x9b\x9b");
  print("<0|1> 0=disable 1=enable\x9b");
}

/**
 * main
 */
int main(int argc, char* argv[])
{
  unsigned char err=0;
  
  OS.lmargn=2;
  
  if (_is_cmdline_dos())
    {
      if (argc<2)
	{
	  opts(argv);
	  return(1);
	}
      enable=atoi(argv[1]);
    }
  else
    {
      // DOS 2.0/MYDOS
      print("\x9b");

      print("DUMP--0=DISABLE, 1=ENABLE\x9b");
      get_line(buf,sizeof(buf));
      enable=atoi(buf);
    }
  
  err=dump_enable(enable);
    
  return(err);
}
