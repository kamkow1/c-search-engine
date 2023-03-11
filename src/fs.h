#ifndef FS_H
#define FS_H

#include <stddef.h>

#include "index.h"

// only on posix
void fs_load_file(const char *path, char *buffer, size_t size);
int fs_process_dir(Index *index, char *path);

#endif // FS_H
