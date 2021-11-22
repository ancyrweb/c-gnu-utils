#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sort(char *lines[], size_t line_count);

int main(int argc, char *argv[]) {
  FILE *f = stdin;
  if (argc > 1) {
    char *filename = argv[1];
    f = fopen(filename, "r");
  }

  if (f == NULL) {
    perror("Could not open file.");
    return EXIT_FAILURE;
  }

  size_t line_size = 1000;
  char *line_buf = malloc(sizeof(char) * line_size);
  char *sec_buf = malloc(sizeof(char) * line_size);

  size_t line_count = 0;
  size_t readed = 0;

  while ((readed = getline(&line_buf, &line_size, f)) != -1) {
    line_count++;
  }

  char **words = malloc(sizeof(char *) * line_count);
  fseek(f, 0, SEEK_SET);

  size_t current_line = 0;
  size_t alloc_size = 0;
  readed = 0;

  while ((readed = getline(&line_buf, &line_size, f)) != -1) {
    if (line_buf[readed - 1] == '\n') {
      // Remove the \n
      words[current_line] = malloc(sizeof(char) * (readed - 1));
      memcpy(words[current_line], line_buf, readed - 1);
      words[current_line][readed - 1] = '\0';
    } else {
      words[current_line] = malloc(sizeof(char) * readed);
      strcpy(words[current_line], line_buf);
    }

    current_line++;
  }

  sort(words, line_count);

  for (size_t i = 0; i < line_count; i++) {
    printf("%s\n", words[i]);
  }

  for (size_t i = 0; i < line_count; i++) {
    free(words[i]);
  }

  free(words);
  free(line_buf);
  return EXIT_SUCCESS;
}

void sort(char *lines[], size_t line_count) {
  char **left = malloc(sizeof(char *) * line_count);
  char **right = malloc(sizeof(char *) * line_count);

  size_t left_index = 0, right_index = 0;
  size_t pivot_index = line_count - 1;
  char *pivot = lines[pivot_index];

  for (size_t i = 0; i < line_count; i++) {
    if (i == pivot_index) {
      continue;
    }

    if (strcmp(pivot, lines[i]) > 0) {
      left[left_index] = lines[i];
      left_index++;
    } else {
      right[right_index] = lines[i];
      right_index++;
    }
  }

  if (left_index > 1) {
    sort(left, left_index);
  }
  if (right_index > 1) {
    sort(right, right_index);
  }

  size_t output_index = 0;
  for (size_t i = 0; i < left_index; i++, output_index++) {
    lines[output_index] = left[i];
  }
  lines[output_index] = pivot;
  output_index++;
  for (size_t i = 0; i < right_index; i++, output_index++) {
    lines[output_index] = right[i];
  }
}