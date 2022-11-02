/**
 * @brief input a line
 * @param c Pointer to char buffer
 */

#include <conio.h>

void input(char *c)
{
  char k=0;
  int pos=0;

  while (k != 0x0A)
    {
      k=cgetc();
      switch(k)
	{
	case 0x03:
	  exit(255);
	  break;
	case 0x08:
	  if (pos>0)
	    {
	      pos--;
	      c[pos]=0x00;
	    }
	  else
	    k=0x00;
	  break;
	case 0x0A:
	  putchar(0x0a);
	  return;
	  break;
	default:
	  c[pos++]=k;
	  break;
	}
      if (k>0)
	putchar(k);
    }
}
