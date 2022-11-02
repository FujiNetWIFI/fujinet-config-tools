/**
 * nget 
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

void get_url(char *c)
{
  cprintf("URL: ");
  scanf("%s",c);
}

bool did_we_connect(void)
{
  bool retries = RETRY_COUNT;
  
  // Wait until connection established
  while (retries > 0)
    {
      csleep(5);
      if (network_statusbyte() & CONNECTED)
	{
	  return true;
	}
      else
	retries--;
    }

  if (retries == 0)
    {
      cprintf("COULD NOT OPEN NET CONNECTION.\n");
      return false;
    }
}

void dir(char *u)
{
  network_open(u,DIR,LONG_DIRECTORY);

  /* if (!did_we_connect()) */
  /*   { */
  /*     cprintf("COULD NOT OPEN CONNECTION. ABORTING.\n"); */
  /*     goto bye; */
  /*   } */

  do
    {
      char buf[1024];
      unsigned short l = network_read(buf,sizeof(buf));

      for (int i=0;i<l;i++)
	putchar(buf[i]==ATASCII_EOL ? 0x0A : buf[i]);
      
    } while (network_statusbyte() & CONNECTED);

 bye:
  network_close();
}

void main(void)
{
  char url[256];

  memset(url,0,sizeof(url));

  while (url[0]==0x00) { get_url(url); }

  dir(url);
}
