/**
 * Network Testing tools
 *
 * ncopy - copy files 
 *  N:<->D: D:<->N: or N:<->N:
 *
 * Author: Thomas Cherryhomes
 *  <thom.cherryhomes@gmail.com>
 *
 * Released under GPL 3.0
 * See COPYING for details.
 */

#include <atari.h>
#include <string.h>
#include "conio.h"
#include "err.h"
#include "nsio.h"
#include "blockio.h"
#include "misc.h"
#include "copy_d_to_n.h"

extern unsigned char yvar;
extern unsigned char destUnit;
extern unsigned char sourceDeviceSpec[255];
extern unsigned char destDeviceSpec[255];
extern unsigned short data_len;
extern unsigned char data[16384];

int _copy_d_to_n(void)
{
  open(D_DEVICE_DATA,4,sourceDeviceSpec,strlen(sourceDeviceSpec));

  if (yvar!=1)
    {
      print_error();
      close(D_DEVICE_DATA);
      return yvar;
    }

  nopen(destUnit,destDeviceSpec,8);

  if (OS.dcb.dstats!=1)
    {
      nstatus(destUnit);
      yvar=OS.dvstat[3];
      print_error();
      close(D_DEVICE_DATA);
      nclose(destUnit);
    }

  while (yvar==1)
    {
      get(D_DEVICE_DATA,data,sizeof(data));

      data_len=OS.iocb[D_DEVICE_DATA].buflen;

      nwrite(destUnit,data,data_len);
      data_len-=data_len;	  
    }

  close(D_DEVICE_DATA);
  nclose(destUnit);
  
  return 0;
}

int copy_d_to_n(void)
{
  if (detect_wildcard(sourceDeviceSpec)==false)
    return _copy_d_to_n();
}
