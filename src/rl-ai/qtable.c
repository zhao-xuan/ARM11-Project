#include "qtable.h"

#define Q_SIZE 100
/*
int compare_env_state(void *this, void *that) {
  grid_t this_grid = ((env_t *) this)->grid;
  grid_t that_grid = ((env_t *) that)->grid;
 
  for (int i = 0; i < GHEIGHT; i++){
    for (int j = 0; j < GWIDTH; j++) {
      if ((this_grid[i][j] > 0) ^ (that_grid[i][j] > 0))
        return this_grid[i][j] - that_grid[i][j];
    }
  }

  return ((env_t *) this)->block - ((env_t *) that)->block;

}
*/
/*
long compare_hash(void *this, void *that) {
  return *((long *) this) - *((long *) that);
}
*/
/*
long identity(void *ptr) {
  return *((long *) ptr);
}
*/
long hash_env_state(void *env_ptr) {
  env_t *env = (env_t *) env_ptr;
  long hash = 0;
  for (int i = 0; i < GWIDTH; i++) {
    hash += env->elevation[i] * 17;
  }
  
  hash += env->block * 5381 + env->spin * 1793;
  
  return hash;
}

long compare_env(void *this, void *that) {
  env_t *env_this = (env_t *) this;
  env_t *env_that = (env_t *) that;
  for (int i = 0; i < GWIDTH; i++) {
    long diff = env_this->elevation[i] - env_that->elevation[i];
    if (diff != 0) return diff;
  }

 if (env_this->block != env_that->block) 
   return env_this->block - env_that->block;
  
 return env_this->spin - env_that->spin;
}


q_table *init_qtable() {
  return create_hashtable(&compare_env, &hash_env_state, Q_SIZE);  
}

bool insert_qtable(q_table *table, env_t *env_state, void *value) {
  return hash_insert(table, (void *) env_state, sizeof(env_t), value);
}

bool load_hash_qtable(q_table *table, long *hash, void *value) {
  return hash_insert(table, hash, sizeof(long), value);
}

void *find_qtable(q_table *table, env_t *env_state) {
  return hash_find(table, (void *) env_state);
}

void free_qtable(q_table *table) {
  hash_table_itr *itr = get_hash_table_itr(table);

  while (hash_iterator_hasnext(itr)) {
    env_t *env = NULL;
    hash_iterator_next(itr, env);
    free(env->elevation);
  }
  free_hashtable(table);
}
