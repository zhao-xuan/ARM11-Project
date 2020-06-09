#include <stddef.h>

#include "mnemonic_table.h"
#include "table.h"

#define NUM_DP_MNEMONICS 10
#define NUM_ML_MNEMONICS 2
#define NUM_BR_MNEMONICS 6
#define NUM_DT_MNEMONICS 2
#define NUM_SP_MNEMONICS 2


static const symbol_table_t *mnemonic_table;


static char dp_mnemonics[][3] = 
  { "add", "sub", "rsb", "and", "eor", "orr", "mov", "tst", "teq", "cmp" };

static char ml_mnemonics[][3] = 
  { "mul", "mla" };

static char br_mnemonics[][3] =  
  { "beq", "bne", "bge", "blt", "bgt", "ble"};

static char dt_mnemonics[][3] = 
  { "ldr", "str" };

static bool init_dp_mnemonics(symbol_table_t *);
static bool init_dt_mnemonics(symbol_table_t *);
static bool init_br_mnemonics(symbol_table_t *);
static bool init_ml_mnemonics(symbol_table_t *);
static bool init_sp_mnemonics(symbol_table_t *);

bool init_mnemonic_table() {
  *mnemonic_table = create_table();
   
  return mnemonic_table 
      && init_dp_mnemonics(mnemonic_table)
      && init_dt_mnemonics(mnemonic_table)
      && init_br_mnemonics(mnemonic_table)
      && init_ml_mnemonics(mnemonic_table)
      && init_sp_mnemonics(mnemonic_table);
}

static bool init_dp_mnemonics(symbol_table_t *mnemonic_table) {
  for (int i = 0; i < NUM_DP_MNEMONICS; i++) {
    mnemonic_p mnemonic = malloc(sizeof(mnemonic_t));
    if (!mnemonic) return false;
    byte_t opcode = (i > 4 && i < 8) ? (i + 7) : i; 
    *mnemonic = (mnemonic_t) {DATA_PROCESSING, ALWAYS, opcode};
    insert_literal(mnemonic_table, dp_mnemonics[i], (void *) mnemonic);
  }
}
static bool init_dt_mnemonics(symbol_table_t *mnemonic_table) {
  for (int i = 0; i < NUM_DT_MNEMONICS; i++) {
    mnemonic_p mnemonic = malloc(sizeof(mnemonic_t));
    *mnemonic = (mnemonic_t) {DATA_TRANSFER, ALWAYS};
    insert_literal(mnemonic_table, dt_mnemonics[i], (void *) mnemonic);
  }
}

static bool init_br_mnemonics(symbol_table_t *mnemonic_table) {
  for (int i = 0; i < NUM_BR_MNEMONICS; i++) {
    mnemonic_p mnemonic = malloc(sizeof(mnemonic_t));
    if (!mnemonic) return false;
    *mnemonic = (mnemonic_t) {BRANCH, i > 1 ? (i + 8) : i}; 
    insert_literal(mnemonic_table, br_mnemonics[i], (void *) mnemonic);
  }
}

static bool init_ml_mnemonics(symbol_table_t *mnemonic_table) {
  for (int i = 0; i < NUM_ML_MNEMONICS; i++) {
    mnemonic_p mnemonic = malloc(sizeof(mnemonic_t));
    if (!mnemonic) return false;
    *mnemonic = (mnemonic_t) {MULTIPLY, ALWAYS}; 
    insert_literal(mnemonic_table, ml_mnemonics[i], (void *) mnemonic);
  }
}

static bool init_sp_mnemonics(symbol_table_t *mnemonic_table) {
  mnemonic_p lsl  = malloc(sizeof(mnemonic_t));
  mnemonic_p halt = malloc(sizeof(mnemonic_t));

  if (!halt || !lsl) return false;
  *halt = (mnemonic_t) {HALT};
  *lsl = (mnemonic_t) {LSL, ALWAYS};

  insert_literal(mnemonic_table, "andeq", (void *) halt);
  insert_literal(mnemonic_table, "lsl", (void *) lsl);
}

mnemonic_p get_mnemonic_data(char *mnemonic) {
  
  return NULL;
}

void free_mnemonic_table() {
  free_table(mnemonic);
}
