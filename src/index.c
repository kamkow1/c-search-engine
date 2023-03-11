#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "index.h"
#include "da.h"

#define IGNORED_TOKENS " ,.-=></?\\{}[]()!@#$%^&*_'\"`~+|:;\n\r\t"

void token_stream_process_input(Token_Stream *ts, char *input)
{
  char *word = strtok(input, IGNORED_TOKENS);
  while (word) {
    // to lowercase
    for (char *c = word; *c; ++c) *c = *c > 0x40 && *c < 0x5b ? *c | 0x60 : *c;
    da_append(ts, (Token){.text = word});
    word = strtok(NULL, IGNORED_TOKENS);
  }
}


void index_process_buffer(Index *index, char *buffer)
{
  Token_Stream tokens;
  da_init(&tokens);
  token_stream_process_input(&tokens, buffer);

  for (size_t i = 0; i < tokens.count; i++) printf("word: %s\n", tokens.items[i].text);
}

