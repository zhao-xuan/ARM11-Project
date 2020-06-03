#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "global.h"

/*
 * Interface for exceptions handling.
 * @param error_code: error_code for the exception.
 * @param addr: address of memory/register that exception takes place
 * If not a memory/register exception, addr should be the current PC value.
 */
void exceptions(byte_t error_code, address_t addr);

#endif