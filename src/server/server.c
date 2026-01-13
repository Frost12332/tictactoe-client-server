

#include "../shared/buffer.h"
#include "../shared/base.h"
#include "../shared/list.h"

#include "transport_api.h"


typedef struct client_context client_context_t;


typedef struct 
{
    client_context_t* first_player;
    client_context_t* second_player;
}game_session_context_t;



typedef struct client_context
{
    int client_fd;

} client_context_t;


int main()
{
    int received_event;

    if (start_server())
        exit(EXIT_FAILURE);

    for(;;)
    {
        setup_timeout();
        prepare_fd_sets();

        /*pselect if need signal*/
        received_event = select(server_context.max_d + 1, 
                        &server_context.read_fds,
                        &server_context.write_fds,
                        NULL, &server_context.timeout);

        if(received_event == 0)
            printf("Time out\n");
        else if (received_event > 0)
            handle_received_event(received_event);
    }
    
    return EXIT_SUCCESS;
}