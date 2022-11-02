/**
 * TDOS tool to set Time/Date to FujiNet clock
 *
 * @author Thom Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <stdio.h>
#include <string.h>
#include "network.h"

#define FUJI_DEV 0x0F
#define toBCD(n) (((n)/10)*16+((n)%10))

typedef struct _time
{
  unsigned char mday;
  unsigned char mon;
  unsigned char year;
  unsigned char hour;
  unsigned char min;
  unsigned char sec;
} FujiTime;

FujiTime time;
unsigned char bcdtime[6];

unsigned char response[1024];

void main(void)
{
  char gt=0xD2; // Command for Get Time

  // Get time/date from FujiNet
  adamnet_write(&gt,1);
  adamnet_read(response,1024);
  memcpy(time,response,6);

  // Translate to TDOS format
  bcdtime[0]=toBCD(time.year);
  bcdtime[1]=toBCD(time.mon);
  bcdtime[2]=toBCD(time.mday);
  bcdtime[3]=toBCD(time.hour);
  bcdtime[4]=toBCD(time.min);
  bcdtime[5]=toBCD(time.sec);

  // And send off to BDOS
  bdos(42,&bcdtime);

  printf("TIME SET TO:\n%02d-%02d-%02d %02d:%02d:%02d\n",time.year,time.mon,time.mday,time.hour,time.min,time.sec);
  
}
