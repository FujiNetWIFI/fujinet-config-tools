/**
 * Simple conio for E:
 */

#include <atari.h>
#include <string.h>
#include "cio.h"
#include "misc.h"

void print(const char* c)
{
  int l=strlen(c);

  _cio(0, IOCB_PUTCHR, 12, c, l);  
}

void get_line(char* buf, unsigned char len)
{
  _cio(0, IOCB_GETREC, 12, buf, len);
}
