#include "Server.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
  Server server;

  server.StartServer();

  server.Listening();

  cout << server.GetMessage() << endl;

  server.SendMessage("Hi");

  server.StopServer();
}
