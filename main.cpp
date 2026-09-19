#include <iostream>
#include <string>
#include "Server.h"


using namespace std;

int main()
{
    Server server;

    string mess;

    while(server.StartServer() && server.Listening())
    {
        mess = server.GetMessage();

        cout << mess << endl;

        server.SendMessage("received");
    }

    server.StopServer();

    return 0;
}
