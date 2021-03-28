/**
 * Network Testing tools
 *
 * nlogin - set N: login/password.
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
#include "err.h"

unsigned char i=0;
char buf[8];

unsigned char nlogin(unsigned char unit, char *login, char *password)
{
  OS.dcb.ddevic=0x71;
  OS.dcb.dunit=unit;
  OS.dcb.dcomnd=0xFD;
  OS.dcb.dstats=0x80;
  OS.dcb.dbuf=login;
  OS.dcb.dtimlo=0x1f;
  OS.dcb.dbyt=256;
  OS.dcb.daux=0;
  siov();

  if (OS.dcb.dstats!=1)
    {
      err_sio();
      return OS.dcb.dstats;
    }

  OS.dcb.dcomnd=0xFE;
  OS.dcb.dstats=0x80;
  OS.dcb.dbuf=password;
  siov();

  if (OS.dcb.dstats!=1)
    {
      err_sio();
      return OS.dcb.dstats;
    }

  return 0;
}

void opts(char* argv0)
{
  print(argv0);
  print(" <N[x]:> <login> <password>\x9b\x9b  <N[x]:> The Devicespec\x9b  <login> The login (user name)\x9b  <password>\x9b\x9b");
}

int main(int argc, char* argv[])
{
  char *deviceSpec;
  char *login;
  char *password;
  unsigned char u=1;
  unsigned char err=0;
  
  OS.lmargn=2;
  
  if (_is_cmdline_dos())
    {
      if (argc<4)
	{
	  opts(argv[0]);
	  return 0;
	}

      login = argv[2];
      password = argv[3];
    }
  else
    {
      // DOS 2.0/MYDOS
      deviceSpec = (char *)malloc(256);
      login = (char *)malloc(256);
      password = (char *)malloc(256);
      print("NLOGIN--DEVICE SPEC?\x9b");
      get_line(deviceSpec,128);
      print("\x9bLOGIN?\x9b");
      get_line(login,128);
      print("\x9bPASSWORD?\x9b");
      get_line(password,128);
    }

  // if no device, set a device path.
  if ((deviceSpec[1]!=':') && (deviceSpec[2]!=':'))
    {
      memmove(&deviceSpec[2],&deviceSpec[0],sizeof(deviceSpec)-3);
      deviceSpec[0]='N';
      deviceSpec[1]=':';
    }
  else if (deviceSpec[2]==':')
    u=deviceSpec[1]-0x30;

  err=nlogin(u,login,password);

  if (!_is_cmdline_dos())
    {
      free(deviceSpec);
      free(login);
      free(password);

      print("\x9bPRESS \xD2\xC5\xD4\xD5\xD2\xCE TO CONTINUE.\x9b");
      get_line(buf,sizeof(buf));
    }
  
  return err==1 ? 0 : err;
}
