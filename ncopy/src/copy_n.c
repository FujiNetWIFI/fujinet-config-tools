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
#include "copy_n.h"
#include "context.h"
#include "wildcard.h"

int _copy_n(bool dest_is_n)
{
  unsigned char err=1;
  unsigned short buf_len;

  err=nopen(sourceUnit,sourceDeviceSpec,IOCB_READ);
  
  if (err!=1)
    {
      print_error(err);
      goto ndone;
    }

  if (dest_is_n==true)
    err=nopen(destUnit,destDeviceSpec,IOCB_WRITE);
  else
    err=open(D_DEVICE_DATA,IOCB_WRITE,destDeviceSpec,strlen(destDeviceSpec));
  
  if (err!=1)
    {
      if (dest_is_n==true)
	print_nerror(destUnit);
      else
	print_error(err);
      
      goto ndone;
    }
  
  do
    {
      nstatus(sourceUnit);
      buf_len = (OS.dvstat[1]*256)+OS.dvstat[0];
      err=OS.dvstat[3];
      
      if (buf_len>0)
	{
	  nread(sourceUnit,buf,buf_len);
	  
	  if ((err != 1) && (err != 136))
	    {
	      print_error(err);
	      goto ndone;
	    }
	}
      else
	break;

      if (dest_is_n==true)
	err=nwrite(destUnit,buf,buf_len);
      else
	err=put(D_DEVICE_DATA,buf,buf_len);
      
      if (err!=1)
	{
	  if (dest_is_n==true)
	    print_nerror(destUnit);
	  else
	    print_error(err);
	  goto ndone;
	}
    }
  while (err==1);
  
  if (err==136)
    err=1;
  
 ndone:
  if (dest_is_n==true)
    nclose(destUnit);
  else
    close(D_DEVICE_DATA);

  return err;
}

int copy_n(bool dest_is_n)
{
  if (detect_wildcard(sourceDeviceSpec))
    return wildcard_n(dest_is_n);
  else
    return _copy_n(dest_is_n);
}
