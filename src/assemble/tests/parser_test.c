#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "testutils.h"
#include "parser.h"

#define to_index(literal) ((int) strtol(literal + 1, NULL, 0))
/*
 * Below are the helper functions in parser.c being tested
 * They are working properly if all tests are passed
 */

// static char *trim_field(char *str);
// static char **operand_processor(const char *operand, int field_count);
// static void parse_dp(assembly_line *line, word_t *bin);
// static void free_tokens(char **tokens);
// static word_t parse_operand2(char *operand2);

// static char *trim_field(char *str) {
//   int i = 0;
//   while (str[i] == ' ' || str[i] == '[') {
//     i++;
//   }
//   int j = strlen(str) - 1;
//   while (!(str[j] - '0' >= 0 && str[j] - '0' <= 9) && !(str[j] - 'a' >= 0 && str[j] - 'a' <= 25)) {
//     j--;
//   }
//   char *trimmed = eMalloc((j - i) * sizeof(char));
//   strncpy(trimmed, str + i, j - i + 1);

//   return trimmed;
// }

// static char **operand_processor(const char *operand, int field_count) {
//   char **tokens = eCalloc(field_count, sizeof(char *));
//   int i = 0;
//   char str[strlen(operand) + 1];
//   for (int j = 0; j < strlen(operand); j++) {
//     str[j] = operand[j];
//   }
//   str[strlen(operand)] = '\0';
//   char *literal = strtok(str, ",");
//   while (literal != NULL && i < field_count) {
//     tokens[i] = eMalloc(strlen(literal) * sizeof(char));
//     strcpy(tokens[i], literal);
//     i++;
//     literal = strtok(NULL, ",");
//   }

//   tokens = eRealloc(tokens, (i + 1) * sizeof(char *));
//   tokens[i] = NULL;

//   return tokens;
// }

// static void free_tokens(char **tokens) {
//   int i = 0;
//   while (tokens[i] != NULL) {
//     free(tokens[i]);
//     i++;
//   }

//   free(tokens);
// }

// static void parse_dp(assembly_line *line, word_t *bin) {
//   byte_t opcode_field = (*bin >> OPCODE_LOCATION) & FOUR_BIT_FIELD;
//   char **tokens = operand_processor(line->operands, 3);
//   if (opcode_field <= 4 || opcode_field == 12) {
//     /* result-computing instruction */
//     *bin |= to_index(tokens[0]) << DP_DT_RD_LOCATION;
//     *bin |= to_index(tokens[1]) << DP_DT_RN_LOCATION;
//     *bin |= parse_dp_operand2(tokens[2]); //parse_dp_operand2 will take care of the immediate bit as well
//   } else if (opcode_field == 13) {
//     /* mov instruction */
//     *bin |= to_index(tokens[0]) << DP_DT_RD_LOCATION;
//     *bin |= parse_dp_operand2(tokens[1]); //parse_dp_operand2 will take care of the immediate bit as well
//   } else if (opcode_field >= 8 && opcode_field <= 10) {
//     /* CPSR flag set instruction */
//     *bin |= to_index(tokens[0]) << DP_DT_RN_LOCATION;
//     *bin |= parse_dp_operand2(tokens[1]); //parse_dp_operand2 will take care of the immediate bit as well
//   }
// }

// static word_t parse_operand2(char *operand2) {
//   word_t bin = 0;
//   if (operand2[0] == '#') {
//     bin |= 1 << IMM_LOCATION;
//     long imm = to_index(operand2);
//     if (imm >= 0 && imm < 128)  {
//       bin |= imm;
//     } else {
//       /* where the right rotation is needed */
//     }
//   } else {
//     char *base_reg = strtok(operand2, ",");
//     bin |= to_index(base_reg);
//     char *shift_name = strtok(NULL, " ");
//     char *shift_name_list[] = {"lsl", "lsr", "asr", "ror"};
//     for (int i = 0; i < 4; i++) {
//       if (strcmp(shift_name_list[i], shift_name)) {
//           bin |= i << OPERAND2_SHIFT_TYPE_LOCATION;
//           break;
//       }
//     }

//     char *shamt_str = strtok(NULL, " ");
//     long shamt = to_index(shamt_str);
//     if (shamt_str[0] == '#') {
//       if (shamt < 32) {
//         bin |= shamt << OPERAND2_INTEGER_SHIFT_LOCATION;
//       } else {
//         /* An error should be thrown: shift integer is not in the range */
//       }
//     } else {
//       bin |= 1 << OPERAND2_SHIFT_SPEC_LOCATION;
//       bin |= shamt << OPERAND2_REGISTER_SHIFT_LOCATION;
//     }
//   }

//   return bin;
// }

int main(void) {
//   /* Testing trim_field() ... */
//   char *name = "Testing if the helper function trim_field works properly";
//   char *trimmed = eMalloc(4 * sizeof(char));
//   trimmed = trim_field(" r1");
//   teststring(trimmed, "r1", name);
//   free(trimmed);
//   trimmed = trim_field("  r3");
//   teststring(trimmed, "r3", name);
//   free(trimmed);
//   trimmed = trim_field("[r2]");
//   teststring(trimmed, "r2", name);
//   free(trimmed);
//   trimmed = trim_field("[r15  ");
//   teststring(trimmed, "r15", name);
//   free(trimmed);
//   trimmed = trim_field("r1,r2,r3");
//   teststring(trimmed, "r1,r2,r3", name);
//   free(trimmed);

//   /* Testing operand_processor */
//   name = "Testing if the operand_processor() works properly";
//   char **tokens1 = operand_processor("r1,r2,r3", 3);
//   teststring(tokens1[0], "r1", name);
//   teststring(tokens1[1], "r2", name);
//   teststring(tokens1[2], "r3", name);
//   free_tokens(tokens1);
//   char **tokens2 = operand_processor("r1,r2,lsl #2", 3);
//   teststring(tokens2[0], "r1", name);
//   teststring(tokens2[1], "r2", name);
//   teststring(tokens2[2], "lsl #2", name);
//   free_tokens(tokens2);
//   char **tokens3 = operand_processor("r0,=0x555555", 2);
//   teststring(tokens3[0], "r0", name);
//   teststring(tokens3[1], "=0x555555", name);
//   free_tokens(tokens3);
//   char **tokens4 = operand_processor("[r4],-r3,lsl #2", 3);
//   teststring(tokens4[0], "[r4]", name);
//   teststring(tokens4[1], "-r3", name);
//   teststring(tokens4[2], "lsl #2", name);
//   free_tokens(tokens4);

//   /* Testing parse_dp ... */
//   name = "testing if parse_dp() and parse_dp_operand2() work properly";
//   /* Assembly Code: add r2,r4,r3 */
//   assembly_line *line = eMalloc(sizeof(assembly_line));
//   line->opcode = "add";
//   line->operands = "r2,r4,r3";
//   mnemonic_p content = get_mnemonic_data(line->opcode);
//   word_t bin = content->bin;
//   parse_dp(line, &bin);
//   testword(bin, 0b11100000100001000010000000000011, name);
//   free(line);
}
