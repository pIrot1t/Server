#include <cstring>
#include <iostream>
#include <string>
#include "Server.h"


using namespace std;

int main()
{
    Server server;

    string mess;

    if (!server.StartServer())
    {
        return 1;
    }

    while(server.Listening())
    {
        mess = server.GetMessage();

        cout << mess << endl;

        if (!strncmp(mess.c_str(), "end", 3))
        {
            break;
        }

        server.SendMessage("received");
    }

    server.StopServer();

    return 0;
}
