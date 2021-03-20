/*
    C++ Socket Client
*/
#include <iostream> //cout
#include <stdio.h> //printf
#include <string.h> //strlen
#include <string> //string
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr
#include <netdb.h> //hostent

using namespace std;

/*
    TCP Client class
*/
class TcpClient
{
    private:
        int sock;
        std::string address;
        string response_data = "";
        int port;
        struct sockaddr_in server;

    public:
        TcpClient();
        bool conn(string, int);
        bool send_data(string data);
        string receive(int);
};