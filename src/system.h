/*******************************************************
 * system.h - Global functions prototypes and some
 * definitions
 *
 * Written by Ashton Fagg (ashton@fagg.id.au)
 * for the Simpnix kernel.
 ******************************************************/

#ifndef SYSTEM_H
#define SYSTEM_H

typedef int size_t;

/* This is the register structure */
typedef struct reg_state
{
  unsigned int gs; /* General purpose segment */
  unsigned int fs; /* General purpose segment */
  unsigned int es; /* Extra segment */
  unsigned int ds; /* Data segment */
  unsigned int cs; /* Code segment */
  unsigned int ss; /* Stack segment */

  /* General purpose registers */
  unsigned int eax, ebx, ecs, ecx, ebp, edi, esi, esp;
  unsigned int eip, e_flags, user_esp;
} reg_state_t;

/* This is memset and memcpy...pretty standard stuff */
extern void *_memcpy(void *dst, void *src, size_t count);
extern void *_memset(void *dst, char val, size_t count);

extern unsigned short *_memsetw(unsigned short *dst,
                               unsigned short val,
                               size_t count);


/* strlen -- also pretty standard */
extern size_t _strlen(const char *s);

/* Use this to read/write from/to ports */
extern unsigned char read_byte(unsigned short paddr);
extern unsigned short write_word(unsigned short paddr);
extern void write_byte(unsigned short paddr, unsigned char val);



#endif
