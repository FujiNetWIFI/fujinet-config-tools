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

#ifndef MISC_H
#define MISC_H

#include <stdbool.h>

/**
 * Print error passed by errnum
 * @param err error number
 */
void print_error(unsigned char err);

/**
 * Detect wildcard
 * @param buf - the devicespec buffer
 * @return true if wildcard present, otherwise false.
 */
bool detect_wildcard(char* buf);

/**
 * Return TRUE if valid network device
 * @param d device character
 * @return true f 'N'
 */
bool valid_network_device(char d);

/**
 * Return TRUE if valid CIO device
 * @param d device character
 * @return true if letter that isn't 'N'
 */
bool valid_cio_device(char d);

/**
 * CIO call wrapper
 * @param channel IOCB
 * @param command CIO command
 * @param aux1 The AUX1 value
 * @param buf the buffer pointer to put into ICBAL/ICBAH
 * @param len the length of the buffer to put into ICBLL/ICBLH
 */
unsigned char _cio(unsigned char channel, unsigned char command, unsigned char aux1, char* buf, unsigned short len);

#endif /* MISC_H */
