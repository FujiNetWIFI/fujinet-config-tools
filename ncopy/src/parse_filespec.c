/**
 * Network Testing tools
 *
 * ncopy - copy files 
 *  N:<->D: D:<->N: or N:<->N:
 *
 * Author: Thomas Cherryhomes
 *  <thom.cherryhomes@gmail.com>
 *
 * Released under GPL 3.0
 * See COPYING for details.
 */

#include <stdbool.h>
#include <string.h>
#include "conio.h"

extern unsigned char sourceDeviceSpec[256];
extern unsigned char destDeviceSpec[256];
extern unsigned char sourceUnit, destUnit;

bool parse_filespec(char* buf)
{
  char *pToken;
  
  // Find comma.
  pToken=strtok(buf,",");
  
  if (pToken==NULL)
    {
      print("NO COMMA\x9b");
      return false;
    }

  strcpy(sourceDeviceSpec,pToken);
  pToken=strtok(NULL,",");

  // Skip over whitespace
  while (*pToken==0x20) { pToken++; }
  
  strcpy(destDeviceSpec,pToken);

  // Put EOLs on the end.
  sourceDeviceSpec[strlen(sourceDeviceSpec)]=0x9B;
  destDeviceSpec[strlen(destDeviceSpec)]=0x9B;

  // Check for valid device name chars
  if (sourceDeviceSpec[0]<0x41 || sourceDeviceSpec[0]>0x5A)
    return false;
  else if (destDeviceSpec[0]<0x41 || destDeviceSpec[0]>0x5A)
    return false;

  // Check for proper colon placement
  if (sourceDeviceSpec[1]!=':' && sourceDeviceSpec[2]!=':')
    return false;
  else if (destDeviceSpec[1]!=':' && destDeviceSpec[2]!=':')
    return false;

  // Try to assign unit numbers.
  if (sourceDeviceSpec[1] != ':')
    sourceUnit=sourceDeviceSpec[1]-0x30;
  if (destDeviceSpec[1] != ':')
    destUnit=destDeviceSpec[1]-0x30;
  
  return true;
}
