/**
 * Close Connection
 *
 * @return status
 */

#include <string.h>
#include "network.h"

unsigned char network_close(void)
{
  char resp='C';
  
  return adamnet_write(&resp,1);
}
