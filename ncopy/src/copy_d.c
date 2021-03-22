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
#include "copy_d.h"
#include "context.h"
#include "wildcard.h"

int _copy_d()
{
  unsigned char err=1;
  unsigned short buf_len;
  
  err=open(D_DEVICE_DATA,IOCB_READ,sourceDeviceSpec,strlen(sourceDeviceSpec));
  
  if (err!=1)
    {
      print_error(err);
      goto dndone;
    }
  
  nopen(destUnit,destDeviceSpec,8);
  
  if (OS.dcb.dstats!=1)
    {
      print_nerror(destUnit);
      goto dndone;
      return err;
    }
  
  do
    {
      err = get(D_DEVICE_DATA,buf,sizeof(buf));
      
      if ((err != 1) && (err != 136))
	{
	  print_error(err);
	  goto dndone;
	}
      
      buf_len=OS.iocb[D_DEVICE_DATA].buflen;
      nwrite(destUnit,buf,buf_len);
      
      if (OS.dcb.dstats!=1)
	{
	  print_nerror(destUnit);
	  goto dndone;
	}
      else
	buf_len-=buf_len;	  
    }
  while (err==1);
  
  if (err==136)
    err=1;
  
 dndone:
  close(D_DEVICE_DATA);
  nclose(destUnit);
  
  return err;
}

int copy_d()
{
  if (detect_wildcard(sourceDeviceSpec)==true)
    return wildcard_d();
  else
    return _copy_d();
}
