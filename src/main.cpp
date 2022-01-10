#include "Can.h"
#include "tcp.h"

int main(int argc, char **argv) 
{
    /*Initialise sockets with default data*/
    Can * can0 = new Can("can0", true);
    Can * can1 = new Can("can1", false);
    TcpClient * arm_server = new TcpClient();
    /** connect and bind to addresses */
    can0->connect();
    can1->connect();
    arm_server->conn("localhost" , 5555);
    Controller controller = new Controller(can0, can1, arm_server)
    while(true)
    {
        Mode * mode = ModeFactory.buildMode(can1.recieve_data())
        mode->perform(controller);


    }
 /**
    connect to arm_socket
    connect to can_0 socket
    connect to can_1 socket
    start_main loop
    if(mode === "autonomous")
    if(mode === "drive")
    if(mode === "arm")
    write useful stats()
 */
}