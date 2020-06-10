#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "mnemonic_table.h"
#include "table.h"

#define NUM_DP_MNEMONICS 10
#define NUM_ML_MNEMONICS 2
#define NUM_BR_MNEMONICS 8
#define NUM_DT_MNEMONICS 2
#define NUM_SP_MNEMONICS 2

#define MNEMONIC_SIZE 3

#define INSERT_MNEMONIC(value)\
  insert_literal(mnemonic_table, value, mnemonic)

static symbol_table_t *mnemonic_table = NULL;


static char dp_mnemonics[][4] = 
  { "and", "eor", "sub", "rsb", "add", "orr", "mov", "tst", "teq", "cmp" };

static char ml_mnemonics[][4] = 
  { "mul", "mla" };

static char br_mnemonics[][4] =  
  { "beq", "bne", "bge", "blt", "bgt", "ble", "bal", "b"};

static char dt_mnemonics[][4] = 
  { "str", "ldr" };

static bool init_dp_mnemonics();
static bool init_dt_mnemonics();
static bool init_br_mnemonics();
static bool init_ml_mnemonics();
static bool init_sp_mnemonics();

bool init_mnemonic_table() {
  if (mnemonic_table) {
    fprintf(stderr, "Mnemonic table already initialised\n");
    return false;
  }

  mnemonic_table = create_table();
   
  bool success = mnemonic_table 
                 && init_dp_mnemonics(mnemonic_table)
                 && init_dt_mnemonics(mnemonic_table)
                 && init_br_mnemonics(mnemonic_table)
                 && init_ml_mnemonics(mnemonic_table)
                 && init_sp_mnemonics(mnemonic_table);

  /* If anything fails then free the table and return false*/
  if (!success) free_mnemonic_table();
  return success;
}

static void get_opcode_and_set_bits(int index, word_t *opcode, word_t *set) {
  word_t opcode_raw = index < 5 ? index : (index < 7 ? index + 7 : index + 1);
  *opcode = opcode_raw << OPCODE_LOCATION;                
  word_t set_raw = index > 6;
  *set = set_raw << SET_COND_LOCATION;
}


static bool init_dp_mnemonics() {
  word_t opcode = 0, set = 0, bin = 0;
  for (int i = 0; i < NUM_DP_MNEMONICS; i++) {
    /* Malloc pointer to value */
    mnemonic_p mnemonic = malloc(sizeof(mnemonic_t));
    if (!mnemonic) return false;
    
    /* Get opcode and S-bit shifted to the right position */
    get_opcode_and_set_bits(i, &opcode, &set);

    /* Set the correct bits in the binary */
    bin = (ALWAYS << COND_LOCATION) | opcode | set;
     *mnemonic = (mnemonic_t) {bin, DATA_PROCESSING};
  
    /* Try to insert mnemonic into table */
    if (!INSERT_MNEMONIC(dp_mnemonics[i])) return false;
  
  }
  return true;
}

static bool init_dt_mnemonics() {
  word_t bin;
  for (int i = 0; i < NUM_DT_MNEMONICS; i++) {
    mnemonic_p mnemonic = malloc(sizeof(mnemonic_t));
    if (!mnemonic) return false;

    bin = (ALWAYS << COND_LOCATION) | DT_CONST;
    bin |= i << LOAD_STORE_LOCATION;
    *mnemonic = (mnemonic_t) {bin, DATA_TRANSFER};

    if (!INSERT_MNEMONIC(dt_mnemonics[i])) return false;
  }
  return true;
}

static word_t get_cond(int index) {
  if (index == NUM_BR_MNEMONICS - 1) 
    return ALWAYS << COND_LOCATION;

  word_t cond_raw = index > 1 ? (index + 8) : index;
  return cond_raw << COND_LOCATION;
}

static bool init_br_mnemonics() {
  for (int i = 0; i < NUM_BR_MNEMONICS; i++) {
    mnemonic_p mnemonic = malloc(sizeof(mnemonic_t));
    if (!mnemonic) return false;
    
    word_t bin = get_cond(i) | BRANCH_CONST;
    *mnemonic = (mnemonic_t) {bin, BRANCH}; 
    
    if (!INSERT_MNEMONIC(br_mnemonics[i])) return false;
  }
  return true; 
}

static bool init_ml_mnemonics() {
  word_t bin = MUL_CONST | (ALWAYS << COND_LOCATION);
  for (int i = 0; i < NUM_ML_MNEMONICS; i++) {
    mnemonic_p mnemonic = malloc(sizeof(mnemonic_t));
    if (!mnemonic) return false;

    word_t set_bit = i << ACCUMULATE_LOCATION;
    *mnemonic = (mnemonic_t) {bin | set_bit, MULTIPLY}; 
   
   if (!INSERT_MNEMONIC(ml_mnemonics[i])) return false; 
  }
  return true;
}

static bool init_sp_mnemonics() {
  mnemonic_p mnemonic = malloc(sizeof(mnemonic_t));
  if (!mnemonic) return false;
  
  *mnemonic = (mnemonic_t) {0, HALT};
  return INSERT_MNEMONIC("andeq"); 
}


mnemonic_p get_mnemonic_data(char *mnemonic) {
  if (!mnemonic_table) {
    fprintf(stderr, "Mnemonic table uninitialised\n");
    return NULL;
  }
  
  return (mnemonic_p) get_literal(mnemonic_table, mnemonic);
}

void free_mnemonic_table() {
  free_table(mnemonic_table);
}
