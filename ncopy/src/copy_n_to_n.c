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

int _copy_n_to_n(Context *context)
{
  unsigned char err;
  unsigned short buf_len;
  
  nopen(context->sourceUnit,context->sourceDeviceSpec,IOCB_READ);

  if (OS.dcb.dstats!=1)
    {
      nstatus(context->sourceUnit);
      err=OS.dvstat[3];
      print_error(err);
      nclose(context->sourceUnit);
    }
  
  nopen(context->destUnit,context->destDeviceSpec,IOCB_WRITE);

  if (OS.dcb.dstats!=1)
    {
      nstatus(context->destUnit);
      err=OS.dvstat[3];
      print_error(err);
      nclose(context->sourceUnit);
      nclose(context->destUnit);
    }

  do
    {
      nstatus(context->sourceUnit);
      buf_len=(OS.dvstat[1]<<8)+OS.dvstat[0];

      if (buf_len==0)
	break;
      
      // Be sure not to overflow buffer!
      if (buf_len>sizeof(context->buf))
	buf_len=sizeof(context->buf);

      err=nread(context->sourceUnit,context->buf,buf_len); // add err chk

      if (err!=1)
	{
	  nstatus(context->sourceUnit);
	  err=OS.dvstat[3];
	  print_error(err);
	  goto nndone;
	}

      err=nwrite(context->destUnit,context->buf,buf_len);

      if (err!=1)
	{
	  nstatus(context->destUnit);
	  err=OS.dvstat[3];
	  print_error(err);
	  goto nndone;
	}
      
    } while (buf_len>0);  

 nndone:  
  nclose(context->sourceUnit);
  nclose(context->destUnit);
  
  return 0;
}

int copy_n_to_n(Context *context)
{
  if (detect_wildcard(context->sourceDeviceSpec))
    return _copy_n_to_n(context);
}
