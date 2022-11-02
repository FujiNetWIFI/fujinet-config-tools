/**
 * Rename file from network FS
 *
 * @param URL of form: N:PROTO://[HOSTNAME]:PORT/PATH/.../
 * @param new filename
 * @return status
 */

#include <string.h>
#include "network.h"

unsigned char network_rename(char *url,char *newname)
{
  char resp[257];
  
  resp[0]=' '; // 0x20

  strncpy(&resp[1],url,256);
  strcat(resp,",");
  strcat(resp,newname);

  return adamnet_write(resp,strlen(resp));
}
