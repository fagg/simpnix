/**************************************************************
 * system.c - Global function implementations. Also see
 * system.h.
 *
 * Written by Ashton Fagg (ashton@fagg.id.au) for the Simpnix
 * kernel.
 *************************************************************/

#include "system.h"

void *_memcpy(void *dst, void *src, size_t count)
{
  const char *src_pointer = (const char*) src;
  char *dst_pointer = (char*) dst;

  while (count != 0) {
    *dst_pointer++ = *src_pointer++;
    count--;
  }
  return dst;
}

void *_memset(void *dst, char val, size_t count)
{
  char *dst_pointer = (char*) dst;
  while (count != 0) {
    *dst_pointer++ = val;
    count--;
  }
  return dst;
}

unsigned short *_memsetw(unsigned short *dst,
                         unsigned short val, 
                         size_t count)
{
  char *dst_pointer = (char*) dst;
  while (count != 0) {
    *dst_pointer++ = val;
  }
  return dst;
}

size_t strlen(const char *s)
{
  size_t ret = 0;
  while (*s != '\0') {
    ret++;
    s++;
  }
  return ret;
}

unsigned char read_byte(unsigned short paddr)
{
  unsigned char ret;
  asm volatile ("inb %1, %0" : "=a" (ret) : "dN" (paddr));
  return ret;
}

unsigned short read_word(unsigned short paddr)
{
  unsigned short ret;
  asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (paddr));
  return ret;
}

void write_byte(unsigned short paddr, unsigned char val)
{
  asm volatile ("outb %1, %0" : : "dN" (paddr), "a" (val));
}
