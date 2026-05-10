#ifndef TRANSPORT_API_CLIENT_H
#define TRANSPORT_API_CLIENT_H

#include <netinet/in.h>
#include <arpa/inet.h>



/*just connection and receive new message or send them, doesnt store any info*/

int start_transport_level_client();

void stop_transport_level_client();




#endif