/**
 * Simple conio for E:
 */

#include <atari.h>
#include <string.h>
#include "cio.h"
#include "misc.h"

void print(char* c)
{
  int l=strlen(c);

  _cio(0, IOCB_PUTCHR, 0, c, l);  
}

void get_line(char* buf, unsigned char len)
{
  _cio(0, IOCB_GETREC, 0, buf, len);
}
