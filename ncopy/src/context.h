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

#ifndef CONTEXT_H
#define CONTEXT_H

extern char buf[8192];
extern unsigned char sourceUnit;
extern unsigned char destUnit;
extern unsigned short dirPos;
extern char wildcardSpec[256];
extern char wildcardPath[256];
extern char sourceDeviceSpec[256];
extern char destDeviceSpec[256];

#endif /* CONTEXT_H */
