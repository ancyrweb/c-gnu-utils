#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc == 1) {
    fprintf(stderr, "No file provided.");
    return EXIT_FAILURE;
  }

  char *filename = argv[argc - 1];
  FILE *f = fopen(filename, "r");
  if (f == NULL) {
    fprintf(stderr, "Could not open file %s", filename);
    return EXIT_FAILURE;
  }

  fseek(f, 0, SEEK_END);
  int filesize = ftell(f);

  if (filesize == 0) {
    return EXIT_SUCCESS;
  }

  char *file_content = malloc(sizeof(char) * filesize);
  strcpy(file_content, "");
  fseek(f, 0, SEEK_SET);

  int line_size = 0;
  int line_read = 0;
  char *line = malloc(sizeof(char) * line_size);

  while ((line_read = getline(&line, &line_size, f)) != -1) {
    strcat(file_content, line);
  }

  free(line);
  printf("%s", file_content);
  free(file_content);

  return EXIT_SUCCESS;
}