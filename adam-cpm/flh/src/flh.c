/**
 * List Host Slots
 */

#include <fujicpm.h>
#include <conio.h>

void main(void)
{
  char hs[8][32];

  cprintf("\nHOST SLOTS\n\n");
  
  fuji_get_host_slots(hs);

  for (char i=0;i<8;i++)
    cprintf("[%d]%s\n",i+1,hs[i]);
}
