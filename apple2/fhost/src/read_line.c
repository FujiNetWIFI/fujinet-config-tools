/**
 * @brief   Line Input routine
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Gosh I love writing these things, really, I do... #@(%#@)_%@
 */

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <conio.h>
#include <apple2.h>

#define CH_DEL 0x7F

/**
 * @brief read an input line.
 * @param s The output buffer. Must be at least len bytes.
 * @param len Allowable # of characters.
 */
void read_line(char *s, uint16_t len)
{
    uint16_t i = 0;
    uint8_t c;

    cursor(1);

    --len;
    do {
      gotox(i);

      c = cgetc();

      if (isprint(c)) {
        if (i < len) {
            putchar(c);
            s[i++] = c;
        }
      }
      else if ((c == CH_CURS_LEFT) || (c == CH_DEL)) {
        if (i) {
          putchar(CH_CURS_LEFT);
          putchar(' ');
          putchar(CH_CURS_LEFT);
          --i;
        }
      }
    } while (c != CH_ENTER);
    putchar('\n');
    s[i] = '\0';

    cursor(0);
}
