#include "global.h"
#include "state.h"
#include <assert.h>

static void test_reg_operations() {
    /*
     * Testing get_reg(word_t reg_no) and set_reg(word_t reg_no, word_t value):
     * Test if the register can be set and accessed properly, and if they are
     * initialized to 0 and in big endian format
     */
    for (word_t i = 0; i < REG_NUM; i++) {
        assert (get_reg(i) == 0);
        set_reg(i, i);
        assert (get_reg(i) == i);
    }
}

static void test_flag_operations() {
    /*
     * Testing get_flag(flag_t flag), set_flag(flag_t flag), and clear_flag(flag_t flag):
     * Test if the flags can be set and accessed properly
     */
    assert !(get_flag(C_FLAG) || get_flag(N_FLAG) || get_flag(V_FLAG) || get_flag(Z_FLAG));
    set_flag(C_FLAG);
    set_flag(N_FLAG);
    set_flag(V_FLAG);
    set_flag(Z_FLAG);
    assert (get_flag(C_FLAG) && get_flag(N_FLAG) && get_flag(V_FLAG) && get_flag(Z_FLAG));
    clear_flag(C_FLAG);
    clear_flag(N_FLAG);
    clear_flag(V_FLAG);
    clear_flag(Z_FLAG);
    assert !(get_flag(C_FLAG) || get_flag(N_FLAG) || get_flag(V_FLAG) || get_flag(Z_FLAG));
}

static void test_memory_operations() {
    /*
     * Test get_word(address_t addr), set_word(address_t, word_t instruction),
     * get_memory(address_t addr), and set_memory(address_t addr, byte_t value):
     * Test if the return value is in big endian format and initialized to 0
     */

    for (address_t addr = 0; addr < MEM_ADDR; addr++) {
        assert(get_word(addr) == 0 && get_memory(addr) == 0);
        word_t test_instr = addr;
        byte_t test_value = addr;
        set_memory(addr, test_value);
        assert (get_memory(addr) == test_value && sizeof(get_memory(addr)) == sizeof(byte_t));
        set_word(addr, test_instr);
        assert(get_word(addr) == test_instr && sizeof(get_word(addr)) == sizeof(word_t);
    }
}

static void test_load_program(char *path) {
    /*
     * Test load_program(word_t *buffer, size_t size):
     * Check if the program is loaded into the memory properly starting
     * with address 0 and 4 bytes for each instruction
     */
    uint32_t *buffer = NULL;
    size_t size;
    if (read_binary_file(path, &buffer, &size) == 0) {
        swap_endian(buffer, size);
        load_program(buffer, size);
    } else {
        fprintf(stderr, "Failed to load binary file into the buffer!");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        assert (get_word(i << 2) == buffer[i]);
    }

    free(buffer);
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

int main(int argc, char **argv) {
    test_reg_operations();
    test_flag_operations();
    test_memory_operations;
    test_load_program(argv[1]);
    test_return_state();
}