#include <stdbool.h>

#include "testutils.h"
#include "global.h"
#include "decode.h"
#include "state.h"

static bool compare_imm_value(imm_value_t *expected, imm_value_t *output) {
	return expected->imm == output->imm
		  	 && expected->rotate == output->rotate;
}

static bool compare_register_form(register_form_t *expected, register_form_t *output) {
	return expected->shift_type == output->shift_type
		 	   && expected->rm == output->rm;
}

static bool compare_data_processing(data_processing_t *expected, data_processing_t *output) {
	if (expected->imm_const ^ output->imm_const) return false;  
	bool operand2_decoded_correctly = 
					expected->imm_const 
					? compare_imm_value(&expected->operand2.imm_value, &output->operand2.imm_value)
					: compare_register_form(&expected->operand2.reg_value, &output->operand2.reg_value);
	return operand2_decoded_correctly
					&& expected->rn        == output->rn  
					&& expected->rd        == output->rd 
					&& expected->opcode    == output->opcode
					&& expected->set       == output->set; 
}
static bool compare_multiply(multiply_t *expected, multiply_t *output) {
	return expected->rm           == output->rm  
				&& expected->rd         == output->rd 
				&& expected->rs         == output->rs
				&& expected->accumulate == output->accumulate 
				&& expected->set        == output->set; 
}
static bool compare_data_transfer(data_transfer_t *expected, data_transfer_t *output) {
	return expected->rn           == output->rn  
				&& expected->rd         == output->rd 
				&& expected->imm_offset == output->imm_offset
				&& expected->pre_index  == output->pre_index 
				&& expected->up_bit     == output->up_bit 
				&& expected->load       == output->load
				&& expected->offset     == output->offset;
}
static bool compare_branch(branch_t *expected, branch_t *output) {
	return  expected->offset == output->offset;  
}


static bool compare_instr(instruction_t *expected, instruction_t *output) {
	if ( expected->type != output->type
		 || expected->cond != output->cond ) return false;
	switch (expected->type) {
		case DATA_PROCESSING:
			return compare_data_processing(expected->instructions.data_processing, 
																		 output->instructions.data_processing);
		case MULTIPLY:
			return compare_multiply(expected->instructions.multiply, 
									 						output->instructions.multiply);
		case DATA_TRANSFER:
			return compare_data_transfer(expected->instructions.data_transfer, 
																   output->instructions.data_transfer);
		case BRANCH:
			return compare_branch(expected->instructions.branch, 
														output->instructions.branch);
		default:
			return true;
	}
}

char *generate_test_name(instruction_t *expected) {
	switch (expected->type) {
		case DATA_PROCESSING:
		{
		   data_processing_t *dp_ptr = expected->instructions.data_processing;
		   if (dp_ptr->imm_const) {
			   return "Data Processing instruction with immediate constant, decoded correctly";
		   }
		   else {
			  return "Data Processing instruction with register form, decoded correctly";
		   }
	   }
		case MULTIPLY:
			 return "Multiply instruction decoded correctly";
		case DATA_TRANSFER:
			 return "Data Transfer instuction decoded correctly";
		case BRANCH:
			 return "Branch instruction decoded correctly";
		default:
			 return "Halt instruction decoded correctly";
	}
}

static void print_data_processing(data_processing_t *instr) {
	printf("rn       : %d\n", instr -> rn);
	printf("rd       : %d\n", instr -> rd);
	printf("opcode   : %d\n", instr -> opcode);
	printf("imm_const: %d\n", instr -> imm_const);
	printf("set      : %d\n", instr -> set);

	if (instr->imm_const) {
		printf("imm_value: %d\n", instr->operand2.imm_value.imm);
		printf("shamt    : %d\n", instr->operand2.imm_value.rotate);
	} else {
		printf("shifttype: %d\n", instr->operand2.reg_value.shift_type);
		printf("rm       : %d\n", instr->operand2.reg_value.rm);
	}
}

static void print_data_transfer(data_transfer_t *instr) {
	printf("offset    : %d\n", instr -> offset);
	printf("rn        : %d\n", instr -> rn);
	printf("rd        : %d\n", instr -> rd);
	printf("imm_offset: %d\n", instr -> imm_offset);
	printf("pre_index : %d\n", instr -> pre_index);
	printf("up_bit    : %d\n", instr -> up_bit);
	printf("load      : %d\n", instr -> load);
}

static void print_branch(branch_t *instr) {
	printf("offset: %d\n", instr -> offset);
}

