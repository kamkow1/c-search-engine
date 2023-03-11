#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stddef.h>

#include "onion/onion.h"
#include "onion/shortcuts.h"
#include "onion/log.h"
#include "onion/response.h"

#include "server.h"
#include "query.h"

onion *SERVER = NULL;

void exit_handler(int _)
{
  ONION_INFO("Exit");
  if (SERVER) {
    onion_listen_stop(SERVER);
    onion_free(SERVER);
  }
}

onion_connection_status server_query_results(void *_, onion_request *req, onion_response *res)
{
  if (onion_request_get_flags(req)&OR_HEAD) {
    onion_response_write_headers(res);
    return OCS_PROCESSED;
  }
  const char *query = onion_request_get_post(req, "query");
  printf("Processing Query: %s\n", query);

  char buffer[1024];
  memset(buffer, 0, sizeof(buffer));
  query_stem(query, buffer);

  printf("Processed Query: %s\n", buffer);

  onion_response_printf(res, "query: %s", buffer);

  return OCS_PROCESSED;
}

void server_run()
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

  onion_url_add(urls, "results", server_query_results);

  onion_listen(SERVER);

  signal(SIGTERM, exit_handler);
  signal(SIGINT, exit_handler);

  onion_free(SERVER);
}

