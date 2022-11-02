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

void main(void)
{
  cprintf("%s\n",network_get_prefix());
}
