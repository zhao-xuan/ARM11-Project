#include "global.h"
#include "state.h"

static void test_reg_operations() {
    /*
     * Testing get_reg(word_t reg_no) and set_reg(word_t reg_no, word_t value):
     * Test if the register can be set and accessed properly, and if they are
     * initialized to 0 and in big endian format
     */
    char *name;

    for (word_t i = 0; i < REG_NUM; i++) {
        /* Testing register initialization */
        name = "Testing if registers initialized to 0"
        testword(i, 0, name);
        set_reg(i, i);
        /* Testing register set */
        name = "Testing if registers can be set properly"
        testword(get_reg(i), i, name);
    }
}

static void test_flag_operations() {
    /*
     * Testing get_flag(flag_t flag), set_flag(flag_t flag), and clear_flag(flag_t flag):
     * Test if the flags can be set and accessed properly
     */
    char *name;

    /* Testing flag initialization */
    name = "Testing if flags are initialized to 0"
    testbool(!(get_flag(C_FLAG) || get_flag(N_FLAG)
            || get_flag(V_FLAG) || get_flag(Z_FLAG)), name);
    set_flag(C_FLAG);
    set_flag(N_FLAG);
    set_flag(V_FLAG);
    set_flag(Z_FLAG);
    name = "Testing if flags can be set properly"
    testbool(get_flag(C_FLAG) && get_flag(N_FLAG)
            && get_flag(V_FLAG) && get_flag(Z_FLAG), name);
    clear_flag(C_FLAG);
    clear_flag(N_FLAG);
    clear_flag(V_FLAG);
    clear_flag(Z_FLAG);
    name = "Testing if flags can be cleared properly"
    testbool(!(get_flag(C_FLAG) || get_flag(N_FLAG)
            || get_flag(V_FLAG) || get_flag(Z_FLAG)), name);
}

static void test_memory_operations() {
    /*
     * Test get_word(address_t addr), set_word(address_t, word_t instruction),
     * get_memory(address_t addr), and set_memory(address_t addr, byte_t value):
     * Test if the return value is in big endian format and initialized to 0
     */

    char *name;
    const address_T STEP = 2048;

    for (address_t addr = 0; addr < MEM_ADDR; addr += STEP) {
        /* Testing memory initialization */
        name = "Testing if memory is initialized to 0";
        testword(get_word(addr), 0, name);
        testword(get_memory(addr), 0, name);

        /* Testing set_memory() */
        name = "Testing if set_memory() works properly";
        word_t test_instr = addr;
        byte_t test_value = addr;
        set_memory(addr, test_value);
        testword(get_memory(addr), test_value, name);

        /* Testing memory size after set_memory()*/
        name = "Testing if memory has the right size after calling set_memory()";
        testsize(sizeof(get_memory(addr)), sizeof(byte_t), name);

        /* Testing set_word() */
        name = "Testing if set_word() works properly";
        set_word(addr, test_instr);
        testword(get_word(addr), test_instr, name);

        /* Testing memory size after set_word() */
        name = "Testing if memory has the right size after calling set_word()";
        testsize(sizeof(get_word(addr)), sizeof(word_t), name);
    }
}

static void test_load_program() {
    /*
     * Test load_program(word_t *buffer, size_t size):
     * Check if the program is loaded into the memory properly starting
     * with address 0 and 4 bytes for each instruction
     */

    /* Creating an example file to load into memory */
    FILE *fp = fopen("memory_load_program_test.txt", "w+");
    fprintf(fp, "^A^P ã^B <81>â");

    char *name;
    uint32_t *buffer = NULL;
    size_t size;

    if (read_binary_file(fp, &buffer, &size) == 0) {
        swap_endian(buffer, size);
        load_program(buffer, size);
    } else {
        fprintf(stdout, "Failed to load binary file into the buffer!");
        exit(EXIT_FAILURE);
    }

    /* Testing if the program is correctly loaded in memory */
    name = "Testing if the program is loaded into the memory";
    for (int i = 0; i < size; i++) {
        testword(get_word(i << 2), buffer[i], name);
    }

    free(buffer);
    fclose(fp);
    system("rm -f memory_load_program_test.txt");
}

static void test_return_state() {
    /*
     * Test return_state(FILE *fp):
     * Check if the state is printed correctly after executing the
     * testing functions above
     */
    FILE *f = fopen("memory_test.txt", "w+");
    if (f == NULL) {
        fprintf(stderr, "Failed to open the text file!");
        exit(EXIT_FAILURE);
    }

    return_state(f);
    /* print to the standard output for convenience*/
    while(!feof(f)) {
        fprintf(stdout, "%c", fgetc(f));
    }

    fclose(f);
}

int main(void) {
    /* construct state using init_state() */
    init_state();

    /* testing cases */
    test_reg_operations();
    test_flag_operations();
    test_memory_operations;
    test_load_program();
    /* the state will be deconstructed in test_return_state() */
    test_return_state();
}
