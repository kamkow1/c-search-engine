#include <json-c/json_object.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defer.h"
#include "index.h"
#include "da.h"
#include "query.h"
#include "json-c/json.h"

#define IGNORED_TOKENS " ,.-=></?\\{}[]()!@#$%^&*_'\"`~+|:;\n\r\t"

typedef struct {
  char **items;
  size_t count;
  size_t cap;
} Ranked_Terms;

void index_rank_term(Index *index, size_t file_index, Term *term)
{
  size_t score = 0;
  for (size_t i = 0; i < index->items[file_index].count; i++)
    if (!strcmp(index->items[file_index].items[i].normalized_name, term->normalized_name)) score++;
  term->score = score;
}


void index_process_buffer(Index *index, size_t file_index, char *buffer)
{
  Ranked_Terms rt;
  da_init(&rt);

  char *word = strtok(buffer, IGNORED_TOKENS);
  while (word) {
    // to lowercase
    for (char *c = word; *c; ++c) *c = *c > 0x40 && *c < 0x5b ? *c | 0x60 : *c;

    char *stemmed_word = (char*)malloc(sizeof(char)*256);
    query_stem(word, stemmed_word);

    da_append(&index->items[file_index], (Term){.normalized_name = stemmed_word});
    index_rank_term(
        index, 
        file_index,
        &da_last(&index->items[file_index])
    );

    da_append(&rt, index->items[file_index].items[index->items[file_index].count-1].normalized_name);
    word = strtok(NULL, IGNORED_TOKENS);
  }
}

void index_debug_print(Index *index)
{
  for (size_t i = 0; i < index->count; i++) {
    printf("File: %s\n", index->items[i].path);
    for (size_t j = 0; j < index->items[i].count; j++) {
      printf("   Term: %s, Score: %ld\n", 
        index->items[i].items[j].normalized_name, 
        index->items[i].items[j].score
      );
    }
  }
}

void index_save_json(Index *index, char *name)
{
  struct json_object *json = json_object_new_array();

  for (size_t i = 0; i < index->count; i++) {
    struct json_object *file = json_object_new_object();
    json_object_object_add(file, "path", json_object_new_string(index->items[i].path));
    
    struct json_object *terms = json_object_new_array();
    for (size_t j = 0; j < index->items[i].count; j++) {
      struct json_object *term = json_object_new_object();
      json_object_object_add(term, "text", json_object_new_string(index->items[i].items[j].normalized_name));
      json_object_object_add(term, "score", json_object_new_int(index->items[i].items[j].score));
      json_object_array_add(terms, term);
    }

    json_object_object_add(file, "terms", terms);
    json_object_array_add(json, file);
  }

  const char *json_string = json_object_to_json_string_ext(json, JSON_C_TO_STRING_PRETTY);
  if (!name) {
    printf("No file provided. Printing to Stdout:\n\n%s\n", json_string);
  } else {
    printf("Saving to `%s`\n", name);
    char buffer[2048];
    sprintf(buffer, "%s.json", name);
    FILE *fp = fopen(buffer, "w+");
    if (!fp) fprintf(stderr, "Failed to open file for writing: `%s`\n", buffer);

    defer { fclose(fp); }
    fprintf(fp, json_string);
  }

  json_object_put(json);
}

void index_save(Index *index, char *name, Index_Save_Interface interface)
{
  switch (interface) {
  case ISI_JSON: index_save_json(index, name); break;
  default: fprintf(stderr, "Unknown saving interface\n"); break;
  }
}

