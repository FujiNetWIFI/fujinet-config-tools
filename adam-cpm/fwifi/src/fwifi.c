/**
 * Set wifi parameters
 */

#include <stdio.h>
#include <stdlib.h>
#include <fujicpm.h>
#include <string.h>
#include <conio.h>

char meter[4]={32,32,32,0};

char *scan_result_meter(SSIDInfo *s)
{
  memset(meter,32,3);
  
  if (s->rssi > -40)
    {
      meter[0] = '*';
      meter[1] = '*';
      meter[2] = '*';
    }
  else if (s->rssi > -60)
    {
      meter[0] = '*';
      meter[1] = '*';
    }
  else
    {
      meter[0] = '*';
    }

  return meter;
}

char *scan_result_name(SSIDInfo *s)
{
  return s->ssid;
}

void get_password(char *c)
{
  char x=0;
  char pos=0;
  
  printf("ENTER PASSWORD: ");
  while ((x=cgetc()) != 0x0A)
    {
      switch (x)
	{
	case 0x08:
	  if (pos>0)
	    {
	      pos--;
	      c--;
	      *c=0x00;
	      putchar(0x08);
	    }
	  break;
	default:
	  pos++;
	  *c=x;
	  c++;
	  putchar('*');
	  break;
	}
    }
  putchar(0x0A);
}

void main(void)
{
  char nn=0;
  char e[4];
  char ssid[32];
  char password[64];
  char attempts=20;
  
  printf("SCANNING FOR NETWORKS...\n");

  nn=fuji_scan_network();

  if (nn==0)
    {
      printf("NO NETWORKS FOUND.\n");
      exit(1);
    }
  else
    {
      printf("%d NETWORKS FOUND.\nPLEASE SELECT A NETWORK:\n\n",nn);
    }

  for (char i=0;i<nn;i++)
    {
      SSIDInfo *s=fuji_get_scan_result(i);
      printf("%2d: [%s] %s\n",i,scan_result_meter(s),scan_result_name(s));
    }

  printf(" H: TO SPECIFY HIDDEN SSID\n\n");

  printf(" SELECT SSID #: ");
  gets(e);

  if (e[0]==0x00)
    exit(0);
  else if (e[0]=='H')
    {
      printf("ENTER NAME OF SSID: ");
      gets(ssid);
      get_password(password);
    }
  else
    {
      SSIDInfo *s = fuji_get_scan_result(atoi(e));
      strcpy(ssid,s->ssid);
      get_password(password);
    }

  printf("SETTING SSID %s\n",ssid);
  fuji_set_ssid(ssid,password);
  
  printf("WAITING FOR NETWORK...\n");

  while ((attempts--) > 0)
    {
      char s=fuji_get_wifi_status();
      switch (s)
	{
	case 1:
	  printf("NO SSID AVAILABLE.\n");
	  return;
	case 3:
	  printf("CONNECTION SUCCESSFUL.\n");
	  return;
	case 4:
	  printf("CONNECT FAILED.\n");
	  return;
	case 5:
	  printf("CONNECTION LOST.\n");
	  return;
	default:
	  continue;
	}
    }

  printf("UNABLE TO CONNECT.\n");
}
