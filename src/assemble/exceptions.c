/*
 * Implementation for exceptions handling.
 * The majority of exceptions are not handled as it's not required by the spec.
 * But skeletons for exception handling are provided so it can be easily
 * implemented if needed.
 */
#include <stdlib.h>
#include "exceptions.h"

void exceptions(byte_t error_code, address_t current) {
  fprintf("Unhandled exception! Error code %d\n", error_code);
  exit(EXIT_FAILURE);
}
