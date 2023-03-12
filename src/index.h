#ifndef INDEX_H
#define INDEX_H

#include <stddef.h>

#include "da.h"

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

typedef enum {
  ISI_JSON
} Index_Save_Interface;

void index_process_buffer(Index *index, size_t file_index, char *buffer);
void index_debug_print(Index *index);
void index_save(Index *index, char *name, Index_Save_Interface interface);

#define index_full_free(index) \
{ \
  for (size_t _i = 0; _i < (index)->count; _i++) { \
    for (size_t _j = 0; _j < (index)->items[_i].count; _j++) { \
      free((index)->items[_i].items[_j].normalized_name); \
    } \
    free((index)->items[_i].path); \
    da_free(&(index)->items[_i]); \
  } \
  da_free((index)); \
} \

#endif // INDEX_H

