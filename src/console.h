/*****************************************************************
 * console.h -- Basic monitor control routine prototypes
 * Written by Ashton Fagg (ashton@fagg.id.au) for the Simpnix
 * kernel.
 *****************************************************************/

#ifndef CONSOLE_H
#define CONSOLE_H

#include "system.h"

/* Write a single character to the screen */
void console_putchar(char c);

/* Write a null terminated string to the console */
void console_putstr(char *c);

/* Write a hexadecimal character to the console */

void console_puthex(unsigned int c);

/* Write a decimal (i.e. base 10) character to the console */
void console_putdec(unsigned int c);

/* Clear the screen */
void console_clr(void);

/* These are some useful constants */
#define BG_COL 0
#define FG_COL 15
#define VID_MEM 0xb8000

static const int SCREEN_WIDTH = 80;   /* Number of columns on screen */
static const int SCREEN_HEIGHT = 25;  /* Number of rows on screen */
static const int SCREEN_SZ = 80 * 25; /* Total characters on screen */

/* This is the default attribute byte for the colours on the console */
static const unsigned char DEFAULT_ATTR = (BG_COL << 4) | (FG_COL & 0x0f);

static const unsigned char DEFAULT_TAB_SZ = 8; /* Default tab size */
#endif
