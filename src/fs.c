#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>

#include "fs.h"
#include "da.h"
#include "defer.h"
#include "index.h"

void fs_load_file(const char *path, char *buffer, size_t size)
{
  int fd = open(path, O_RDONLY);
  int len = lseek(fd, 0, SEEK_END);
  char *data = (char*)mmap(0, len, PROT_READ, MAP_PRIVATE, fd, 0);
  memcpy(buffer, data, size);
}

int fs_process_dir(Index *index, char *path)
{
  DIR *dir;
  if (!(dir = opendir(path))) {
    fprintf(stderr, "Cannot open directory: `%s`\n", path);
    return 0;
  }
  defer { closedir(dir); }

  struct dirent *entry;
  while ((entry = readdir(dir))) {
    char fullname[256] = {0};
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
    snprintf(fullname, 100, "%s/%s", path, entry->d_name);
#pragma GCC diagnostic pop

    if (entry->d_type == DT_DIR) {
      if (!strcmp(entry->d_name, ".") && !strcmp(entry->d_name, ".."))
        return fs_process_dir(index, fullname);
    } else {
      printf("Indexing: `%s`\n", fullname);

      da_append(index, (File){.path = fullname});

      char buffer[1024*8];
      fs_load_file(fullname, buffer, sizeof(buffer));
      if (index) index_process_buffer(index, index->count-1, buffer);
    }
  }

  return 0;
}

