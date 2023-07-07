/**
 * Show FujiNet Adapter Config
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <apple2enh.h>
#include <stdbool.h>
#include <conio.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "sp.h"
#include "mousetext.h"

#define ITEMS_PER_PAGE 16

#define STATUS_READ_HOST_SLOTS 0xF4
#define STATUS_READ_DRIVE_SLOTS 0xF2

#define CTRL_OPEN_DIRECTORY 0xF7
#define CTRL_READ_DIRECTORY_ENTRY 0xF6
#define CTRL_CLOSE_DIRECTORY 0xF5
#define CTRL_SET_DIRECTORY_POSITION 0xE4

enum {
  INIT,
  HOST_SLOT,
  DIR_SHOW,
  DIR_SELECT,
  DIR_PGUP,
  DIR_PGDN,
  DIR_PARENT,
  DIR_TRAVERSE,
  DRIVE_SLOT,
  DONE,
  FUJINET_NOT_FOUND,
} state = INIT; 

unsigned char fuji;
unsigned char hostSlots[8][32];
unsigned char selected_host_slot;
unsigned short dir_position;
char path[256];
char filter[32];

void init(void)
{
  videomode(VIDEOMODE_80x24);  

  sp_init();
  fuji=sp_find_fuji();

  if (fuji == 0)
    state=FUJINET_NOT_FOUND;
  else
    state=HOST_SLOT;
}

void fujinet_not_found(void)
{
  videomode(VIDEOMODE_40x24);
  clrscr();
  printf("FUJINET NOT FOUND. PRESS A KEY TO CONTINUE.");
  cgetc();
  state=DONE;
}

void clear_path(void)
{
  memset(path,0,sizeof(path));
  memset(filter,0,sizeof(filter));
  strcpy(path,"/");
}

void host_slot(void)
{
  unsigned char i=0, s=0, c=0, m=0;
  
  screen("Select a HOST SLOT","Type a number, or use arrows. Then press Return.");

  sp_status(fuji,STATUS_READ_HOST_SLOTS);
  
  memcpy(hostSlots,sp_payload,256);
  
  while (c != 0x0d)
    {
      gotoxy(0,4);
            
      for (i=0;i<8;i++)
	{
	  m=0;
	  revers(0);
	  
	  cprintf("%d. ",i+1);
	  
	  if (s==i)
	    revers(1);
	  else
	    revers(0);

	  if (hostSlots[i][0] == 0x00)
	    cprintf(" (empty) \r\n\n");
	  else
	    {
	      cprintf(" %s \r\n\n",hostSlots[i]);
	      m++;
	    }
	}

      c = cgetc();

      switch(c)
	{
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	  c -= 0x31;
	  if (hostSlots[c][0] != 0x00)
	    s = c;
	  break;
	case 0x0b:
	case 0x08:
	  if (s > 0)
	    s--;
	  break;
	case 0x0a:
	case 0x15:
	  if (s <= m)
	    s++;
	  break;
	case 0x0d:
	  selected_host_slot = s;
	  dir_position = 0;
	  clear_path();
	  break;
	}
      
    }

  state = DIR_SHOW;
  
}

void dir_show(void)
{
  char h[60];
  unsigned char err;
  unsigned short l = 1 + strlen(path) + 1 + strlen(filter) + 1;
  unsigned char i=0;
  
  strcpy(h,"Select a FILE from: ");
  strcat(h,(const char *)hostSlots[selected_host_slot]);
  screen(h,"Use Arrows, A and Z for Page, F for filter, Delete for Parent. Then press Return.");

  // Perform open
  
  sp_payload[i++] = l & 0xFF;
  sp_payload[i++] = l >> 8;
  sp_payload[i++] = selected_host_slot;
  strcpy((char *)&sp_payload[i++],path);
  i += strlen(path);
  strcpy((char *)&sp_payload[i],filter);

  if (sp_ctrl(fuji,CTRL_OPEN_DIRECTORY))  
    {
      cputs("Could not open directory.");
      cgetc();
      state=INIT;
      return;
    }

  while ((sp_payload[0] != 0x7F) || (j < ITEMS_PER_PAGE))
    {
      i=0;

      // Ask for next directory entry
      
      sp_payload[i++]=2;  // Read dir CONTROL expects 2 bytes.
      sp_payload[i++]=0;
      sp_payload[i++]=78; // 80 - 2 (so we can plop in a folder icon on left)
      sp_payload[i++]=1;  // We do want additional info (so we can detect folder)

      if (sp_ctrl(fuji,CTRL_READ_DIRECTORY_ENTRY))
	{
	  cputs("Could not send read directory entry.");
	  cgetc();
	  state=INIT;
	  return;
	}

      // Retrieve reply
      
      memset(sp_payload,0,sizeof(sp_payload));
      if (sp_status(fuji,STATUS_READ_DIRECTORY_ENTRY))
	{
	  
	}
    }
  
  sp_ctrl(fuji,CTRL_CLOSE_DIRECTORY);
  
}

void done(void)
{
  for(;;);
}

void main(void)
{
  while(true)
    {
      switch(state)
	{
	case INIT:
	  init();
	  break;
	case HOST_SLOT:
	  host_slot();
	  break;
	case DIR_SHOW:
	  dir_show();
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
	  done();
	  break;
	case FUJINET_NOT_FOUND:
	  fujinet_not_found();
	  break;
	}
    }
}
