/**
 * Read from channel 
 *
 * @param buf Buffer
 * @param len length
 * @return adamnet status code
 */

#include <string.h>
#include <conio.h>
#include "network.h"

extern unsigned char response[1024];

char *network_get_prefix(void)
{
  char c='0'; // 0x30 (PWD)
  unsigned char r=adamnet_write(&c,1);

  if (r!=0x80)
    return NULL;

  r=adamnet_read(response,sizeof(response));
  return response;
}
