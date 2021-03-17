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
#include "context.h"
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

Context context;

int main(int argc, char* argv[])
{  
  OS.lmargn=2;

  // Args processing.  
  if (argc>1)
    {
      // CLI DOS, concatenate arguments together.
      for (i=1;i<argc;i++)
	{
	  strcat(context.buf,argv[i]);
	  if (i<argc-1)
	    strcat(context.buf," ");
	}
    }
  else
    {
      // Interactive
      print("NET COPY--FROM, TO?\x9b");
      get_line(context.buf,255);
    }

  if (parse_filespec(&context)==0)
    {
      print("COULD NOT PARSE FILESPEC.\x9b");
      return(1);
    }

  if (valid_cio_device(context.sourceDeviceSpec[0]) && valid_network_device(context.destDeviceSpec[0]))
    return copy_d_to_n(&context);
  else if (valid_network_device(context.sourceDeviceSpec[0]) && valid_cio_device(context.destDeviceSpec[0]))
    return copy_n_to_d(&context);
  else if (valid_network_device(context.sourceDeviceSpec[0]) && valid_network_device(context.destDeviceSpec[0]))
    return copy_n_to_n(&context);

  if (!_is_cmdline_dos())
    {
      print("\x9bPRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
      get_line(context.buf,sizeof(context.buf));
    }
  
}
