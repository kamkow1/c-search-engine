#ifndef INDEX_H
#define INDEX_H

#include <stddef.h>

typedef struct {
  char *normalized_name;
  size_t score;
} Term;

typedef struct {
  char *path;
  Term *items;
  size_t cap;
  size_t count;
} File;

typedef struct {
  File *items;
  size_t cap;
  size_t count;
} Index;

void index_process_buffer(Index *index, size_t file_index, char *buffer);

#endif // INDEX_H

