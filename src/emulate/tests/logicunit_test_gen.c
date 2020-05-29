#include <stdio.h>
//gcc logicunit_test_gen.c && ./a.out | xclip -selection c
int main(void) {
  char* ops[] = {"and", "eor", "sub", "rsb", "add", "orr", "mov", "lsl", "lsr", "asr", "ror"};

  for (int i = 0; i < 11; i++) {
    // test values are wrong, need to specify manually, use vscode to
    // autoformat.
    printf(
        "void test_%s() {"
        "int op1, op2;"
        "word_t result, expected;"
        "bool carry;"
        "char *name;"

        "/* Test %s */"
        "name = \"%s: %s can be computed correctly\";"
        "op1 = 0b10011100011111100110100111001001;"
        "op2 = 0b110010100100101110101011;"
        "expected = 0b00000000010010100100100110001001;"
        "%s(op1, op2, &carry, &result);"
        "testword(result, expected, name);"

        "op1 = 0;"
        "op2 = 0;"
        "expected = 0;"
        "%s(op1, op2, &carry, &result);"
        "testword(result, expected, name);"

        "name = \"%s: %s sets carry correctly\";"
        "testbool(!carry, name);"
        "printf(\"\\n\");}\n\n",
        ops[i], ops[i], ops[i], ops[i], ops[i], ops[i], ops[i], ops[i]);
  }

	printf ("int main(void) {\n");
	for (size_t i = 0; i < 11; i++)
	{
		printf("test_%s();\n", ops[i]);
	}
	printf ("return 0;}\n");

	
}