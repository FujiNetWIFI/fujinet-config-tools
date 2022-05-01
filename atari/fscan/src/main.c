/**
 * FujiNet Tools for CLI
 *
 * fscan - scan and return list of wireless networks
 *
 * usage:
 *  fscan
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

#define MAX_SSID_LEN 33 /* SSID + NULL */

char buf[40];

union 
{
  struct
  {
    char ssid[33];
    signed char rssi;
  };
  unsigned char rawData[34];
} ssidInfo;

unsigned char num_networks[4];

/**
 * Return number of networks
 */
unsigned char scan(void)
{
  OS.dcb.ddevic=0x70;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd=0xFD; // do scan
  OS.dcb.dstats=0x40; // Peripheral->Computer
  OS.dcb.dbuf=&num_networks;
  OS.dcb.dtimlo=0x0F; // 15 second timeout
  OS.dcb.dbyt=4;      // 4 byte response
  OS.dcb.daux=0;
  siov();

  if (OS.dcb.dstats!=0x01)
    {
      err_sio();
    }
  return OS.dcb.dstats;
}

/**
 * Return Network entry from last scan
 */
unsigned char scan_result(unsigned char n)
{
  OS.dcb.ddevic=0x70;
  OS.dcb.dunit=1;
  OS.dcb.dcomnd=0xFC; // Return scan result
  OS.dcb.dstats=0x40; // Peripheral->Computer
  OS.dcb.dbuf=&ssidInfo.rawData;
  OS.dcb.dtimlo=0x0F; // 15 second timeout
  OS.dcb.dbyt=sizeof(ssidInfo.rawData);
  OS.dcb.daux1=n;     // get entry #n
  siov();

  if (OS.dcb.dstats!=0x01)
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
  
  print("\x9b");
  
  OS.lmargn=2;
  
  print("Scanning...\x9b");
  err=scan();

  if (err==1)
    {
      for (i=0;i<num_networks[0];i++)
      {
        err=scan_result(i);
        if (err!=1)
          break;
        print("* ");
        print(ssidInfo.ssid);
        print("\x9b");
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
