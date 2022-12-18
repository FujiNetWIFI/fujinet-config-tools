/**
 * Show FujiNet Adapter Config
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <apple2.h>
#include <conio.h>
#include <stdio.h>
#include <stdint.h>
#include <peekpoke.h>
#include "sp.h"

#define GET_ADAPTER_CONFIG 0xe8

int8_t fuji; // The FujiNet device #

void io_boot(void)
{
  char ostype;

  ostype = get_ostype() & 0xF0;

  if (ostype == APPLE_IIIEM)
  {
    asm("STA $C082");  // disable language card (Titan3+2)
    asm("LDA #$77");   // enable A3 primary rom
    asm("STA $FFDF");
    asm("JMP $F4EE");  // jmp to A3 reset entry
  }
  else  // Massive brute force hack that takes advantage of MMU quirk. Thank you xot.
  {
    POKE(0x100,0xEE);
    POKE(0x101,0xF4);
    POKE(0x102,0x03);
    POKE(0x103,0x78);
    POKE(0x104,0xAD);
    POKE(0x105,0x82);
    POKE(0x106,0xC0);
    POKE(0x107,0x6C);
    POKE(0x108,0xFC);
    POKE(0x109,0xFF);

    asm("JMP $0100");
  }
}


void reset(void)
{
  sp_control(fuji,0xFF); // Do a reset.
  printf("FUJINET RESET. PRESS ANY KEY TO REBOOT.");
  cgetc();

  io_boot();
}

void main(void)
{
  cursor(1);
  sp_init();
  fuji = sp_find_fuji();

  if (fuji == 0)
      printf("\nCOULD NOT FIND FUJINET.\n");
  else
    reset();

  printf("PRESS ANY KEY TO CONTINUE.");
  cgetc();  
}
