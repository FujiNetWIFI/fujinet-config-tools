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
#include "context.h"

int _copy_n_to_n()
{
  unsigned char err;
  unsigned short buf_len;
  
  nopen(sourceUnit,sourceDeviceSpec,IOCB_READ);

  if (OS.dcb.dstats!=1)
    {
      print_nerror(sourceUnit);
      goto nndone;
    }
  
  nopen(destUnit,destDeviceSpec,IOCB_WRITE);

  if (OS.dcb.dstats!=1)
    {
      print_nerror(destUnit);
      goto nndone;
    }

  do
    {
      nstatus(sourceUnit);
      buf_len=(OS.dvstat[1]<<8)+OS.dvstat[0];

      if (buf_len==0)
	break;
      
      // Be sure not to overflow buffer!
      if (buf_len>sizeof(buf))
	buf_len=sizeof(buf);

      err=nread(sourceUnit,buf,buf_len); // add err chk

      if (err!=1)
	{
	  print_nerror(sourceUnit);
	  goto nndone;
	}

      err=nwrite(destUnit,buf,buf_len);

      if (err!=1)
	{
	  print_nerror(destUnit);
	  goto nndone;
	}
      
    } while (buf_len>0);  

 nndone:  
  nclose(sourceUnit);
  nclose(destUnit);
  
  return 0;
}

int copy_n_to_n()
{
  if (detect_wildcard(sourceDeviceSpec))
    return _copy_n_to_n();
}
