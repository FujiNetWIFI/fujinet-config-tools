/**
 * Simple CIO GETCHR/PUTCHR wrappers
 */

#include <atari.h>
#include "blockio.h"
#include "cio.h"

void open(unsigned char channel, unsigned char aux1, char* buf, unsigned short len)
{
  OS.iocb[channel].buffer=buf;
  OS.iocb[channel].buflen=len;
  OS.iocb[channel].command=IOCB_OPEN;
  OS.iocb[channel].aux1=aux1;
  ciov(channel);
}

void close(unsigned char channel)
{
  OS.iocb[channel].command=IOCB_CLOSE;
  ciov(channel);
}

void get(unsigned char channel, char* buf, unsigned short len)
{
  OS.iocb[channel].buffer=buf;
  OS.iocb[channel].buflen=len;
  OS.iocb[channel].command=IOCB_GETCHR;
  ciov(channel);
}

void getrec(unsigned char channel, char* buf, unsigned short len)
{
  OS.iocb[channel].buffer=buf;
  OS.iocb[channel].buflen=len;
  OS.iocb[channel].command=IOCB_GETREC;
  ciov(channel);
}

void put(unsigned char channel, char* buf, unsigned short len)
{
  OS.iocb[channel].buffer=buf;
  OS.iocb[channel].buflen=len;
  OS.iocb[channel].command=IOCB_PUTCHR;
  ciov(channel);
}
