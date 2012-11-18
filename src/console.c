/*************************************************************************
 * console.c -- Console control routines
 * Written by Ashton Fagg (ashton@fagg.id.au) for the Simpnix kernel.
 ************************************************************************/

#include "console.h"
#include "system.h"

unsigned short *vid_mem_ptr = (unsigned short *) VID_MEM;

/* Cursor position markers */
unsigned char CURSOR_X = 0;
unsigned char CURSOR_Y = 0;

/* This updates the cursor position physically on the screen. Thus, every time
 * we do something to change the cursor position, we MUST call this to show the
 * change on the screen. */
static void cursor_update(void)
{
  unsigned short location = CURSOR_Y * SCREEN_WIDTH + CURSOR_X;
  /* First we tell the VGA controller we're setting the high cursor byte */
  write_byte(0x3d4, 14);
  write_byte(0x3d5, location >> 8); /* Shift to the high byte */
  /* Now set the low byte */
  write_byte(0x3d4, 15);
  write_byte(0x3d5, location);
}

/* This scrolls the characters up a row on the screen if we've hit the bottom
 * row. This is quite simple to do, as we just assume that everything that was
 * at the top gets rolled off, and we stick a blank row on the bottom */
static void console_scroll(void)
{
  /* Check to see if we've hit the end */
  if (CURSOR_Y >= SCREEN_HEIGHT) {
    int i;
    
    /* Move everything up by one line */
    for (i = 0; i < (SCREEN_HEIGHT - 1) * SCREEN_WIDTH; i++) {
      vid_mem_ptr[i] = vid_mem_ptr[i + SCREEN_WIDTH];
    }

    /* Set up a blank character */
    unsigned char space = 0x20 | (DEFAULT_ATTR << 8);
    for (i = (SCREEN_HEIGHT - 1) * SCREEN_WIDTH; i < SCREEN_SZ; i++) {
      vid_mem_ptr[i] = space;
    }
    /* Set the cursor to the last line */
    CURSOR_Y = SCREEN_HEIGHT - 1;
  }
}

/* Put a single character on the screen */

void console_putchar(char c)
{
  unsigned short attr = DEFAULT_ATTR << 8;
  unsigned short *pos;

  /* This controls what to do with the character, depending on what type of
   * character it is. These are the states:
   * Backspace
   * Tab
   * Carriage return
   * New line
   * Default - this handles all other printable characters
   */
  switch (c) {
    /* Backspace -- move the cursor back by one x position if necessary */
    case (0x08):
      if (CURSOR_X) CURSOR_X--;
    break;
    /* Tab -- moves the cursor forward to the new position */
    case (0x09):
      CURSOR_X = (CURSOR_X + DEFAULT_TAB_SZ) & ~(DEFAULT_TAB_SZ - 1);
    break;
    /* Carriage return -- go back to start of the line */
    case ('\r'):
      CURSOR_X = 0;
    break;
    /* New line -- go back to start horizontally, and then down one line */
    case ('\n'):
      CURSOR_X = 0;
      ++CURSOR_Y;
    break;
    /* This is any other printable character */
    default:
      pos = vid_mem_ptr + (CURSOR_Y * SCREEN_WIDTH + CURSOR_X);
      *pos = c | attr;
      ++CURSOR_X;
    break;
  }
  /* Now we need to check if we've exceeded the horizontal limits of the
   * screen. So we check to see if we're at or past the width, and then go down
   * to the next line and back to the beginning if we are */
  if (CURSOR_X >= SCREEN_WIDTH) {
    CURSOR_X = 0;
    ++CURSOR_Y;
  }

  /* Same again for the vertical limits, but we use the scroll function defined
   * further up for this */
  console_scroll();

  /* Finally, we physically update the screen */
  cursor_update();
}

/* This clears the screen */
void console_clr(void)
{
  int i;
  unsigned char space = 0x20 | (DEFAULT_ATTR << 8);
  for (i = 0; i < SCREEN_SZ; i++) vid_mem_ptr[i] = space;
  CURSOR_X = 0;
  CURSOR_Y = 0;
  cursor_update();
}

/* Write a null terminated ASCII string to the console */
void console_putstr(char *c)
{
  int pos = 0;
  while (c[pos]) console_putchar(c[pos++]);
}

/* Write a hex value to the screen */
void console_puthex(unsigned int c)
{
  signed int t;
  int i;
  unsigned char nz = 1;

  /* tack 0x onto the beginning */
  console_putstr("0x");

  for (i = 28; i > 0; i -= 4) {
    t = (c >> i) & 0xf;
    if (t == 0 && nz != 0) continue;
    if (t >= 0xa) {
      nz = 0;
      console_putchar(t-0xa+'a');
    }
    else {
      nz = 0;
      console_putchar(t+'0');
    }
  }
  t = c & 0xf;
  if (t >= 0xa) console_putchar(t-0xa+'a');
  else console_putchar(t+'0');
}

/* Write a decimal (base 10) value to the screen */
void console_putdec(unsigned int c)
{
  /* If we've got 0, just print 0 and return */
  if (c == 0) {
    console_putchar('0');
    return;
  }
  /* Otherwise ... */
  signed int a = c;
  char b[32];
  int i = 0;
  while (a > 0) {
    b[i] = '0' + a%10;
    a = a / 10;
    i++;
  }
  b[i] = 0;
  char b2[32];
  b2[i--] = 0;
  int j = 0;
  while (i >= 0) b2[i--] = b[j++];
  console_putstr(b2);
}
