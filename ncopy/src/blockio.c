/**
 * Simple CIO GETCHR/PUTCHR wrappers
 */

#include <atari.h>
#include <string.h>
#include <stdlib.h>
#include "blockio.h"
#include "cio.h"
#include "misc.h"

unsigned char open(unsigned char channel, unsigned char aux1, char* buf, unsigned short len)
{
  return _cio(channel, IOCB_OPEN, aux1, buf, len);
}

unsigned char close(unsigned char channel)
{
  return _cio(channel, IOCB_CLOSE, 0, NULL, 0);
}

unsigned char get(unsigned char channel, char* buf, unsigned short len)
{
  return _cio(channel, IOCB_GETCHR, 0, buf, len);
}

unsigned char getrec(unsigned char channel, char* buf, unsigned short len)
{
  return _cio(channel, IOCB_GETREC, 0, buf, len);
}

unsigned char put(unsigned char channel, char* buf, unsigned short len)
{
  return _cio(channel, IOCB_PUTCHR, 0, buf, len);
}
