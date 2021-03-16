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
 */
void print_error(void);

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

#endif /* MISC_H */
