/**
 * Set Login/password for channel 
 *
 * @param u Username
 * @param p Password
 * @return status
 */

#include <string.h>
#include "network.h"

unsigned char network_login(char *u, char *p)
{
  char lc[128]={0xFD};
  char pc[128]={0xFE};
  unsigned char r=0;
  
  strncpy(&lc[1],u,127);
  strncpy(&pc[1],p,127);

  r=adamnet_write(lc,sizeof(lc));
  if (r != 0x80)
    return r;
  
  r=adamnet_write(pc,sizeof(pc));
  
  return r;
}
