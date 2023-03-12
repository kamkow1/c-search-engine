#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "da.h"
#include "index.h"
#include "query.h"
#include "server.h"
#include "defer.h"
#include "fs.h"

#define CLI_CASE_SENSITIVE false

int main(int argc, char **argv)
{
  char *output_name = "index";
  int opt;

  while ((opt = getopt(argc, argv, "s:i:o:")) != -1) {
    switch (opt) {
    case 's': {
      printf("Starting the server...\n");
      printf("Open your browser at `localhost:8080`\n");
      server_run();
    } break;
    case 'i': {
      printf("%s\n", optarg);
      char *dirpath = realpath(optarg, NULL);
      defer { free(dirpath); }

      printf("Indexing directory: `%s`\n", dirpath);

      Index index;
      da_init(&index);
      for (size_t i = 0; i < index.count; i++) da_init(&index.items[i]);
      fs_process_dir(&index, dirpath);

      index_save(&index, output_name, ISI_JSON);
      index_full_free(&index);
    } break;
    case 'o': {
      output_name = optarg;
    } break;
    default:
      printf("Unsage: %s [-si] [args...]", argv[0]);
      exit(1);
    }
  }

  return 0;
}
