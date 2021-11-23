#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Implementation of the `cp` command.
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: cp [source] [dest]");
    return EXIT_SUCCESS;
  }

  char *source_path = argv[1], *dest_path = argv[2];
  if (access(source_path, F_OK) != 0) {
    fprintf(stderr, "Could not open file %s", source_path);
    return EXIT_FAILURE;
  }
  if (access(dest_path, F_OK) == 0) {
    fprintf(stderr, "File %s already exists", dest_path);
    return EXIT_FAILURE;
  }

  FILE *source = fopen(source_path, "r");
  if (source == NULL) {
    fprintf(stderr, "Could not open file %s : %s", source_path,
            strerror(errno));
    return EXIT_FAILURE;
  }

  FILE *dest = fopen(dest_path, "w");
  if (dest == NULL) {
    fprintf(stderr, "Could not open file %s : %s", dest_path, strerror(errno));
    return EXIT_FAILURE;
  }

  size_t line_size = 1000;
  char *line_buf = malloc(sizeof(char) * line_size);
  size_t readed = 0, text_buff_size = 256, total_readed = 0;

  char *text_buff = malloc(sizeof(char) * text_buff_size);

  // The algorithm reads all the content and append it to a char buffer
  // In order to write the data in one single batch.
  // We can as well write immediately after reading.
  while ((readed = getline(&line_buf, &line_size, source)) != -1) {
    total_readed += readed;

    // Dynamically resize the text buffer if its size is too small
    if (total_readed > text_buff_size) {
      while (total_readed > text_buff_size) {
        text_buff_size *= 2;
      }

      text_buff = realloc(text_buff, text_buff_size);
    }

    // Append the line
    strcat(text_buff, line_buf);
  }

  fwrite(text_buff, total_readed, sizeof(char), dest);

  fclose(source);
  fclose(dest);
  return EXIT_SUCCESS;
}