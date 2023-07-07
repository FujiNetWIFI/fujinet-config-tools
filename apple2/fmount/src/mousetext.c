/**
 * Mousetext routines
 * needs apple2enh
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <apple2enh.h>
#include <stdio.h>
#include <conio.h>
#include "mousetext.h"

/**
 * @brief Toggle MouseText mode
 * @param t true = enable, false = disable
 */
void mousetext(bool t)
{
  if (t==true)
    {
      putchar(0x0F);
      putchar(0x1B);
    }
  else
    {
      putchar(0x18);
      putchar(0x0E);
    }
}

/**
 * @brief display folder icon at cursor position
 */
void folder(void)
{
  mousetext(true);
  putchar(0x58);
  putchar(0x59);
  mousetext(false);
}

/**
 * @brief draw horizontal line
 * @param y position
 */
void line(unsigned char y)
{
  unsigned char i;

  gotoxy(0,y);
  
  for (i=0;i<80;i++)
    putchar(0x53);
}

/**
 * @brief creates screen with header/footer
 * @param h Header text
 * @param f Footer text
 */
void screen(const char *h, const char *f)
{
  clrscr();
  mousetext(true);
  line(1);
  line(22);
  mousetext(false);
  cputsxy(0,0,h);
  cputsxy(0,23,f);
  gotoxy(0,2);
}
