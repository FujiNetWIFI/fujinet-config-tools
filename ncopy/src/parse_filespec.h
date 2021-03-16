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

#ifndef PARSE_FILESPEC_H
#define PARSE_FILESPEC_H

/**
 * Parse the filespec pointed by buf
 * @param The filespec buf.
 * @return true if successful
 */
bool parse_filespec(char* buf);

#endif /* PARSE_FILESPEC_H */
