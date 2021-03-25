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
#include "parse_filespec.h"
#include "context.h"
#include "misc.h"

extern char sourcePathSeperator;
extern char destPathSeperator;

bool parse_filespec()
{
  char *p;

  p=strchr(buf,0x9b);

  // Remove trailing EOL
  if (p!=NULL)
      *p=0x00;

  // Find comma.
  p=strtok(buf,",");
  
  if (p==NULL)
    {
      print("NO COMMA\x9b");
      return false;
    }

  strcpy(sourceDeviceSpec,p);
 
  p=strtok(NULL,",");

  // Skip over whitespace
  while (*p==0x20) { p++; }

  strcpy(destDeviceSpec,p);
  
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

  sourceUnit=destUnit=1;
  
  // Try to assign unit numbers.
  if (sourceDeviceSpec[1] != ':')
    sourceUnit=sourceDeviceSpec[1]-0x30;
  if (destDeviceSpec[1] != ':')
    destUnit=destDeviceSpec[1]-0x30;

  // Detect the path seperator
  detect_seperator();

  // If no filename on end of seperator, append it from source.
  /* if (destDeviceSpec[strlen(destDeviceSpec)]==0x00) */
  /*     strcat(destDeviceSpec,strrchr(sourceDeviceSpec,sourcePathSeperator)+1); */
  
  // Append EOL to each
  strcat(sourceDeviceSpec,"\x9b");
  strcat(destDeviceSpec,"\x9b");
  
  return true;
}
