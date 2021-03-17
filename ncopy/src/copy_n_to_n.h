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

#ifndef COPY_N_TO_N_H
#define COPY_N_TO_N_H

#include "context.h"

/**
 * Copy file from CIO to N:
 * @param context Context object
 * @return error code
 */
int copy_n_to_n(Context *context);

#endif /* COPY_N_TO_N_H */
