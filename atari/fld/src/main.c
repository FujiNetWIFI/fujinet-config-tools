/**
 * FujiNet Tools for CLI
 *
 * fld - list disk slots
 *
 * usage:
 *  fld
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
  struct
  {
    unsigned char hostSlot;
    unsigned char mode;
    char file[36];
  } slot[8];
  unsigned char rawData[304];
} deviceSlots;

/**
 * Read Device Slots
 */
unsigned char disk_read(void)
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
    }
  return OS.dcb.dstats;
}

/**
 * main
 */
int main(void)
{
  unsigned char i=0;
  unsigned char err=0;

  OS.lmargn=2;
  
  // Read in host and device slots from FujiNet
  err=disk_read();

  if (err==1)
    {
      print("\x9b");

      for (i=0;i<8;i++)
        {
          unsigned char n=i+0x31;
          unsigned char hs=deviceSlots.slot[i].hostSlot+0x31;
          unsigned char m=(deviceSlots.slot[i].mode==0x02 ? 'W' : 'R');

          if (deviceSlots.slot[i].hostSlot!=0xFF)
            {
              print("D");
              printc(&n);
              print(": ");
              print("(");
              printc(&hs);
              print(") ");
              print("(");
              printc(&m);
              print(") ");
              print(deviceSlots.slot[i].file);
              print("\x9b");
            }
          else
            {
              print("D");
              printc(&n);
              print(": ");
              print("Empty\x9b");
            }
        }
    }

  print("\x9b");

  if (!_is_cmdline_dos())
    {
      print("PRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
      get_line(buf,sizeof(buf));
    }
  
  return err==1 ? 0 : err;
}
