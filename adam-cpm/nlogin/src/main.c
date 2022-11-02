/**
 * N: Login
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include "network.h"

void get_password(char *p)
{
  char k=0;
  unsigned char pos=0;
  
  while (k!=0x0a)
    {
      k=getch();

      switch(k)
	{
	case 0x08: // BS
	  if (pos>0)
	    {
	      pos--;
	      p[pos]=0;
	      putchar(0x08);
	    }
	  break;
	case 0x0a:
	  putchar(0x0d);
	  putchar(0x0a);
	  break;
	default:
	  p[pos++]=k;
	  putchar(0x2A);
	  break;
	}
    }
}

void get_username(char *u)
{
  char k=0;
  unsigned char pos=0;

  while (k!=0x0a)
    {
      k=getch();

      switch(k)
	{
	case 0x08: // BS
	  if (pos>0)
	    {
	      pos--;
	      putchar(0x08);
	    }
	case 0x0a:
	  putchar(0x0d);
	  putchar(0x0a);
	  break;
	default:
	  u[pos++]=k;
	  putchar(k);
	  break;
	}
    }
}

void main(void)
{
  char u[128];
  char p[128];
  
  memset(u,0,128);
  memset(p,0,128);

  printf("USERNAME: ");
  get_username(u);

  printf("PASSWORD: ");
  get_password(p);

  network_login(u,p);
}
