#pragma once
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#define MESSAGE_LENGTH 1024
#define PORT 51111

class Server
{
private:
    struct sockaddr_in serveraddress, client;
    socklen_t length;
    int socket_file_descriptor, connection, bind_status, connection_status;
    char message[MESSAGE_LENGTH];

public:
    bool StartServer();

    void StopServer();

    bool Listening();

    std::string GetMessage();

    void SendMessage(const char *message);
};
