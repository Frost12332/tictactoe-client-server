#ifndef TRANSPORT_API_H
#define TRANSPORT_API_H


#include <time.h>
#include <sys/select.h>

#define DEFAULT_TCP 0
#define ALL_NETWORK "0.0.0.0"

#define TIMEOUT_SEC 5
#define TIMEOUT_NSEC 0


enum
{
    queue_connection = 5
};

typedef struct 
{
    int max_d;
    int socket_fd;

    struct timespec timeout;
    fd_set read_fds, write_fds;
}transport_context_t;

extern transport_context_t server_context;



int start_server();

void setup_timeout();

void prepare_fd_sets();

void handle_received_event();

#endif