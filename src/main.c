#include <onion/response.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stddef.h>

#include "onion/onion.h"
#include "onion/shortcuts.h"
#include "onion/log.h"

#include "libstemmer.h"

#define LANG "english"

onion *SERVER = NULL;

void stem_input(const char *input, char *buffer)
{
  const char *charenc = NULL;
  struct sb_stemmer *stemmer = sb_stemmer_new(LANG, charenc);
  const sb_symbol *output = sb_stemmer_stem(stemmer, (sb_symbol*)input, strlen(input));

  for (size_t i = 0; i < strlen((char*)output); i++) buffer[i] = output[i];
  sb_stemmer_delete(stemmer);
}

onion_connection_status query_results(void *_, onion_request *req, onion_response *res)
{
  if (onion_request_get_flags(req)&OR_HEAD) {
    onion_response_write_headers(res);
    return OCS_PROCESSED;
  }
  const char *query = onion_request_get_post(req, "query");
  printf("Processing Query: %s\n", query);

  char buffer[1024];
  memset(buffer, 0, sizeof(buffer));
  stem_input(query, buffer);

  printf("Processed Query: %s\n", buffer);

  onion_response_printf(res, "query: %s", buffer);

  return OCS_PROCESSED;
}

void exit_handler(int _)
{
  ONION_INFO("Exit");
  if (SERVER) {
    onion_listen_stop(SERVER);
    onion_free(SERVER);
  }
}

int main(void)
{
  SERVER = onion_new(O_ONE_LOOP);
  onion_url *urls = onion_root_url(SERVER);

  onion_url_add_static(
      urls, 
      "",
      "<html>"
      "<head>"
      "  <title>C Search Engine</title>"
      "</head>"
      "<body>"
      "  <form method = \"post\" action=\"results\">"
      "    <label for=\"query\">Search:<label>\n"
      "    <input type=\"text\" name=\"query\">"
      "    <input type=\"submit\" value=\"Submit\">"
      "  </form>"
      "</body>"
      "</html>",
      HTTP_OK
  );

  onion_url_add(urls, "results", query_results);

  onion_listen(SERVER);

  signal(SIGTERM, exit_handler);
  signal(SIGINT, exit_handler);

  onion_free(SERVER);

  return 0;
}
