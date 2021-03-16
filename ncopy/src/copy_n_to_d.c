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
#include "copy_n_to_d.h"

extern unsigned char sourceUnit;
extern unsigned char sourceDeviceSpec[255];
extern unsigned char destDeviceSpec[255];
extern unsigned short data_len;
extern unsigned char data[16384];

int _copy_n_to_d(void)
{
  unsigned char err=1;
  
  nopen(sourceUnit,sourceDeviceSpec,IOCB_READ);

  if (OS.dcb.dstats!=1)
    {
      nstatus(sourceUnit);
      err=OS.dvstat[3];
      print_error(err);
      nclose(sourceUnit);
    }

  err=open(D_DEVICE_DATA,IOCB_WRITE,destDeviceSpec,strlen(destDeviceSpec));

  if (err!=1)
    {
      print_error(err);
      goto nddone;
    }  

  do
    {
      nstatus(sourceUnit);
      data_len=(OS.dvstat[1]<<8)+OS.dvstat[0];

      if (data_len==0)
	break;
      
      // Be sure not to overflow buffer!
      if (data_len>sizeof(data))
	data_len=sizeof(data);

      err=nread(sourceUnit,data,data_len); // add err chk

      if (err!=1)
	{
	  nstatus(sourceUnit);
	  err=OS.dvstat[3];
	  print_error(err);
	  goto nddone;
	}
      else
	{
	  err=put(D_DEVICE_DATA,data,data_len);
	  if (err!=1)
	    {
	      print_error(err);
	      goto nddone;
	    }
	}
    } while (data_len>0);

 nddone:
  nclose(sourceUnit);
  close(D_DEVICE_DATA);
  
  return err == 1 ? 0 : err;
}

int copy_n_to_d(void)
{
  if (detect_wildcard(sourceDeviceSpec)==false)
    return _copy_n_to_d();
}
