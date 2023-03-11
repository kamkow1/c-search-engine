#ifndef SERVER_H
#define SERVER_H

#include "onion/onion.h"

onion_connection_status server_query_results(void *_, onion_request *req, onion_response *res);
void server_run(void);

#endif // SERVER_H
