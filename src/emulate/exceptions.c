/* 
 * Implementation for exceptions handling.
 * The majority of exceptions are not handled as it's not required by the spec. But skeletons
 * for exception handling are provided so it can be easily implemented if needed.
 */

#include <stdio.h>
#include "state.h"
#include "exceptions.h"

static void memory_out_of_bound(address_t addr);

void exceptions(byte_t error_code, address_t addr)
{
  switch (error_code)
  {
  case REGISTER_INDEX_OUT_OF_BOUND:
    fprintf(stderr, "Illegal register access at register %d. ", addr);
    break;
  case MEMORY_INDEX_OUT_OF_BOUND:
    memory_out_of_bound(addr);
    return;
  case UNKNOWN_ALU_OPCODE:
    fprintf(stderr, "Unknown ALU opcode received at 0x%08x\n", addr - 8);
    break;
  case UNKNOWN_SHIFT_TYPE:
    fprintf(stderr, "Unknown shift type received at 0x%08x\n", addr - 8);
    break;
  case UNKNOWN_INSTRUCTION_TYPE:
    fprintf(stderr, "Unknown instruction received at 0x%08x\n", addr - 4);
    break;
  case RUN_TIME_INSTRUCTION_ERROR:
    fprintf(stderr, "Unknown instruction at runtime at 0x%08x\n", addr - 8);
    break;
  default:
    fprintf(stderr, "Unhandled exception! Error code: %d.\n", error_code);
  }

  fprintf(stderr, "Exit the program.\n");
  free_state();
  exit(EXIT_FAILURE);
}

/*
 * Handles memory out of bound excpetion.
 * The program continues running as required by the test cases.
 * @param addr: address of memory that the exception takes place.
 */
static void memory_out_of_bound(address_t addr)
{
  fprintf(stdout, "Error: Out of bounds memory access at address 0x%08x\n", addr);
}