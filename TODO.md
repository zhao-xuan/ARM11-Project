# To-do List
## Deadlines and important notice
* Friday June 5th 13:00 BST: Interim Checkpoint Report(pdf)
* Friday June 19th 13:00 BST: Final Report(pdf), Final Master Branch Repo Commit via LabTS, Presentation Slides(pdf), and Video URL(txt)
* Link to our report: https://www.overleaf.com/4289975884mvfmsjfdbbyf
* All reports MUST be in Latex(assessed).
* Remember to submit the pdf on CATe. The pdf reports are also in the doc folder in the repository
* Testing platform: git clone https://gitlab.doc.ic.ac.uk/kgk/arm11_testsuite.git. Ruby has to be installed in order to use the testsuite

## Task Assignment

### Part 1

| Task                               | Name   | Status      |
| ---------------------------------- | ------ | ----------- |
| Binary file loader                 | Daniel | In progress |
| Headers and includes               | Hoang  | In progress |
| Memory and register Implementation | Tom    | In progress |
| Data Processing instructions       |        | Incomplete  |
| Multiply and Branch                |        | Incomplete  |
| Single Data Transfer               |        | Incomplete  |
| Pipeline and integration           |        | Incomplete  |
| Testing                            |        | Incomplete  |

### Part 2

| Task                                        | Name | Status     |
| ------------------------------------------- | ---- | ---------- |
| Binary file writer                          |      | Incomplete |
| Tokenizer and symbol table                  |      | Incomplete |
| Data processing instructions                |      | Incomplete |
| Multiply and Branch                         |      | Incomplete |
| Single Data Transfer & Special Instrucitons |      | Incomplete |
| Testing                                     |      | Incomplete |

## Part 1
Should we workout the overall structure of the project and write “emulate.c” (main function) and all the header files first?
I agree. We should define core functions with its argument like in the java tests so that everyone knows what info to process and pass on. We can define that in the header file for each module

### list of point-to-notice:
* Use Little-endian machine
* Initialize all memory and register units to 0
* All zero instructions means termination: andeq r0, r0, r0
* Print register and non-zero memory location upon termination
* The emulator is pipelined, so there are three instructions inside the pipeline, the PC counter is thus 8 bytes away from the current fetched instruction address(e.g. PC_Exe = PC_Fetched + 8)
* No memory-to-memory operation
* Check for errors:
	* Remember to check if the register fields are valid(i.e. in some cases Rm and Rn could not be the same, PC could not be in Rn and Rd, etc.)
	* Check if the address is greater than 65536, etc.
* Any instruction is executed if and only if the CPSR is satisfied

### A rough to-do-list:
* Build a binary file loader(potentially it’s basically a reader)
* Memory implementation: array?
* General register implementation: another array? Struct?
* The pipeline:
	* Fetch instruction from memory: copy from PC, PC + 4
	* Instruction decoder:
		* Recognize instruction type
		* Which parts refer to registers/memory/constants and resolve them
		* Execute the decoded instruction
	* Four different types of instructions to implement and Cond:
		* Data processing:
			* Cond(see Cond section below), Immediate Operand(constant or register), OpCode(see page 6), Set Condition Codes(choose to set CPSR flags or not), First Operand Register(Rn), Destination Register(Rd), Second Operand(operand2, 12-bits, could be constant or register, see below)
			* About Set Condition Codes: If Set Condition Codes is set to 0, then don’t change CPSR; if set to one, then V is unaffected, Z will be set only if result is all zeros, N will be set to the logical value of bit 31 of the result, and C will be set to the carry out of the result(see more detail at the bottom of page 6).
			* About operand2: If Immediate Operand is set to 1, then operand2 is a 12-bits immediate value(constant), with first 8 bits being unsigned and the rest of 4 bits being the half of the shift amount to the immediate(see page 7). If Immediate Operant is 0, then operand2 is a register(see page 7 for loads of details).
		* Multiply:
			* Cond(see Cond section below), Accumulate(if set then perform multiply and accumulate, otherwise perform multiplication only), Set Condition Codes(Same as in Data Processing instructions, see above), Destination register(Rd), Operand registers(Rn, Rs, Rm)
			* Again, instruction will only be executed if the condition field is satisfied.
			* Multiply instruction: Rd := Rm x Rs. Multiply-accumulate instruction: Rd := Rm x Rs + Rn.
			* Remember to check that Rd is not the same as Rm
			* Set Condition Codes: if it is 1 then set N to bit 31 of the result and Z to 1 if and only if the result is all 0
		* Single Data Transfer:
			* Cond(see Cond section below), Immediate Offset(if 1 then offset is a shifted register, if 0 then offset is an unsigned immediate offset), Pre/Post indexing bit(see page 9 and 10), Up bit, Load/Store bit, Base register, Source/Destination register, Offset(could be an unsigned 12 bits immediate OR a register)
			* In post-indexing, Rm and Rn could not be the same. If PC is used as the base register, make sure to consider the pipeline effect(e.g. PC_Exe = PC_Fetched + 8).
		* Branch:
			* Cond(see Cond section below), Offset: a signed 24 bit offset(notice this is 2’s complement signed)
			* The offset is first shifted left by 2 bits, sign extends to 32 bits and then added to the PC. Remember to take into account of the pipeline effect: PC_Exe = PC_Fetched + 8
			* When branch is executed, the pipeline is cleared(the fetched instruction is no longer valid).
		* Cond: eq, ne, ge, lt, gt, le, al. All instructions have this, see page 4.
		* Maybe more as an extension…
	* Finally, create the emulator loop to execute the pipeline and the execution of the four types of instructions, and terminate when executing all-0 instructions.
	* Testing using test suite and additional test cases

