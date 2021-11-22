#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum States { WORD, WHITESPACE };

int main(int argc, char *argv[]) {
  FILE *f = stdin;

  if (argc > 1) {
    char *filename = argv[1];
    f = fopen(filename, "r");
  }

  if (f == NULL) {
    fprintf(stderr, "Could not open file.");
    return EXIT_FAILURE;
  }

  size_t line_buf_size = 1;
  char *line_buf = malloc(sizeof(char) * line_buf_size);
  size_t line_count = 0;
  size_t readed = 0;

  size_t words = 0, lines = 0, characters = 0;
  size_t state = WORD;

  while ((readed = getline(&line_buf, &line_buf_size, f)) != -1) {
    size_t len = strlen(line_buf);
    lines++;
    state = WHITESPACE;

    for (size_t i = 0; i < len; i++) {
      characters++;

      if (line_buf[i] == ' ') {
        if (state == WORD) {
          words++;
          state = WHITESPACE;
        }
      } else {
        if (line_buf[i] != '\n') {
          state = WORD;
        }
      }
    }

    if (state == WORD) {
      words++;
    }
  };

  fprintf(stdout, "%d\t%d\t%d", lines - 1, words, characters);
  free(line_buf);
  fclose(f);
  return EXIT_SUCCESS;
}