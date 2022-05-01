/**
 * Network Testing tools
 *
 * ncopy - copy files to/from N:
 *
 * Author: Thomas Cherryhomes
 *  <thom.cherryhomes@gmail.com>
 *
 * Released under GPL 3.0
 * See COPYING for details.
 */

#include <atari.h>
#include <string.h>
#include <stdlib.h>
#include <peekpoke.h>
#include "sio.h"
#include "conio.h"
#include "nsio.h"
#include "misc.h"

unsigned char _nsio(unsigned char dunit, unsigned char dcomnd, unsigned char dstats, void *dbuf, unsigned short dbyt, unsigned short daux)
{
  OS.dcb.ddevic=0x71;
  OS.dcb.dunit=dunit;
  OS.dcb.dcomnd=dcomnd;
  OS.dcb.dstats=dstats;
  OS.dcb.dbuf=dbuf;
  OS.dcb.dtimlo=0x1f;
  OS.dcb.dbyt=dbyt;
  OS.dcb.daux=daux;
  return siov();
}

unsigned char nopen(unsigned char unit, char* buf, unsigned char aux1)
{
  return _nsio(unit, 'O', 0x80, buf, 256, aux1);
}

unsigned char nclose(unsigned char unit)
{
  return _nsio(unit, 'C', 0x00, NULL, 0, 0);
}

unsigned char nread(unsigned char unit, char* buf, unsigned short len)
{
  return _nsio(unit, 'R', 0x40, buf, len, len);
}

unsigned char nwrite(unsigned char unit, char* buf, unsigned short len)
{
  return _nsio(unit, 'W', 0x80, buf, len, len);
}

unsigned char nstatus(unsigned char unit)
{
  return _nsio(unit, 'S', 0x40, OS.dvstat, 4, 0);
}

unsigned char ncd(unsigned char unit, char* buf)
{
  return _nsio(unit, 0x2C, 0x80, buf, 256, 0);
}

unsigned char npwd(unsigned char unit, char* buf)
{
  return _nsio(unit, 0x30, 0x40, buf, 256, 0);
}

void print_nerror(unsigned char unit)
{
  nstatus(unit);
  print_error(OS.dvstat[3]);
}
