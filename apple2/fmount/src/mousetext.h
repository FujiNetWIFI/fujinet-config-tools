/**
 * Mousetext routines
 * needs apple2enh
 *
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3
 */

#include <stdbool.h>

#ifndef MOUSETEXT_H
#define MOUSETEXT_H

/**
 * @brief Toggle MouseText mode
 * @param t true = enable, false = disable
 */
void mousetext(bool t);

/**
 * @brief display folder icon at cursor position
 */
void folder(void);

/**
 * @brief creates screen with header/footer
 * @param h Header text
 * @param f Footer text
 */
void screen(const char *h, const char *f);

/**
 * @brief draw horizontal line
 * @param y position
 */
void line(unsigned char y);

#endif /* MOUSETEXT_H */
