/**w
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
#include "context.h"

int _copy_n_to_d()
{
  unsigned char err=1;
  unsigned short buf_len;

  nopen(sourceUnit,sourceDeviceSpec,IOCB_READ);

  if (OS.dcb.dstats!=1)
    {
      print_nerror(sourceUnit);
      goto nddone;
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
	  goto nddone;
	}
      else
	{
	  err=put(D_DEVICE_DATA,buf,buf_len);
	  if (err!=1)
	    {
	      print_error(err);
	      goto nddone;
	    }
	}
    } while (buf_len>0);

 nddone:
  nclose(sourceUnit);
  close(D_DEVICE_DATA);
  
  return err == 1 ? 0 : err;
}

int copy_n_to_d()
{
  if (detect_wildcard(sourceDeviceSpec)==false)
    return _copy_n_to_d();
}
