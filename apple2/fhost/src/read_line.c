/**
 * @brief   Line Input routine
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose Gosh I love writing these things, really, I do... #@(%#@)_%@
 */

#include <apple2.h>
#include <conio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "text.h"

#define CH_DEL 0x7F

/**
 * @brief read an input line.
 * @param s The output buffer. Must be at least len bytes.
 * @param len Allowable # of characters.
 * @return last character emitted.
 */
char read_line(char *s, uint16_t len)
{
    uint16_t i=0;
    char c=0;

    // Clear line buffer.
    memset(s,0x00,len);
    
    while (c != CH_ENTER)
    {
        c=inc();

        if ((c == CH_CURS_LEFT) || (c == CH_DEL))
        {
            if (i)
            {
                outc(0x08);
                i--;
            }
        }
        else if (c == CH_ENTER)
        {
            outc(0xA0);
            continue;
        }
        else if (c == CH_CURS_RIGHT)
        {
            continue;
        }
        else if (c < 0x20 || c > 0x7E) // Non printable ignored.
        {
            continue;
        }
        else // print and append character in buffer.
        {
            if ((i+1) > len)
            {
                continue;
            }
            else
            {
                outc(c);
                s[i++] = c;
            }
        }
    }

    return c;
}
