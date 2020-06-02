/*
 * Implementation of the print_state function.
 */

#include "print.h"

#include <stdio.h>

void print_state(FILE *fp) {
  fprintf(fp, "Registers:\n");
  for (int i = 0; i < 13; i++) {
    fprintf(fp, "$%-3u: %10d (0x%08x)\n", i, get_reg(i), get_reg(i));
  }
  fprintf(fp, "PC  : %10d (0x%08x)\n", get_reg(PC), get_reg(PC));
  fprintf(fp, "CPSR: %10d (0x%08x)\n", get_reg(CPSR), get_reg(CPSR));
  fprintf(fp, "Non-zero memory:\n");
  for (int i = 0; i<MEM_ADDR>> 2; i++) {
    word_t value = get_word(i << 2);
    /* swaps back to little endian */
    if (value)
      fprintf(fp, "0x%08x: 0x%08x\n", i << 2, __builtin_bswap32(value));
  }
}