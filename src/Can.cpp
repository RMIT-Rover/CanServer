#include "Can.h"

Can::Can(std::string interface_name, bool is_ctre)
{
    this->interface_name = interface_name;
    this->socket = -1;
    this->is_ctre = is_ctre;
    if(this->is_ctre)
    {
        this->address = this->create_address();
    }
}
			
int Can::phoenix_connect()
{
    return ctre::phoenix::platform::can::SetCANInterface(this->interface_name.c_str());
}

/**
 * calls connect on the specified interface_name
 */
void Can::connect()
{
    if(this->is_ctre)
    {
        this->sock = phoenix_connect();
    }
    else if(this->sock > -1)
    {
        printf("Socket already connected to, call reconnect or close to restart this connection");
    }
    else
    {
        this->sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        did_fail_connection(this->sock, "Failed to create socket for " << interface_name);
        
        int is_bound = bind(this->sock, this->address, sizeof(this->address));
        did_fail_connection(this->sock, std::format("Failed to bind to {}\n", interface_name));
    }
}

int Can::did_fail_connection(int sock, std::string err_msg)
{
    if(sock < 0)
        perror(err_msg.c_str());
        return 1;
    }
}

void Can::close()
{
    int is_closed = close(this->sock);
    did_fail_connection(is_closed, std::format("Failed to close socket bound to {}\n", interface_name));
    
}

void Can::reconnect()
{
    this->close();
    this->connect();
}

sockaddr * Can::create_address()
{
    /* Define connection type*/
    struct ifreq ifr;
    strcpy(ifr.ifr_name, this->interface_name);
    ioctl(s, SIOCGIFINDEX, &ifr);

    /* Define connection address*/
    struct sockaddr * socket_address = new sockaddr();
    socket_address->can_ifindex = ifr.ifr_ifindex;
    socket_address->can_family = AF_CAN;
    return socket_address;
}

void Can::send_data(can_frame frame)
{
    nbytes = write(this->sock, &frame, sizeof(struct can_frame));

}

int Can::recieve_data()
{
    
    nbytes = read(this->sock, &frame, sizeof(struct can_frame));
}