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
extern unsigned char sourceUnit;
extern unsigned char sourceDeviceSpec[255];
extern unsigned char destDeviceSpec[255];
extern unsigned short data_len;
extern unsigned char data[16384];

int _copy_n_to_d(void)
{
  nopen(sourceUnit,sourceDeviceSpec,4);

  if (OS.dcb.dstats!=1)
    {
      nstatus(sourceUnit);
      yvar=OS.dvstat[3];
      print_error();
      nclose(sourceUnit);
    }

  open(D_DEVICE_DATA,8,destDeviceSpec,strlen(destDeviceSpec));

  if (yvar!=1)
    {
      print_error();
      close(D_DEVICE_DATA);
      return yvar;
    }  

  do
    {
      nstatus(sourceUnit);
      data_len=OS.dvstat[1]*256+OS.dvstat[0];

      if (data_len==0)
	break;
      
      // Be sure not to overflow buffer!
      if (data_len>sizeof(data))
	data_len=sizeof(data);

      nread(sourceUnit,data,data_len); // add err chk

      put(D_DEVICE_DATA,data,data_len);
      
    } while (data_len>0);

  nclose(sourceUnit);
  close(D_DEVICE_DATA);
  return 0;
}

int copy_n_to_d(void)
{
  if (detect_wildcard(sourceDeviceSpec)==false)
    return _copy_n_to_d();
}
