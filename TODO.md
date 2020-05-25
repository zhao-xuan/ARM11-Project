# To-do List
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
Memory implementation: array?
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
### A rough to-do list
* 