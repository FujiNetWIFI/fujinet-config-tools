/**
 * Write to channel 
 *
 * @param buf Pointer to buffer, should have +1 more than number of bytes to write.
 * @param len number of actual bytes to write
 * @return number of bytes written
 * @return status
 */

#include <string.h>
#include <conio.h>
#include "network.h"

unsigned char network_write(unsigned char *buf, unsigned short len)
{  
  while (len>0)
    {
      char out[513]={'W'};
      unsigned short l = len > 128 ? 128 : len;
      unsigned char r=0;
      
      memcpy(&out[1],buf,l);

      r=adamnet_write(out,l+1);

      if (r!=0x80)
	break;
      else
	{
	  len -= l;
	}
      
    }
}

