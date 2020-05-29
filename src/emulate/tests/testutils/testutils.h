/*
 * This testutils module is originally written by Duncan White.
 * It has been adapted to provide extra functionalities.
 * More infomation on this module can be found at:
 * https://www.doc.ic.ac.uk/~dcw/c-tools-2020/lecture2/
 */

extern void testbool( bool condition, char * testname );
extern void testint( int got, int expected, char * testname );
extern void testlong( long got, long expected, char * testname );
extern void testdouble( double got, double expected, char * testname );
extern void teststring( char * got, char * expected, char * testname );
extern void testword( uint32_t got, uint32_t expected, char *testname);
