/**
 * nget 
 */

#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "network.h"

#define READ 4
#define NO_TRANSLATION 0

#define RETRY_COUNT 128
#define CONNECTED 2

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

void get_url(char *c)
{
  cprintf("URL: ");
  input(c);
}

void get_filename(char *c)
{
  cprintf("DEST FILENAME: ");
  input(c);
}

bool did_we_connect(void)
{
  bool retries = RETRY_COUNT;
  
  // Wait until connection established
  while (retries > 0)
    {
      printf(".");
      csleep(5);
      if (network_statusbyte() & CONNECTED)
	{
	  cprintf("CONNECTED!\n");
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

void get(char *u, char *f)
{
  FILE *fp = fopen(f,"w");
  unsigned long total = 0;
  
  if (fp == NULL)
    {
      cprintf("COULD NOT OPEN DEST FILE %s - ABORTING\n",f);
      fclose(fp);
      exit(1);
    }

  printf("Open Network... URL: %s",u);
  network_open(u,READ,NO_TRANSLATION);
  
  if (!did_we_connect())
    goto bye;

  while (1)
    {
      char buf[1024];
      unsigned short l = network_read(buf,sizeof(buf));

      if (fwrite(buf,sizeof(char),l,fp) != l)
	{
	  cprintf("COULD NOT WRITE TO DEST FILE. ABORTING.\n");
	  goto bye;
	}

      total += l;
      
      cprintf("%8lu BYTES TRANSFERRED.\r",total);
      
      if (!(network_statusbyte() & CONNECTED))
	{
	  l = network_read(buf,sizeof(buf));
	  fwrite(buf,sizeof(char),l,fp);
	  cprintf("\n\nDONE.\n");
	  break;
	}

    }

 bye:
  network_close();
  fclose(fp);
}

void main(void)
{
  char url[256];
  char filename[16];

  memset(url,0,sizeof(url));
  memset(filename,0,sizeof(filename));

  while (url[0]==0x00) { get_url(url); }
  while (filename[0]==0x00) { get_filename(filename); }

  get(url,filename);
}
