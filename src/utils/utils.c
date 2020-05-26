#include "utils.h"

int readBinaryFile(char *path, uint32_t **buffer, int *size) {
  FILE *file;
  size_t fileLen;

  file = fopen(path, "rb");
  if (!file) {
    fprintf(stderr, "Unable to open file %s", path);
    return -1;
  }

  fseek(file, 0, SEEK_END);
  fileLen = ftell(file);
  fseek(file, 0, SEEK_SET);

  *size = fileLen + 1;
  *buffer = (uint32_t *)malloc(*size);

  if (!*buffer) {
    fprintf(stderr, "Memory error!");
    fclose(file);
    return -1;
  }
  fread(*buffer, fileLen, 1, file);
  fclose(file);
  return 0;
}

void printBits(uint32_t x) {
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

void dump_buffer(uint32_t *buffer, int size) {
  for (int i = 0; i < size; i++) printBits(buffer[i]);
}

int main(void){
		uint32_t *buffer = NULL;
		int size;
		if (readBinaryFile("./hello", &buffer, &size) == 0) {
			printf("%d\n", size);
			dump_buffer(buffer, size);
			free(buffer);
  	}
	return 0;
}