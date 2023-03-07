#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "onion.h"
#include "shortcuts.h"
#include "log.h"

onion *SERVER = NULL;

onion_connection_status query_results(void *_, onion_request *req, onion_response *res)
{
  if (onion_request_get_flags(req)&OR_HEAD) {
    onion_response_write_headers(res);
    return OCS_PROCESSED;
  }
  const char *query = onion_request_get_post(req, "query");
  onion_response_printf(res, "query: %s", query);
  return OCS_PROCESSED;
}

void exit_handler(int _)
{
  ONION_INFO("Exit");
  if (SERVER) onion_listen_stop(SERVER);
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

  signal(SIGTERM, exit_handler);
  signal(SIGINT, exit_handler);

  onion_listen(SERVER);
  onion_free(SERVER);

  return 0;
}
