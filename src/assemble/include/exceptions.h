#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "global.h"

/*
 * Interface for exceptions handling.
 * @param error_code: error_code for the exception.
 * @param current: The location where the exception takes place.
 */
void exceptions(byte_t error_code, address_t current);

#endif
