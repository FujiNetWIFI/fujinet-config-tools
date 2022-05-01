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
#include <stdbool.h>
#include <string.h>
#include "wildcard.h"
#include "misc.h"
#include "context.h"
#include "blockio.h"
#include "copy_d.h"
#include "copy_n.h"
#include "conio.h"
#include "nsio.h"

static char fname[13];
static char j;

extern char sourcePathSeperator;
extern char destPathSeperator;

void wildcard_setsourceDevice_pos()
{
  sourceDevice_pos = strrchr(sourceDeviceSpec,sourcePathSeperator);
  sourceDevice_pos++;
}

void wildcard_setdestDevice_pos()
{
  destDevice_pos = strrchr(destDeviceSpec,destPathSeperator);
  destDevice_pos++;
}

void wildcard_filename()
{
  memset(fname,0,sizeof(fname));
  j=0;
  for (i=2;i<13;i++)
    {
      if (i==10 && buf[i+1] != ' ')
	fname[j++]='.';
      if (buf[i]!=' ')
	fname[j++]=buf[i];
    }

  strcat(sourceDeviceSpec,fname);
  strcat(destDeviceSpec,fname);

  print("COPYING--");
  print(strrchr(sourceDeviceSpec,sourcePathSeperator)+1);
  print("\x9b");
}

void wildcard_clear_filenames()
{
  // Clear the source and dest filenames
  *sourceDevice_pos=0x00;
  *destDevice_pos=0x00;
}

void wildcard_setup()
{
  dirPos = 0;
  strcpy(wildcardSpec, sourceDeviceSpec);
  wildcard_setsourceDevice_pos();
  wildcard_setdestDevice_pos();
  close(D_DEVICE_DIRECTORY);
}

bool wildcard_valid_dir_entry()
{
  return (buf[0] == ' ') || (buf[0] == '*');
}

int wildcard_d()
{
  unsigned char err=1;

  // init
  wildcard_setup();
  
  while (err==1)
    {
      unsigned char pos;

      wildcard_clear_filenames();

      err=open(D_DEVICE_DIRECTORY,IOCB_DIRECTORY,wildcardSpec,strlen(wildcardSpec));
      if (err!=1)
	{
	  print_error(err);
	  break;
	}

      // Get requested directory entry.
      for (pos=0;pos<=dirPos;pos++)
	{
	  err = getrec(D_DEVICE_DIRECTORY,buf,64);
	  if (err != 1)
	    break;
	}
      
      close(D_DEVICE_DIRECTORY);

      if (err != 1)
	break;

      if (!wildcard_valid_dir_entry())
	{
	  dirPos++;
	  continue;
	}
      else
	wildcard_filename();
      
      // Do copy
      err = copy_d();

      // Did copy error?
      if ((err = copy_d()) != 1)
      	break;

      // Otherwise, increment to next filename, and continue.
      dirPos++;
    }
  
  return err;
}

int wildcard_n(bool dest_is_n)
{
  unsigned char err=1;
  unsigned short bw;

  // Migrate prefix from source unit to unit 8
  memset(wildcardSpec,0,256);
  strcat(wildcardSpec,"N8:");
  memset(buf,0,256);
  ncd(8,buf);
  npwd(sourceUnit,buf);
  strcat(wildcardSpec,buf);
  ncd(8,wildcardSpec);
  
  // init
  wildcard_setup();
  
  err=nopen(8,wildcardSpec,IOCB_DIRECTORY);
  
  if (err!=1)
    {
      print_nerror(err);
      goto wndone;
    }

  while (err==1)
    {
      wildcard_clear_filenames();

      nstatus(8);
      bw=OS.dvstat[0]+(OS.dvstat[1]*256);

      if (bw==0)
	{
	  err=136;
	  break;
	}
      
      err=nread(8,buf,bw > 18 ? 18 : bw);
      
      if (err != 1)
	break;
      
      if (wildcard_valid_dir_entry())
	{
	  wildcard_filename();
	  
	  /* // Do copy */
	  if ((err = copy_n(dest_is_n)) != 1)
	    break;	  
	}
    }

 wndone:
  nclose(8);
  return err;
}
