#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "index.h"
#include "da.h"

#define IGNORED_TOKENS " ,.-=></?\\{}[]()!@#$%^&*_'\"`~+|:;\n\r\t"

void index_rank_term(Index *index, size_t file_index, Term *term)
{
  size_t score = 0;
  for (size_t i = 0; i < index->items[file_index].count; i++)
    if (!strcmp(index->items[file_index].items[i].normalized_name, term->normalized_name)) score++;
  term->score = score;
}


void index_process_buffer(Index *index, size_t file_index, char *buffer)
{
  char *word = strtok(buffer, IGNORED_TOKENS);
  while (word) {
    // to lowercase
    for (char *c = word; *c; ++c) *c = *c > 0x40 && *c < 0x5b ? *c | 0x60 : *c;
    da_append(&index->items[file_index], (Term){.normalized_name = word});
    word = strtok(NULL, IGNORED_TOKENS);
  }

  for (size_t i = 0; i < index->items[file_index].count; i++)
    index_rank_term(index, file_index, &index->items[file_index].items[i]);

  for (size_t i = 0; i < index->items[file_index].count; i++) {
    printf("term: %s, score: %ld\n", 
        index->items[file_index].items[i].normalized_name, 
        index->items[file_index].items[i].score
    );
  }
}

