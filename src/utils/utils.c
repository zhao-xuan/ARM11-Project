#include "utils.h"

void read_binary_file(const char *path, uint32_t **buffer, size_t *size) {
  FILE *file;
  size_t file_length;

  file = fopen(path, "rb");
  if (!file) {
    fprintf(stderr, "Unable to open file %s\n", path);
    exit(EXIT_FAILURE);
  }

  fseek(file, 0, SEEK_END);
  file_length = ftell(file);
  fseek(file, 0, SEEK_SET);

  size_t size_bytes = file_length + 1;
  *buffer = (uint32_t *)malloc(size_bytes);
  *size = size_bytes / 4;

  if (!*buffer) {
    fprintf(stderr, "Memory error!\n");
    fclose(file);
    exit(EXIT_FAILURE);
  }

  if (fread(*buffer, file_length, 1, file) != 1 || ferror(file)) {
    fprintf(stderr, "Error reading from file.\n");
    fclose(file);
    exit(EXIT_FAILURE);
  }
  fclose(file);
}

void print_bits(uint32_t x) {
  int i;
  uint32_t mask = 1 << 31;
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

void dump_buffer(uint32_t *buffer, size_t size) {
  for (int i = 0; i < size; i++) print_bits(buffer[i]);
}

void dump_hex(uint32_t *buffer, size_t size) {
  for (int i = 0; i < size; i++) printf("%08x\n", buffer[i]);
}

int out_of_bound_check(uint32_t addr, size_t size) {
  if (addr >= size) {
    fprintf(stdout, "Error: Out of bounds memory access at address 0x%08x\n", addr);
    return -1;
  }
  return 0;
}

uint32_t sign_extend(uint32_t x, int bits) {
    uint32_t m = 1u << (bits - 1);
    return (x ^ m) - m;
}

