#include <fujicpm.h>
#include <stdio.h>
#include <stdlib.h>

AdapterConfig ac;

void main(void)
{  
  printf("\x1b" "E" "FUJINET CONFIG\n");
  printf("--------------\n\n");

  fuji_write("\xE8",1);
  fuji_read((char *)ac);
  
  printf("%28s\n","SSID");
  printf("%28s\n\n",ac.ssid);
  printf("%10s%s\n","HOSTNAME:",ac.hostname);
  printf("%10s%u.%u.%u.%u\n","IP:",ac.localIP[0],ac.localIP[1],ac.localIP[2],ac.localIP[3]);
  printf("%10s%u.%u.%u.%u\n","NETMASK:",ac.netmask[0],ac.netmask[1],ac.netmask[2],ac.netmask[3]);
  printf("%10s%u.%u.%u.%u\n","DNS:",ac.dnsIP[0],ac.dnsIP[1],ac.dnsIP[2],ac.dnsIP[3]);
  printf("%10s%02x:%02x:%02x:%02x:%02x:%02x\n","MAC:",ac.macAddress[0],ac.macAddress[1],ac.macAddress[2],ac.macAddress[3],ac.macAddress[4],ac.macAddress[5]);
  printf("%10s%02x:%02x:%02x:%02x:%02x:%02x\n","BSSID:",ac.bssid[0],ac.bssid[1],ac.bssid[2],ac.bssid[3],ac.bssid[4],ac.bssid[5]);
  printf("%10s%s\n\n","FNVER:",ac.fn_version);
}
