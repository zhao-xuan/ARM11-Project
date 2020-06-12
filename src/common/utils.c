/*
 * Implementation of utility functions
 */

#include "utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"

/* error detection for memory allocation */
void *eMalloc(size_t size) {
  void *vp = malloc(size);
  if (!vp) {
    fprintf(stderr, "Malloc memory error!\n");
    exit(EXIT_FAILURE);
  }
  return vp;
}

void *eCalloc(size_t nmemb, size_t size) {
  void *vp = calloc(nmemb, size);
  if (!vp) {
    fprintf(stderr, "Calloc memory error!\n");
    exit(EXIT_FAILURE);
  }
  return vp;
}

void *eRealloc(void *ptr, size_t size) {
  void *vp = realloc(ptr, size);
  if (!vp) {
    fprintf(stderr, "Realloc memory error!\n");
    exit(EXIT_FAILURE);
  }
  return vp;
}

/* File operations */
void read_binary_file(const char *path, word_t *buffer, size_t *size) {
  FILE *file;
  size_t file_length;

  file = fopen(path, "rb");
  if (!file) {
    fprintf(stderr, "Unable to open file %s. \n", path);
    exit(EXIT_FAILURE);
  }

  fseek(file, 0, SEEK_END);
  file_length = ftell(file);
  fseek(file, 0, SEEK_SET);
  *size = (file_length + 1) / 4;

  if (fread(buffer, file_length, 1, file) != 1 || ferror(file)) {
    fprintf(stderr, "Error reading from file.\n");
    fclose(file);
    exit(EXIT_FAILURE);
  }
  fclose(file);
}

void write_binary_file(const char *path, word_t *buffer, size_t size) {
  FILE *file;

  file = fopen(path, "wb");
  if (!file) {
    fprintf(stderr, "Unable to open file %s. \n", path);
    exit(EXIT_FAILURE);
  }

  if (fwrite(buffer, 4 * size, 1, file) != 1 || ferror(file)) {
    fprintf(stderr, "Error writing to file.\n");
    fclose(file);
    exit(EXIT_FAILURE);
  }
  fclose(file);
}

int read_assembly_file(const char *path, char **buffer) {
  FILE *file;

  file = fopen(path, "r");
  if (!file) {
    fprintf(stderr, "Unable to open file %s. \n", path);
    exit(EXIT_FAILURE);
  }

  char readbuffer[MAX_LINE_LENGTH];
  int lines;
  for (lines = 0; fgets(readbuffer, MAX_LINE_LENGTH, file); lines++) {
    if (readbuffer[0] == '\n') {
      lines--;
      continue;
    }

    buffer[lines] = eMalloc(strlen(readbuffer) + 1);
    strncpy(buffer[lines], strtok(readbuffer, "\n"), strlen(readbuffer));
  }

  fclose(file);
  return lines;
}

void free_buffer(char **buffer, int size) {
  for (int i = 0; i < size; i++) {
    if (buffer[i]) free(buffer[i]);
  }
  free(buffer);
}

/* Prints for debugging */
void print_bits(word_t x) {
  int i;
  word_t mask = 1 << 31;
  for (i = 0; i < 32; ++i) {
    if ((x & mask) == 0) {
      printf("0");
    } else {
      printf("1");
    }
    x = x << 1;
  }
  printf("\n");
}

void dump_buffer(word_t *buffer, size_t size) {
  for (int i = 0; i < size; i++) print_bits(buffer[i]);
}

void dump_hex(word_t *buffer, size_t size) {
  for (int i = 0; i < size; i++) printf("%08x\n", buffer[i]);
}

/*Sign extending words */
word_t sign_extend(word_t x, int bits) {
  word_t m = 1u << (bits - 1);
  return (x ^ m) - m;
}
