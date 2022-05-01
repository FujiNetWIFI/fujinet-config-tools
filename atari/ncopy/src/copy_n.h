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

#ifndef COPY_N_TO_D_H
#define COPY_N_TO_D_H

/**
 * Copy file from N: to CIO
 * @param dest_is_n Destination is Nx:
 * @return error code
 */
int copy_n(bool dest_is_n);

#endif /* COPY_N_TO_D_H */
