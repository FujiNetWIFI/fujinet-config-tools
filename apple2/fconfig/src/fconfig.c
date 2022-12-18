/**
 * Show FujiNet Adapter Config
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <apple2.h>
#include <conio.h>
#include <stdio.h>
#include <stdint.h>
#include "sp.h"

#define GET_ADAPTER_CONFIG 0xe8

int8_t fuji; // The FujiNet device #

struct _adapterConfig
{
  char ssid[33];
  char hostname[64];
  unsigned char localIP[4];
  unsigned char gateway[4];
  unsigned char netmask[4];
  unsigned char dnsIP[4];
  unsigned char macAddress[6];
  unsigned char bssid[6];
  char firmware[15];
} *adapterConfig;

void print_adapter_config(void)
{
  unsigned char err;
  
  printf("\n");

  err = sp_status(fuji,GET_ADAPTER_CONFIG);

  if (err)
    {
      printf("ERROR RETRIEVING ADAPTER CONFIGURATION.\n\n");
      return;
    }

  adapterConfig = (struct _adapterConfig *)&sp_payload[0];

  printf("FUJINET CONFIGURATION\n");
  printf("---------------------\n\n");

  printf("SSID:\n%-32s\n\n",adapterConfig->ssid);

  printf("HOSTNAME:\n%-64s\n\n",adapterConfig->hostname);

  printf("      IP: %3u.%3u.%3u.%3u\n",
	 adapterConfig->localIP[0],
	 adapterConfig->localIP[1],
	 adapterConfig->localIP[2],
	 adapterConfig->localIP[3]);

  printf(" NETMASK: %3u.%3u.%3u.%3u\n",
	 adapterConfig->netmask[0],
	 adapterConfig->netmask[1],
	 adapterConfig->netmask[2],
	 adapterConfig->netmask[3]);

  printf(" GATEWAY: %3u.%3u.%3u.%3u\n",
	 adapterConfig->gateway[0],
	 adapterConfig->gateway[1],
	 adapterConfig->gateway[2],
	 adapterConfig->gateway[3]);

  printf("     DNS: %3u.%3u.%3u.%3u\n",
	 adapterConfig->dnsIP[0],
	 adapterConfig->dnsIP[1],
	 adapterConfig->dnsIP[2],
	 adapterConfig->dnsIP[3]);

  printf("     MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
	 adapterConfig->macAddress[0],
	 adapterConfig->macAddress[1],
	 adapterConfig->macAddress[2],
	 adapterConfig->macAddress[3],
	 adapterConfig->macAddress[4],
	 adapterConfig->macAddress[5]);

  printf("   BSSID: %02x:%02x:%02x:%02x:%02x:%02x\n",
	 adapterConfig->bssid[0],
	 adapterConfig->bssid[1],
	 adapterConfig->bssid[2],
	 adapterConfig->bssid[3],
	 adapterConfig->bssid[4],
	 adapterConfig->bssid[5]);

  printf("   FNVER: %s",
	 adapterConfig->firmware);
  
  printf("\n\n");
}

void main(void)
{
  cursor(1);
  sp_init();
  fuji = sp_find_fuji();

  if (fuji == 0)
      printf("\nCOULD NOT FIND FUJINET.\n");
  else
    print_adapter_config();

  printf("PRESS ANY KEY TO CONTINUE.");
  cgetc();  
}
