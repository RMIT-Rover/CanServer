#include <linux/can.h>
#include <linux/can/raw.h>
#include <iostream> //cout
#include <stdio.h> //printf
#include <string.h> //strlen
#include <string> //string
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr
#include <netdb.h> //hostent
// #include "ctre/Phoenix.h"
#include "ctre/phoenix/platform/Platform.h"
#include "ctre/phoenix/unmanaged/Unmanaged.h"
#include "ctre/phoenix/cci/Unmanaged_CCI.h"

class Can 
{
public:
    Can(std::string interface_name, bool is_ctre);
    void connect();
    int phoenix_connect();
    int did_fail_connection(int socket, std::string err_msg);
    void close();
    void reconnect();
    sockaddr * create_address();
    void send_data(can_frame msg);
    int recieve_data();

private:
    std::string interface_name;
    int sock;
    bool is_ctre;
    sockaddr * address;
}