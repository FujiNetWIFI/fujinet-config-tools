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

extern unsigned char yvar;
extern unsigned char destUnit, sourceUnit;
extern unsigned char sourceDeviceSpec[255];
extern unsigned char destDeviceSpec[255];
extern unsigned short data_len;
extern unsigned char data[16384];

int _copy_n_to_n(void)
{
  nopen(sourceUnit,sourceDeviceSpec,4);

  if (OS.dcb.dstats!=1)
    {
      nstatus(sourceUnit);
      yvar=OS.dvstat[3];
      print_error();
      nclose(sourceUnit);
    }
  
  nopen(destUnit,destDeviceSpec,8);

  if (OS.dcb.dstats!=1)
    {
      nstatus(destUnit);
      yvar=OS.dvstat[3];
      print_error();
      nclose(sourceUnit);
      nclose(destUnit);
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
      nwrite(destUnit,data,data_len);
      
    } while (data_len>0);  
  
  nclose(sourceUnit);
  nclose(destUnit);
  
  return 0;
}

int copy_n_to_n(void)
{
  if (detect_wildcard(sourceDeviceSpec))
    return _copy_n_to_n();
}
