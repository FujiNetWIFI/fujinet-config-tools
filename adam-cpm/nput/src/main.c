/**
 * nput 
 */

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "network.h"

#define WRITE 8
#define NO_TRANSLATION 0

#define RETRY_COUNT 128
#define CONNECTED 2

char buf[512];

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
  cprintf("SOURCE FILENAME: ");
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

void put(char *u, char *f)
{
  FILE *fp = fopen(f,"rb");
  unsigned long total = 0;
  
  if (fp == NULL)
    {
      cprintf("COULD NOT OPEN SOURCE FILE %s - ABORTING\n",f);
      fclose(fp);
      exit(1);
    }

  network_open(u,WRITE,NO_TRANSLATION);

  while (!feof(fp))
    {
      int l = fread(buf,sizeof(char),128,fp);

      network_write(buf,l);
      
      total += l;
      
      cprintf("\r%8lu BYTES TRANSFERRED.\r",total);
    }

  cprintf("\nDONE.\n");
  
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

  while (filename[0]==0x00) { get_filename(filename); }
  while (url[0]==0x00) { get_url(url); }

  put(url,filename);
}