## Part 2
### A list of point-to-notice:
* Each line of assmebler file is either an instruction or a directive, optionally preceded by a lavel.
* No line is longer than 511 characters in length
* Each instruction is exactly 32 bits(ignore the 16-bits instructions)
* Registers are referred by strings r0, r1, r2, ... r16. Their roles are specified in Part I
* Support for the shifted register is optional
* Using 0x to specify the hexadecimal immediate values
* We can assume all instructions are in lower cases and well-formed

### A rough to-do list
* Construct a binary file writer
* Build a symbol table abstract data type. For now, using two-pass table is better I think
* Design the assembler with:
	* A tokenizer for breaking a line into its label, opcode, and operand fields according to the types and formats below. strtok_r and strtol functions are helpful.
	* Four functions, each is responsible for each of the four instruction type. Remember to use the function pointers: this can help to avoid a large switch statement of nested conditionals in the assmebler loop.
	* An integrated two-pass assembly process loop
* Data Processing Instructions(three formats):
	* Compute results: <opcode> Rd, Rn, <Operand2>. e.g. add, eor, sub, rsb, add, orr
	* Single operand assignment: mov Rd, <Operand2>
	* Set CPSR flags: <opcode> Rn, <Operand2>. e.g. tst, teq, cmp
	* For the fields above, Rd, Rn, Rm are registers, usually written in r0, r1, r2 ... r16
	* Operand2 represents an operand: could be an <expression> or Rm{,<shift>}
		* <#expression> is a numeric constant and must be within 8 bit. Throw an error if it's out of boundary. Specify hexadecimals by 0x
		* <shift> can be <shiftname> <register> or <shiftname> <#expression>. See Emulator Data Processing Instruction section(page 6) for more details. Supporting this case is optional
			* <shiftname> can be one of asr(algebraic shift left), lsl(logical shift left), lsr(logical shift right), or ror(rotate right)
	* When assmebling testing instructions(tst, teq, cmp), S should be 1; otherwise set it to 0
	* All data processing instructions should set the Cond field with 1110
* Multiply instructions(two formats):
	* Multiply with syntax: mul Rd, Rm, Rs
	* Multiply with accumulate: mla Rd, Rm, Rs, Rn
	* Field A should be 0 when performing mul and 1 when performing mla. S should be 0 and Cond should be 1110
* Single Data Transfer(only one format):
	* Load: ldr Rd, <address>
	* Store: str Rd, <address>
	* L is set to 1 when it's load, 0 when it's store
	* <address> can be:
		* Numeric constant of form <=expression>. It should be put in four bytes at the end of the assembled program and use the address of this extended value. Pc will be used as the base register and an offset will be provided.
		* A pre-indexed address:
			* [Rn], using base register Rn with an offset zero
			* [Rn, <#expression>], using base register Rn with offset of <#expression> bytes
			* (Optional) [Rn, {+/-}Rm{,<shift>}], using base register Rn with offset by +/- the content in Rm shifted by <shift> amount
		* A post-indexing addressing:
			* [Rn], <#expression>, offset base register Rn by <#expression> bytes
			* (Optional) [Rn], {+/-}Rm{,<shift>}, offset base register Rn by +/- the content in the index register Rm shifted by <shift>
* Branch Instructions(one format):
	* all with the form <opcode> <cond> <expression>
	* See the appropriate Cond field on page 16
	* <expression> is the target address(a label)
	* Compute the offset between the current address and the label and notice the off-by-8 bytes effect in the ARM pipeline in Part I
* Special instructions(andeq, lsl):
	* andeq r0, r0, r0 should produce th binary value 0x00000000. Could be implemented as part of the Data Processing Instruction or as a special case(just an ```and``` instruction)
	* lsl: lsl Rn, <#expression> as mov Rn, Rn, lsl <#expression>

