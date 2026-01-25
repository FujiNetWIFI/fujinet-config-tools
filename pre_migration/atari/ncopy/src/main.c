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
#include "nsio.h"
#include "blockio.h"
#include "misc.h"
#include "parse_filespec.h"
#include "copy_d.h"
#include "copy_n.h"
#include "copy_n.h"

/* GLOBALS */

char buf[8192]; // changed so we don't conflict with cart if there.
unsigned char sourceUnit;
unsigned char destUnit;
unsigned short dirPos;
char wildcardSpec[256];
char wildcardPath[256];
char sourceDeviceSpec[256];
char destDeviceSpec[256];
char* sourceDevice_pos;
char* destDevice_pos;
char sourcePathSeperator;
char destPathSeperator;

unsigned char i;

/**
 * @brief Shown if in ATARI DOS 3
 */
void dos3_clear(void)
{
	OS.rowcrs=9;
	OS.colcrs=2;

    print("\xC3\xEF\xF0\xF9\xA0\xCE\xE5\xF4\xF7\xEF\xF2\xEB\xA0\xC6\xE9\xEC\xE5\xA8\xF3\xA9\x9b\x9b");
    print("\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c");
}

int main(int argc, char* argv[])
{
  unsigned char err=0;

  if (PEEK(0x715) == 0x38)
      dos3_clear();

  OS.lmargn=2;
  // Args processing.
  if (argc>1)
    {
      // CLI DOS, concatenate arguments together.
      for (i=1;i<argc;i++)
  	{
  	  strcat(buf,argv[i]);
  	  if (i<argc-1)
  	    strcat(buf," ");
  	}
    }
  else
    {
      // Interactive
      print("NET COPY--FROM, TO?\x9b");
      get_line(buf,255);
    }

  if (parse_filespec()==0)
    {
      print("COULD NOT PARSE FILESPEC.\x9b");
      return(1);
    }

  if (valid_cio_device(sourceDeviceSpec[0]) && valid_network_device(destDeviceSpec[0]))
    return copy_d();
  else if (valid_network_device(sourceDeviceSpec[0]) && valid_cio_device(destDeviceSpec[0]))
    return copy_n(false);
  else if (valid_network_device(sourceDeviceSpec[0]) && valid_network_device(destDeviceSpec[0]))
    return copy_n(true);

  if (_dos_type == MYDOS || err != 1)
    {
      print("\x9bPRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
      get_line(buf,sizeof(buf));
    }

  return err==1 ? 0 : err;
}
