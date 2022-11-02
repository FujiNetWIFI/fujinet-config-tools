/**
 * #Fujinet Network Functions
 */

#include <stdbool.h>

#ifndef NETWORK_H
#define NETWORK_H

#define NET_DEV 0x09

typedef struct
{
  unsigned short rxBytesWaiting;
  bool connected;
  unsigned char error;
} NetStatus;

typedef struct _dcb
{
  unsigned char status;
  void *buf;
  unsigned short len;
  unsigned long block;
  unsigned char unit;
  unsigned char reserved0;
  unsigned char reserved1;
  unsigned char reserved2;
  unsigned char reserved3;
  unsigned char reserved4;
  unsigned char reserved5;
  unsigned char dev;
  unsigned short max_len;
  unsigned char type;
  unsigned char dev_status;
} DCB;

DCB *find_dcb(void);
unsigned char adamnet_read(char *buf, unsigned short len);
unsigned char adamnet_write(char *buf, unsigned short len);
unsigned char network_open(char* url, unsigned char mode, unsigned char translation);
unsigned char network_close(void);
unsigned char network_status(NetStatus *ns);
unsigned short network_read(char *buf, unsigned short len);
unsigned char network_write(unsigned char *buf, unsigned short len);
unsigned char network_statusbyte(void);

#endif /* NETWORK_H */
