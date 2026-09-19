#include "Server.h"
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>

bool Server::StartServer() {
  socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_file_descriptor == -1) {
    cout << "Socket creation failed" << endl;
    return false;
  }

  serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddress.sin_port = htons(PORT);
  serveraddress.sin_family = AF_INET;

  bind_status = bind(socket_file_descriptor, (struct sockaddr *)&serveraddress,
                     sizeof(serveraddress));

  if (bind_status == -1) {
    cout << "Socket binding failed" << endl;
    return false;
  }

  return true;
}

bool Server::Listening() {
  connection_status = listen(socket_file_descriptor, 5);

  if (connection_status == -1) {
    cout << "Socket is unable to listen for new connection" << endl;
    return 0;
  } else {
    cout << "Server is listening for new connection" << endl;
  }

  length = sizeof(client);

  connection =
      accept(socket_file_descriptor, (struct sockaddr *)&client, &length);

  if (connection == -1) {
    cout << "Server is unable to accept the data from client" << endl;
    return false;
  }

  return true;
}

void Server::StopServer() {
  close(socket_file_descriptor);
  cout << "Server has been stopped" << endl;
}

string Server::GetMessage() {
  bzero(message, MESSAGE_LENGTH);
  read(connection, message, sizeof(message));
  return message;
}

void Server::SendMessage(char *_message) {
  strcpy(message, _message);
  bzero(message, MESSAGE_LENGTH);
  ssize_t bytes = write(connection, message, sizeof(message));
  if (bytes >= 0) {
    cout << "Data successfully sent to the client" << endl;
  }
}
