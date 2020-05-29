/*
 * This testutils module is originally written by Duncan White.
 * It has been adapted to provide extra functionalities.
 * More infomation on this module can be found at:
 * https://www.doc.ic.ac.uk/~dcw/c-tools-2020/lecture2/
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


#include "testutils.h"


void testbool( bool condition, char *testname ) {
  printf( "T %s: %s\n", testname, condition?"OK":"FAIL" );
}

void testint( int got, int expected, char *testname ) {
  printf( "T %s (expected=%d, got=%d): %s\n",
	testname, expected, got, expected==got?"OK":"FAIL" );
}

void testlong( long got, long expected, char *testname ) {
  printf( "T %s (expected=%ld, got=%ld): %s\n",
	testname, expected, got, expected==got?"OK":"FAIL" );
}

void testdouble( double got, double expected, char *testname ) {
  printf( "T %s (expected=%g, got=%g): %s\n",
	testname, expected, got, expected==got?"OK":"FAIL" );
}

void teststring( char *got, char *expected, char *testname ) {
  printf( "T %s (expected='%s', got='%s'): %s\n",
	testname, expected, got, strcmp(expected,got)==0?"OK":"FAIL" );
}

void testword( uint32_t got, uint32_t expected, char *testname) {
  printf( "T %s (expected=0x%08x, got=0x%08x): %s\n",
    testname, expected, got, expected==got?"OK":"FAIL");
}

void testbyte( uint8_t got, uint8_t expected, char *testname ) {
    printf( "T %s (expected=0x%08x, got=0x%08x): %s\n",
            testname, expected, got, expected==got?"OK":"FAIL");
}

void testsize( size_t got, size_t expected, char *testname ) {
    printf( "T %s (expected=0x%08x, got=0x%08x): %s\n",
            testname, expected, got, expected==got?"OK":"FAIL");
}