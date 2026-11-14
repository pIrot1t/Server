#include "Server.h"
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>

using namespace std;

bool Server::StartServer()
{
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_file_descriptor == -1)
    {
        cout << "Socket creation failed" << endl;
        return false;
    }

    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;

    bind_status = bind(socket_file_descriptor, (struct sockaddr *)&serveraddress, sizeof(serveraddress));

    if (bind_status == -1)
    {
        cout << "Socket binding failed" << endl;
        return false;
    }

    return true;
}

bool Server::Listening()
{
    connection_status = listen(socket_file_descriptor, 5);

    if (connection_status == -1)
    {
        cout << "Socket is unable to listen for new connection" << endl;
        return 0;
    }
    else
    {
        cout << "Server is listening for new connection" << endl;
    }

    length = sizeof(client);

    connection = accept(socket_file_descriptor, (struct sockaddr *)&client, &length);

    if (connection == -1)
    {
        cout << "Server is unable to accept the data from client" << endl;
        return false;
    }

  return true;
}

void Server::StopServer()
{
    close(socket_file_descriptor);
    cout << "Server has been stopped" << endl;
}

string Server::GetMessage()
{
    size_t end;

    while ((end = buffer.find('\n')) == string::npos)
    {
        ssize_t bytes = read(connection, message, sizeof(message));

        if (bytes <= 0)
        {
            buffer.clear();
            return "";
        }

        buffer.append(message, bytes);
    }

    string line = buffer.substr(0, end + 1);
    buffer.erase(0, end + 1);
    return line;
}

void Server::SendMessage(const char *_message)
{
    size_t total = strlen(_message);
    size_t sent = 0;

    while (sent < total)
    {
        ssize_t bytes = write(connection, _message + sent, total - sent);

        if (bytes <= 0)
        {
            cout << "Failed to send data to the client" << endl;
            return;
        }

        sent += bytes;
    }

    cout << "Data successfully sent to the client" << endl;
}
