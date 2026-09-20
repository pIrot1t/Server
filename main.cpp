#include <iostream>
#include <string>
#include <vector>
#include "MessageManager.cpp"
#include "Server.h"


using namespace std;

int main()
{
    Server server;

    vector<string> command;

    if (!server.StartServer())
    {
        return 1;
    }

    if (!server.Listening())
    {
        return 1;
    }

    while(true)
    {
        command = mprec(server.GetMessage());
        
        if (command.empty())
        {
            continue;
        }
        else if (command[0] == "END")
        {
            break;
        }
        else if (command[0] == "REGISTER")
        {
            cout << "Registartion: " << command[1] << " " << command[2] << endl;
            server.SendMessage(mpsen({"SUCCESS", "Registartion success"}).c_str());
        }
        else if (command[0] == "AUTHORIZ")
        {
            cout << "Authorization: " << command[1] << " " << command[2] << endl;
            server.SendMessage(mpsen({"SUCCESS", "Authorization success"}).c_str());
        }
        else if (command[0] == "DELETEAC")
        {
            cout << "Delete account: " << command[1] << " " << command[2] << endl;
            server.SendMessage(mpsen({"SUCCECS", "Account deleted"}).c_str());
        }
        else
        {
            server.SendMessage(mpsen({"ERROR", "The command is not recognized"}).c_str());
        }
    }

    server.StopServer();

    return 0;
}
