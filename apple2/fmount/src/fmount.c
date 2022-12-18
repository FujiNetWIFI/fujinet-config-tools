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
#include "sp.h"

enum {
  HOST_SLOT,
  DIR_SHOW,
  DIR_SELECT,
  DIR_PGUP,
  DIR_PGDN,
  DIR_PARENT,
  DIR_TRAVERSE,
  DRIVE_SLOT,
  DONE
} state = HOST_SLOT; 

void main(void)
{
  switch(state)
    {
    case HOST_SLOT:
      break;
    case DIR_SHOW:
      break;
    case DIR_SELECT:
      break;
    case DIR_PGUP:
      break;
    case DIR_PGDN:
      break;
    case DIR_PARENT:
      break;
    case DIR_TRAVERSE:
      break;
    case DRIVE_SLOT:
      break;
    case DONE:
      break;
    }
}
