/**
 * FujiNet Tools for CLI
 *
 * fesclk - Set SIO Clock rate to <kHz>
 *
 * usage:
 *  fesclk <kHz>
 *
 * Author: Thomas Cherryhomes
 *  <thom.cherryhomes@gmail.com>
 *
 * Released under GPL, see COPYING
 * for details
 */

#include <atari.h>
#include <string.h>
#include <stdlib.h>
#include "conio.h"
#include "err.h"
#include "fn_io.h"

unsigned char buf[40];
unsigned short kHz;

/**
 * SIO command to set R: listening kHz
 */
unsigned char fesclk(unsigned short kHz)
{
  fn_io_set_sio_external_clock(kHz);
  if (OS.dcb.dstats != 1)
    err_sio();

  return OS.dcb.dstats;
}

/**
 * show options
 */
void opts(char *argv[])
{
  print(argv[0]);
  print(" <kHz>\x9b\x9b");
  print("<kHz> - Rate in kHz\x9b");
}

/**
 * main
 */
int main(int argc, char *argv[])
{
  unsigned char err = 0;
  char ratestr[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  OS.lmargn = 2;

  if (_is_cmdline_dos())
  {
    if (argc < 2)
    {
      opts(argv);
      return (1);
    }
    kHz = atoi(argv[1]);
  }
  else
  {
    // DOS 2.0/MYDOS
    print("\x9b");

    print("EXTERNAL SIO CLOCK RATE--kHz?\x9b");
    get_line(buf, sizeof(buf));
    kHz = atoi(buf);
  }

  if (kHz == 0)
  {
    print("DISABLING CLOCK.\x9b");
  }
  else
  {
    itoa(kHz, ratestr, 10);
    print("SETTING CLOCK TO ");
    print(ratestr);
    print(" kHz.\x9b\x9b");
  }

  err = fesclk(kHz);

  if (!_is_cmdline_dos())
  {
    print("\x9bPRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
    get_line(buf, sizeof(buf));
  }

  return err == 1 ? 0 : err;
}