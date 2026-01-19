#include "../shared/buffer.h"
#include "../shared/base.h"
#include "../shared/list.h"

#include "transport_api.h"


int main()
{
    int received_event;

    /*multiple call subscribe on event for callback*/

    if (start_transport_level())
    {
        stop_transport_level();
        exit(EXIT_FAILURE);
    }

    for(;;)
    {
        received_event = await_event();
        /*or if received_event == -1 error*/
        if(received_event == 0)
            printf("Time out\n");
        else if (received_event > 0)
        {
            /*is_receive_new_connection if true call accept_new_connection*/
            /*next using sessions list check if fd ready for handle, and call handle_received_event*/
        }
    }
    stop_transport_level();
    return EXIT_SUCCESS;
}