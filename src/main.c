/******************************************************
 * main.c -- Kernel main routine
 * Written by Ashton Fagg (ashton@fagg.id.au) for the 
 * Simpnix kernel
 ******************************************************/

#include "system.h"
#include "console.h"

int main(struct multiboot *mboot_ptr)
{
  console_clr();
  console_putstr("Welcome to Simpnix, by Ashton Fagg (ashton@fagg.id.au).\n");
  console_putstr("Starting up...\n");
  for (;;);
  return 0x0b00b135;
}
