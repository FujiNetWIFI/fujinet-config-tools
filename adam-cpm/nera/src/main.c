/**
 * ncd
 */

#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "network.h"

#define DIR 6
#define LONG_DIRECTORY 128

#define RETRY_COUNT 128
#define CONNECTED 2

#define ATASCII_EOL 0x9B

void input(char *c)
{
  char k=0;
  int pos=0;

  while (k != 0x0A)
    {
      k=cgetc();
      switch(k)
	{
	case 0x03:
	  exit(255);
	  break;
	case 0x08:
	  if (pos>0)
	    {
	      pos--;
	      c[pos]=0x00;
	    }
	  else
	    k=0x00;
	  break;
	case 0x0A:
	  putchar(0x0a);
	  return;
	  break;
	default:
	  c[pos++]=k;
	  break;
	}
      if (k>0)
	putchar(k);
    }
}

void main(void)
{
  char url[256];
  const char clear="N:";
  
  memset(url,0,sizeof(url));
  
  cprintf("URL: ");
  input(url);

  if (url[0]!=0x00)
    network_delete(url);
}
