/**
 * Simple CIO GETCHR/PUTCHR wrappers
 */

#include <atari.h>
#include "blockio.h"
#include "cio.h"

unsigned char open(unsigned char channel, unsigned char aux1, char* buf, unsigned short len)
{
  OS.iocb[channel].buffer=buf;
  OS.iocb[channel].buflen=len;
  OS.iocb[channel].command=IOCB_OPEN;
  OS.iocb[channel].aux1=aux1;
  return ciov(channel);
}

unsigned char close(unsigned char channel)
{
  OS.iocb[channel].command=IOCB_CLOSE;
  return ciov(channel);
}

unsigned char get(unsigned char channel, char* buf, unsigned short len)
{
  OS.iocb[channel].buffer=buf;
  OS.iocb[channel].buflen=len;
  OS.iocb[channel].command=IOCB_GETCHR;
  return ciov(channel);
}

unsigned char getrec(unsigned char channel, char* buf, unsigned short len)
{
  OS.iocb[channel].buffer=buf;
  OS.iocb[channel].buflen=len;
  OS.iocb[channel].command=IOCB_GETREC;
  return ciov(channel);
}

unsigned char put(unsigned char channel, char* buf, unsigned short len)
{
  OS.iocb[channel].buffer=buf;
  OS.iocb[channel].buflen=len;
  OS.iocb[channel].command=IOCB_PUTCHR;
  return ciov(channel);
}
