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
#include "copy_n_to_n.h"

extern unsigned char destUnit, sourceUnit;
extern unsigned char sourceDeviceSpec[255];
extern unsigned char destDeviceSpec[255];
extern unsigned short data_len;
extern unsigned char data[16384];

int _copy_n_to_n(void)
{
  unsigned char err;
  
  nopen(sourceUnit,sourceDeviceSpec,4);

  if (OS.dcb.dstats!=1)
    {
      nstatus(sourceUnit);
      err=OS.dvstat[3];
      print_error(err);
      nclose(sourceUnit);
    }
  
  nopen(destUnit,destDeviceSpec,8);

  if (OS.dcb.dstats!=1)
    {
      nstatus(destUnit);
      err=OS.dvstat[3];
      print_error(err);
      nclose(sourceUnit);
      nclose(destUnit);
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
	  goto nndone;
	}

      err=nwrite(destUnit,data,data_len);

      if (err!=1)
	{
	  nstatus(destUnit);
	  err=OS.dvstat[3];
	  print_error(err);
	  goto nndone;
	}
      
    } while (data_len>0);  

 nndone:  
  nclose(sourceUnit);
  nclose(destUnit);
  
  return 0;
}

int copy_n_to_n(void)
{
  if (detect_wildcard(sourceDeviceSpec))
    return _copy_n_to_n();
}