static void print_multiply(multiply_t *instr) {
	printf("rm        : %d\n", instr -> rm);
	printf("rd        : %d\n", instr -> rd);
	printf("rs        : %d\n", instr -> rs);
	printf("rn        : %d\n", instr -> rn);
	printf("accumulate: %d\n", instr -> accumulate);
	printf("set       : %d\n", instr -> set);
}

static void print_decoded(instruction_t *instr) {
	printf("cond: %d\n", instr->cond);
	switch (instr->type) {
		case DATA_PROCESSING:
			print_data_processing(instr->instructions.data_processing);
			break;
		case MULTIPLY:
			print_multiply(instr->instructions.multiply);
			break;
		case DATA_TRANSFER:
			print_data_transfer(instr->instructions.data_transfer);
			break;
		case BRANCH:
			print_branch(instr->instructions.branch);
			break;		
		default:
			break;
	}	
}

void test_instruction(instruction_t *expected, const word_t binary) {
	instruction_t *got = decode(binary);
	char instr_string[30];
	sprintf(instr_string, "\n  Instr in hex: 0x%08x", binary);
	char *partial_test_name = generate_test_name(expected);
	bool test_result = compare_instr(expected, got);
	char test_name[200];
	strcpy(test_name, partial_test_name);
	strcat(test_name, instr_string);
	testbool(test_result, test_name);
	if (!test_result) {
		printf("EXPECTED: \n");
		print_decoded(expected);
		printf("GOT: \n");
		print_decoded(got);    
	}
}

int main() {
	word_t input;
	data_processing_t dp;
	imm_value_t imm_value;
	register_form_t reg_value;
	multiply_t mul;
	data_transfer_t dt;
	branch_t branch;
	instruction_t expected;

	/* 
	 *  Data Processing with immediate constant
	 *  add01
	 *  11100011 10100000 00010000 00000001
	 */
	input = 3818917889;
	expected.type = DATA_PROCESSING;
	expected.cond = 14;
	imm_value = (imm_value_t) {1, 0};
	dp = (data_processing_t) {0, 1, 13, 1, 0};
	dp.operand2.imm_value = imm_value;
	expected.instructions.data_processing = &dp; 
	test_instruction(&expected, input); 

	/* 
	 *  Data Processing with register form
	 *  add04
	 *  11100000 10000001 00110000 00000010
	 */
	input = 3766562818;
	expected.type = DATA_PROCESSING;
	expected.cond = 14;
	reg_value = (register_form_t) {0, 2};
	reg_value.shift.integer_shift = 0;
	dp = (data_processing_t) {1, 3, 4, 0, 0};
	dp.operand2.reg_value = reg_value;
	expected.instructions.data_processing = &dp; 
	test_instruction(&expected, input); 

	/* 
	 *  Data Processing with immediate constatnt
	 *  eor1
	 *  11100011 10100000 00100000 11111111
	 */
	input = 3818922239;
	expected.type = DATA_PROCESSING;
	expected.cond = 14;
	imm_value = (imm_value_t) {0xff, 0};
	dp = (data_processing_t) {0, 2, 13, 1, 0};
	dp.operand2.imm_value = imm_value;
	expected.instructions.data_processing = &dp; 
	test_instruction(&expected, input); 

	/*  Multiply 
	 *  mul01
	 *  11100000 00000011 00000010 10010001
	 */
	input = 3758293649;
	expected.type = MULTIPLY;
	expected.cond = 14;
	mul = (multiply_t) {1, 3, 2, 0, 0, 0};
	expected.instructions.multiply = &mul;
	test_instruction(&expected, input);

	/* Data transfer  
	 *  ldr01 
	 *  11100101 10010000 00100000 00000000 
	 */
	input = 3851427840;
	expected.cond = 14;
	expected.type = DATA_TRANSFER;
	dt = (data_transfer_t) {0, 0, 2, 0, 1, 1, 1};
	expected.instructions.data_transfer = &dt;
	test_instruction(&expected, input);

	/* Branch 
	 *  b01
	 *  11101010 00000000 00000000 00000000
	 */
	input = 3925868544;
	expected.cond = 14;
	expected.type = BRANCH;
	branch = (branch_t) {0};
	expected.instructions.branch = &branch;
	test_instruction(&expected, input);       

	/* Halt */
	input = 0;
	expected.cond = 0;
	expected.type = HALT;
	test_instruction(&expected, input);
	return 0;
}
