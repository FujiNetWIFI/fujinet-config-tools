/**
 * Create New Disk
 */

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fujicpm.h>
#include <stdio.h>
#include "input.h"

#define READWRITE 2

void home(void)
{
  printf("\x1b" "E");
}

DeviceSlot deviceSlots[8];

unsigned char select_host_slot(void)
{
  char hs[8][32];
  char c=0;
  
  fuji_get_host_slots(hs);

  home();
  printf("\nNEW DISK--HOST SLOTS\n\n");
  for (char i=0;i<8;i++)
    printf("[%u]%s\n",i+1,hs[i]);

  printf("\n");
  
  while (c < 0x30 || c > 0x39)
    {
      printf("SELECT HOST SLOT: ");
      getc(c);
      if (c==0x03)
	{
	  printf("\n\nABORTING\n");
	  exit(0);
	}
    }
  
  return c - '1';
}

unsigned long select_size(void)
{
  char c[10];
  unsigned long l;

  while (l<1)
    {
      home();
      printf("SELECT DISK SIZE\n\n");
      printf("[1]160K\n");
      printf("[2]256K (DDP)\n");
      printf("[3]320K\n");
      printf("[4]718K\n");
      printf("[5]1418K\n");
      printf("[6]8192K\n");
      printf("\n\nSELECT SIZE\nOR # OF BLOCKS: \n");
      input(c);
      l=atol(c);
      
      switch(l)
	{
	case 1:
	  l=160;
	  break;
	case 2:
	  l=256;
	  break;
	case 3:
	  l=320;
	  break;
	case 4:
	  l=718;
	  break;
	case 5:
	  l=1418;
	  break;
	case 6:
	  l=8192;
	  break;
	default:
	  break;
	}
    }
  
  return l;
}

void enter_filename(char *p)
{
  printf("FILENAME: ");
  input(p);
}

void show_device_slots(void)
{
  printf("\nPUT INTO DEVICE SLOT\n\n");
  
  for (char i=0;i<4;i++)
    {
      printf("[%d]",i+1);
      if (deviceSlots[i].file == 0x00)
	printf("--Empty--\n");
      else
	printf("%s\n",deviceSlots[i].file);
    }
}

unsigned char select_device_slot(void)
{
  unsigned char s=0;

  fuji_get_device_slots(deviceSlots);

  while (s < 1)
    {
      home();
      show_device_slots();
      
      printf("\nSELECT DEVICE SLOT: ");
      getc(s);
      s-=0x31;
    }

  return s;  
}

bool are_you_sure(unsigned char hs, unsigned char ds, unsigned long nb, char *p)
{
  bool yn=false;
  char c=0;
  
  printf("HOST SLOT: %u\n",hs+1);
  printf("DISK SLOT: %u\n",ds+1);
  printf(" # BLOCKS: %lu\n",nb);
  printf(" FILENAME: %s\n",p);
  printf("\nARE YOU SURE[Y/N]? ");
  
  getc(c);

  if (c == 'y' || c == 'Y')
    yn=true;
  
  return yn;
}

void main(void)
{
  unsigned char hs, ds;
  unsigned long numBlocks;
  char p[256];
  
  hs=select_host_slot();
  numBlocks=select_size();
  enter_filename(p);
  ds=select_device_slot();

  if (are_you_sure(hs,ds,numBlocks,p))
    {
      printf("CREATING...");
      fuji_mount_host_slot(hs);
      fuji_new_disk(hs,ds,numBlocks,p);

      deviceSlots[ds].mode = READWRITE;
      deviceSlots[ds].hostSlot = hs;
      strncpy(deviceSlots[ds].file,p,36);
      
      fuji_set_device_filename(ds,p);
      fuji_put_device_slots(deviceSlots);
      fuji_mount_device_slot(ds,READWRITE);
      printf("DONE.\n");
    }
}
