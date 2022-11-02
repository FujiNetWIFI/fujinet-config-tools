/**
 * FMOUNT - Mount a disk/ddp into device slot
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fujicpm.h>
#include <stdbool.h>
#include "input.h"

enum
  {
   HOST_SLOT,
   SELECT_FILE,
   DEVANCE_DIR,
   DRIVE,
   MOUNT,
   DONE
  } state=HOST_SLOT;

unsigned char selected_host_slot, selected_device_slot;
DirectoryPosition selected_file, pos;
char path[256]="/";
DeviceSlot ds[8];
unsigned char mode;

void home(void)
{
  printf("\x17\x1b" "E");
}

void host_slot(void)
{
  char hs[8][32];

  home();
  
  printf("\nHOST SLOTS\n\n");

  fuji_get_host_slots(hs);

  for (char i=0;i<8;i++)
    {
      printf("[%d]%s\n",i+1,hs[i]);
    }

  printf("\nHOST SLOT? ");
  getc(selected_host_slot);
  
  if ((selected_host_slot < 0x30) || (selected_host_slot > 0x38))
    {
      printf("\nHOST SLOT MUST BE BETWEEN 1 and 8.\n");
      return;
    }

  selected_host_slot -= 0x31;
  
  if (hs[selected_host_slot][0] == 0x00)
    {
      printf("\nHOST SLOT MUST NOT BE EMPTY.\n");
      return;
    }
  
  state=SELECT_FILE;
}

void select_file(void)
{
  char n[6];

  home();

  fuji_mount_host_slot(selected_host_slot);
  printf("OPENING DIRECTORY %s\n\n",path);
  fuji_open_directory(selected_host_slot,path,""); // TODO: ADD FILTER.
  fuji_set_directory_position(pos);

  for (char i=0;i<15;i++)
    {
      char *e = fuji_read_directory_entry(72,0);
      
      if (e[0] == 0x7F)
	break;
      else
	printf("[%5d]%s\n",i+pos,e);
    }
  
  fuji_close_directory();

  printf("\n[Q]UIT [U]P [B]ACK [RETURN]PAGE\n OR SELECT #: ");
  input(n);
  selected_file=atoi(n);

  if (n[0]=='U' || n[0]=='u')
    {
      state=DEVANCE_DIR;
      return;
    }
  else if (n[0]=='Q' || n[0]=='q')
    {
      state=DONE;
      return;
    }
  else if (n[0]=='B' || n[0]=='b')
    {
      pos -= 15;
      if (pos<0)
	pos=0;
      state=SELECT_FILE;
      return;
    }
  else if (n[0]==0x00)
    {
      state=SELECT_FILE;
      pos += 15;
      return;
    }
  
  // Now get full path
  fuji_open_directory(selected_host_slot,path,"");
  fuji_set_directory_position(selected_file);
  strcat(path,fuji_read_directory_entry(248,0));
  fuji_close_directory();

  if (path[strlen(path)-1] == '/')
    state=SELECT_FILE;
  else
    state=DRIVE;
}

void devance_dir(void)
{
  char *e = strrchr(path,'/');
  
  if (e == NULL)
    {
      state = DONE;
      return;
    }

  e--;
  
  while (*e != '/')
    {
      e--;
    }

  e++;

  *e=0x00;

  state=SELECT_FILE;
}

void show_device_slots(void)
{
  for (char i=0;i<4;i++)
    {
      printf("[%d]",i+1);
      if (ds[i].file == 0x00)
	printf("--Empty--\n");
      else
	printf("%s\n",ds[i].file);
    }
}

void drive(void)
{
  char k;

  home();
  
  if (path[0]==0x7f) // User selected invalid #
    {
      printf("FILE NUMBER NOT VALID\n");
      state = SELECT_FILE;
      return;
    }

  printf("\n\nMOUNT\n\n%s\n\nTO DRIVE SLOT:\n\n",path);

  fuji_get_device_slots(ds);

  show_device_slots();
  
  printf("\nSELECT SLOT\n[Q] TO QUIT\n[B]ACK: ");
  getc(selected_device_slot);

  if (selected_device_slot=='Q' || selected_device_slot=='q')
    {	    
      state=DONE;
      return;
    }
  else if (selected_device_slot=='B' || selected_device_slot=='b')
    {
      state=SELECT_FILE;
      return;
    }
  else
    {
      selected_device_slot -= 0x31;
    }

  printf("[R]EAD/[W]RITE? ");
  getc(k);
  mode=(k == 'W' || k == 'w') ? 0x02 : 0x01;
  state=MOUNT;
}

void mount(void)
{
  ds[selected_device_slot].hostSlot=selected_host_slot;
  strncpy(ds[selected_device_slot].file,path,36);
  ds[selected_device_slot].mode=mode;

  fuji_set_device_filename(selected_device_slot,path);
  fuji_put_device_slots(ds);

  fuji_mount_device_slot(selected_device_slot,mode);
  
  home();
  
  show_device_slots();

  state=DONE;
}

void main(void)
{
  while (state!=DONE)
    {
      switch(state)
	{
	case HOST_SLOT:
	  host_slot();
	  break;
	case SELECT_FILE:
	  select_file();
	  break;
	case DEVANCE_DIR:
	  devance_dir();
	  break;
	case DRIVE:
	  drive();
	  break;
	case MOUNT:
	  mount();
	  break;
	case DONE:
	  return;
	}
    }
}
