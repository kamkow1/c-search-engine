#ifndef DA_H
#define DA_H

#include <stdlib.h>
#include <assert.h>

#define DA_INIT_CAP 4

#define da_append(da, new) \
{ \
  if ((da)->count >= (da)->cap) { \
    (da)->cap = (da)->cap == 0 ? DA_INIT_CAP : (da)->cap*2; \
    (da)->items = realloc((da)->items, (da)->cap*sizeof(*(da)->items)); \
    assert((da)->items != NULL && "Not enough memory"); \
  } \
  (da)->items[(da)->count++] = (new); \
} \

#define da_init(da) \
{ \
  (da)->count = 0; \
  (da)->cap = DA_INIT_CAP; \
  (da)->items = malloc(sizeof(*(da)->items)*(da)->cap); \
} \

#define da_free(da) \
{ \
  free((da)->items); \
} \

#define da_last(da) ((da)->items[(da)->count-1])

#endif // DA_H

