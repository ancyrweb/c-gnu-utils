#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int get_file_size(FILE *);
int concat_size(char *, char *);
void print_chmod(short, char *);

int main(int argc, char *argv[]) {
  char *path = NULL;
  if (argc > 1) {
    path = argv[argc - 1];
  }

  if (path == NULL) {
    size_t path_size = sizeof(char) * PATH_MAX;
    path = malloc(path_size);
    if (getcwd(path, path_size) == NULL) {
      perror("Could not open current directory");
      exit(-1);
    }
  }

  if (strcmp(path + (strlen(path) - 1), "/") != "0") {
    strcat(path, "/");
  }

  DIR *d = opendir(path);
  if (d == NULL) {
    perror("Could not open directory");
    exit(-1);
  }

  struct dirent *entry;
  FILE *f;
  char *full_file_name;
  char *line_buf = malloc(sizeof(char) * 1000);
  char *txt_buf = malloc(sizeof(char) * 1000);

  while ((entry = readdir(d))) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    full_file_name =
        malloc(sizeof(char) * concat_size(path, entry->d_name) + 1);
    strcpy(full_file_name, path);
    strcat(full_file_name, entry->d_name);

    struct stat file_stats;
    if (stat(full_file_name, &file_stats) != 0) {
      sprintf(txt_buf, "Could not get informations about file %s",
              full_file_name);
      perror(txt_buf);
      continue;
    }

    // Initialize the line
    strcpy(line_buf, "");

    // --- TYPE
    if (S_ISDIR(file_stats.st_mode)) {
      strcat(line_buf, "d");
    } else if (S_ISREG(file_stats.st_mode)) {
      strcat(line_buf, "-");
    } else {
      strcat(line_buf, "?");
    }

    // --- CHMOD
    short user_mode = (file_stats.st_mode >> 6) & 0000007;
    short group_mode = (file_stats.st_mode >> 3) & 0000007;
    short all_mode = file_stats.st_mode & 0000007;

    print_chmod(user_mode, txt_buf);
    strcat(line_buf, txt_buf);
    print_chmod(group_mode, txt_buf);
    strcat(line_buf, txt_buf);
    print_chmod(all_mode, txt_buf);
    strcat(line_buf, txt_buf);

    strcat(line_buf, "\t");

    // --- SIZE
    sprintf(txt_buf, "%d\t", file_stats.st_size);
    strcat(line_buf, txt_buf);

    // --- NAME
    strcat(line_buf, entry->d_name);

    // Finally print it
    printf("%s\n", line_buf);
    free(full_file_name);
  }

  free(line_buf);
  closedir(d);
  return 0;
}

int get_file_size(FILE *f) { return 0; }

int concat_size(char *a, char *b) { return strlen(a) + strlen(b); }

void print_chmod(short value, char *buff) {
  strcpy(buff, "");
  strcat(buff, (value & 4) > 0 ? "r" : "-");
  strcat(buff, (value & 2) > 0 ? "w" : "-");
  strcat(buff, (value & 1) > 0 ? "x" : "-");
}