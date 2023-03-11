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

typedef struct {
  char *text;
} Token;

typedef struct {
  Token *items;
  size_t cap;
  size_t count;
} Token_Stream;

void index_process_buffer(Index *index, char *buffer);

#endif // INDEX_H

