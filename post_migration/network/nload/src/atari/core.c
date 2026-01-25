/**
 * @brief   Network Load File
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 */

#include <6502.h>
#include <atari.h>
#include <string.h>
#include <stdbool.h>
#include <fujinet-network.h>
#include <peekpoke.h>
#include "cio.h"
#include "conio.h"
#include "put_error.h"

extern unsigned char _LMARGN_save;
extern unsigned char _LSTACK_RUN__[];
extern unsigned char _LSTACK_LOAD__[];
extern unsigned char _LSTACK_SIZE__[];
extern unsigned char _LOADER_RUN__[];
extern unsigned char _LOADER_LOAD__[];
extern unsigned char _LOADER_SIZE__[];

extern void load_app(void);

bool nuke_memory=false;

void fix_vseror(void)
{
    unsigned char *p = (unsigned char *)0xE000;
    unsigned int i=0;

    if (_dos_type != 5) // Skip if not Atari DOS, not needed.
	    return;

    // This whole routine exists because DOS 2 (AND ONLY DOS 2!) for some reason re-vectors VSEROR when DUP starts.
    // God only knows why. It only affects us because we purge memory when our loader starts up.
    // Thanks to JB for pointing this out.

    // Find VSERIN in ROM
    while (p < (unsigned char *)0xFFFF)
    {
        if (*p++ = 0x98 && *p++ == 0x48 && *p++ == 0xAD && *p++ == 0x0F)
        {
            OS.vserin = (unsigned char *)p-4;
        }
    }

    // Find VSEROR in ROM
    p=(unsigned char *)0xE000;

    while (p < (unsigned char *)0xFFFF)
    {
        if (*p++ == 0x98 && *p++ == 0x48 && *p++ == 0xe6 && *p++ == 0x32)
        {
            OS.vseror = (unsigned char *)p-4;
        }
    }
}

void load(void)
{
    fix_vseror(); // STUPID STUPID STUPID DOS 2!

    OS.dvstat[0] =
        OS.dvstat[1] =
        OS.dvstat[2] =
        OS.dvstat[3] = 0x00;

    network_open((char *)OS.lbuff, 4, 0);

    if (OS.dvstat[3]>1)
    {
        network_close((char *)OS.lbuff);
        memset(OS.lbuff,0x00,sizeof(OS.lbuff));
        put_error(OS.dvstat[3]);

        // This is silly, but...
        memset(&OS.lbuff[5],0x00,1);

        print("ERROR-   ");
        print((char *)&OS.lbuff[1]);
        return;
    }

    // Move loader into low memory
    memcpy(_LSTACK_RUN__, _LSTACK_LOAD__, _LSTACK_SIZE__);
    memcpy(_LOADER_RUN__, _LOADER_LOAD__, _LOADER_SIZE__);

    // load the program
    load_app();
}

/**
 * @brief Shown if in ATARI DOS 3
 */
void dos3_clear(void)
{
    OS.rowcrs=9;
    OS.colcrs=2;

    print("\xCE\xE5\xF4\xF7\xEF\xF2\xEB\xA0\xC2\xE9\xEE\xE1\xF2\xF9\xA0\xCC\xEF\xE1\xE4\x9b\x9b");
    print("\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c\x9c");
}


bool get_filename(int argc,char **argv)
{
    unsigned char *s = &OS.lbuff[0];
    unsigned char *d = &OS.lbuff[3];
    unsigned char i=0;

    if (PEEK(0x715) == 0x38)
        dos3_clear();

    memset(OS.lbuff,0,sizeof(OS.lbuff));

    OS.lmargn = _LMARGN_save; // cc65 monkeys with this. stop it.

    if (_is_cmdline_dos() && argv[1])
    {
        unsigned char start_arg = 1;

        if (!strcasecmp(argv[1],"/X"))
        {
            nuke_memory=true;
            start_arg++;
        }

        // We are abusing that the pointer *argv points to is contiguous for 64 bytes.
 	for (i=start_arg;i<argc;i++)
		strcat((char *)OS.lbuff,argv[i]);
    }
    else
    {
        print("CLEAR MEMORY BEFORE LOAD? (Y/N)\x9B");
        get_line((char *)OS.lbuff,sizeof(OS.lbuff));

        if (!nuke_memory)
            if (OS.lbuff[0] == 'Y' || OS.lbuff[0] == 'y')
                nuke_memory=1;

        print("NETWORK LOAD FROM WHAT FILE?\x9B");
        get_line((char *)OS.lbuff,sizeof(OS.lbuff));
    }

    if (OS.lbuff[0]==0x9B)
        return false;

    if (OS.lbuff[0]=='N' && OS.lbuff[1] == ':')
        return true;

    if (OS.lbuff[0]=='N' && OS.lbuff[2] == ':')
        return true;

    // We only got a filename, shift devicespec over and stick N1: there.
    memmove(d,s,124);
    OS.lbuff[0] = 'N'; OS.lbuff[1] = '1'; OS.lbuff[2] = ':';

    // We're good.
    return true;
}
