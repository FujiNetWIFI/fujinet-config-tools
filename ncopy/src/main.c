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
#include "copy_n_to_d.h"
#include "copy_n_to_n.h"

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
