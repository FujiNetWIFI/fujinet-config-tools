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
#include <stdlib.h>
#include <peekpoke.h>
#include <stdbool.h>
#include "sio.h"
#include "conio.h"
#include "err.h"
#include "nsio.h"
#include "blockio.h"
#include "misc.h"
#include "parse_filespec.h"
#include "copy_d_to_n.h"

#define D_DEVICE_DATA      2
#define D_DEVICE_DIRECTORY 3

unsigned char yvar;

unsigned char i;
unsigned char argbuf[255];
unsigned char sourceDeviceSpec[255];
unsigned char destDeviceSpec[255];
unsigned char data[16384];
unsigned short data_len;
unsigned short block_len;
unsigned char* dp;
char errnum[4];
char* pToken;
char* pWildcardStar, *pWildcardChar;
unsigned char sourceUnit=1, destUnit=1;
char buf[8];

int _copy_n_to_d(void)
{
  nopen(sourceUnit,sourceDeviceSpec,4);

  if (OS.dcb.dstats!=1)
    {
      nstatus(destUnit);
      yvar=OS.dvstat[3];
      print_error();
      nclose(destUnit);
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

bool valid_network_device(char d)
{
  return (d=='N');
}

bool valid_cio_device(char d)
{
  return (d!='N' && (d>0x40 && d<0x5B));
}

int main(int argc, char* argv[])
{
  OS.lmargn=2;

  // Args processing.  
  if (argc>1)
    {
      // CLI DOS, concatenate arguments together.
      for (i=1;i<argc;i++)
	{
	  strcat(argbuf,argv[i]);
	  if (i<argc-1)
	    strcat(argbuf," ");
	}
    }
  else
    {
      // Interactive
      print("NET COPY--FROM, TO?\x9b");
      get_line(argbuf,255);
    }

  if (parse_filespec(argbuf)==0)
    {
      print("COULD NOT PARSE FILESPEC.\x9b");
      return(1);
    }

  if (valid_cio_device(sourceDeviceSpec[0]) && valid_network_device(destDeviceSpec[0]))
    return copy_d_to_n();
  else if (valid_network_device(sourceDeviceSpec[0]) && valid_cio_device(destDeviceSpec[0]))
    return copy_n_to_d();
  else if (valid_network_device(sourceDeviceSpec[0]) && valid_network_device(destDeviceSpec[0]))
    return copy_n_to_n();

  if (!_is_cmdline_dos())
    {
      print("\x9bPRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
      get_line(buf,sizeof(buf));
    }
  
}
