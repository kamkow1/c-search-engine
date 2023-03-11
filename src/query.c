#include <string.h>

#include "libstemmer.h"
#include "query.h"

void query_stem(const char *input, char *buffer)
{
  const char *charenc = NULL;
  struct sb_stemmer *stemmer = sb_stemmer_new("english", charenc);
  const sb_symbol *output = sb_stemmer_stem(stemmer, (sb_symbol*)input, strlen(input));

  for (size_t i = 0; i < strlen((char*)output); i++) buffer[i] = output[i];
  sb_stemmer_delete(stemmer);
}

