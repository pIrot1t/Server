#include <iostream>
#include <string>
#include <vector>
#include <csignal>
#include "Server.h"
#include "MessageManager.h"
#include "UserManager.h"


using namespace std;

int main()
{
    Server server;
    UserManager userManager;

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
        signal(SIGPIPE, SIG_IGN);
        string raw = server.GetMessage();
        if (raw.empty())
        {
            break;
        }
        command = mprec(raw);
        
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
            if (command.size() < 3)
            {
                continue;
            }

            if(userManager.Register(command[1], command[2]) == 1)
            {
                server.SendMessage(mpsen({"ERROR", "This name is using"}).c_str());
            }
            else
            {
                server.SendMessage(mpsen({"SUCCESS", "Registration success"}).c_str());
            }
        }
        else if (command[0] == "AUTHORIZ")
        {
            if (command.size() < 3)
            {
                continue;
            }

            int id = userManager.Authoriz(command[1], command[2]);
            if (id < 0)
            {
                server.SendMessage(mpsen({"ERROR", "Wrong name or password"}).c_str());
            }
            else
            {
                server.SendMessage(mpsen({"SUCCESS", "Authorization success", to_string(id)}).c_str());
            }
        }
        else if (command[0] == "DELETEAC")
        {
            if (command.size() < 3)
            {
                continue;
            }
            
            if(userManager.Deleteac(command[1], command[2]) == 1)
            {
                server.SendMessage(mpsen({"ERROR", "Wrong name or password"}).c_str());
            }
            else
            {
                server.SendMessage(mpsen({"SUCCESS", "Account deleted"}).c_str());
            }
        }
        else
        {
            server.SendMessage(mpsen({"ERROR", "The command is not recognized"}).c_str());
        }
    }

    server.StopServer();

    return 0;
}
