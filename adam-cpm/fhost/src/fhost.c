#include <stdio.h>
#include <stdlib.h>
#include <fujicpm.h>
#include <string.h>
#include "input.h"

unsigned char slot;
char hs[8][32];

void show_host_slots(void)
{  
  for (int i=0;i<8;i++)
    printf("[%u]%s\n",i+1,hs[i]);
  
  printf("\n");
}

void ask(void)
{
  while ((slot<0x31) || (slot>0x38))
    {
      show_host_slots();
      
      printf("HOST SLOT #");
      slot=getch();
      
      if (slot == 0x03)
	{
	  printf("\nABORTING\n");
	  exit(0);
	}

      printf("%c\n",slot);
    }

  slot -= 0x31;
  
  printf("HOSTNAME? ");
  memset(hs[slot],0,32);
  input(hs[slot]);
}

int main(int argc, char *argv[])
{
  fuji_get_host_slots(hs);

  if (argc < 2)
    ask();
  else
    {
      slot = atoi(argv[1]) - 1;
      strncpy(hs[slot],argv[2],32);
    }

  if (argc == 2)
    {
      memset(hs[slot],0,32);
      printf("\nSLOT %u CLEARED\n",slot+1);
    }
  else
    {
      printf("\nSLOT %u SET TO:\n%s\n\n",slot+1,hs[slot]);
    }

  fuji_put_host_slots(hs);
  
  return 0;
}
