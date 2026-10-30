#include <iostream>
#include <string>
#include <vector>
#include <csignal>
#include "Server.h"
#include "MessageManager.h"
#include "UserManager.h"
#include "ChatManager.h"

using namespace std;

int main()
{
    Server server;
    UserManager userManager;
    ChatManager chatManager;

    vector<string> command;

    if (!server.StartServer())
    {
        return 1;
    }

    if (!server.Listening())
    {
        return 1;
    }

    signal(SIGPIPE, SIG_IGN);

    while(true)
    {
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

            int result = userManager.Register(command[1], command[2]);

            if (result == 0)
            {
                server.SendMessage(mpsen({"SUCCESS", "Registration success"}).c_str());
            }
            else if (result == 1)
            {
                server.SendMessage(mpsen({"ERROR", "This name is using"}).c_str());
            }
            else
            {
                server.SendMessage(mpsen({"ERROR", "Database error"}).c_str());
            }
        }
        else if (command[0] == "AUTHORIZE")
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
        else if (command[0] == "DELACCOUNT")
        {
            if (command.size() < 3)
            {
                continue;
            }
            
            int result = userManager.Deleteac(command[1], command[2]);

            if (result == 0)
            {
                server.SendMessage(mpsen({"SUCCESS", "Account deleted"}).c_str());
            }
            else if (result == 1)
            {
                server.SendMessage(mpsen({"ERROR", "Wrong name or password"}).c_str());
            }
            else
            {
                server.SendMessage(mpsen({"ERROR", "Database error"}).c_str());
            }
        }
        else if (command[0] == "CREATECHAT")
        {
            if (command.size() < 3 || !isNumber(command[2]))
            {
                server.SendMessage(mpsen({"ERROR", "Invalid arguments"}).c_str());
                continue;
            }

            int chatID = chatManager.CreateChat(command[1]);

            if (chatID < 0)
            {
                server.SendMessage(mpsen({"ERROR", "Database error"}).c_str());
                continue;
            }

            int result = chatManager.AddUserToChat(chatID, stoi(command[2]));

            if (result != 0)
            {
                server.SendMessage(mpsen({"ERROR", "Chat created, but user was not added"}).c_str());
                continue;
            }

            server.SendMessage(mpsen({"SUCCESS", "Chat created", to_string(chatID)}).c_str());
        }
        else if (command[0] == "GETCHATS")
        {
            if (command.size() < 2 || !isNumber(command[1]))
            {
                server.SendMessage(mpsen({"ERROR", "Invalid arguments"}).c_str());
                continue;
            }

            vector<int> chatsID = chatManager.GetChats(stoi(command[1]));

            for (int chatID : chatsID)
            {
                server.SendMessage(mpsen({to_string(chatID), chatManager.GetChatName(chatID)}).c_str());
            }

            server.SendMessage(mpsen({"ENDLIST"}).c_str());
        }
        else if (command[0] == "GETCHATNAME")
        {
            if (command.size() < 2 || !isNumber(command[1]))
            {
                server.SendMessage(mpsen({"ERROR", "Invalid arguments"}).c_str());
                continue;
            }

            string chatname = chatManager.GetChatName(stoi(command[1]));

            server.SendMessage(mpsen({"SUCCESS", chatname}).c_str());
        }
        else if (command[0] == "ADDUSERCHAT")
        {
            if (command.size() < 3 || !isNumber(command[1]) || !isNumber(command[2]))
            {
                server.SendMessage(mpsen({"ERROR", "Invalid arguments"}).c_str());
                continue;
            }

            int result = chatManager.AddUserToChat(stoi(command[1]), stoi(command[2]));

            if (result == 0)
            {
                server.SendMessage(mpsen({"SUCCESS", "User has been added"}).c_str());
            }
            else if (result == 1)
            {
                server.SendMessage(mpsen({"ERROR", "User is already in this chat"}).c_str());
            }
            else
            {
                server.SendMessage(mpsen({"ERROR", "Database error"}).c_str());
            }
        }
        else if (command[0] == "GETUSERSCHAT")
        {
            if (command.size() < 2 || !isNumber(command[1]))
            {
                server.SendMessage(mpsen({"ERROR", "Invalid arguments"}).c_str());
                continue;
            }

            vector<string> users = chatManager.GetUsersChat(stoi(command[1]));

            for (const string& user : users)
            {
                server.SendMessage(mpsen({user}).c_str());
            }

            server.SendMessage(mpsen({"ENDLIST"}).c_str());
        }
        else if (command[0] == "SENDMSG")
        {
            if (command.size() < 4 || !isNumber(command[1]) || !isNumber(command[2]))
            {
                server.SendMessage(mpsen({"ERROR", "Invalid arguments"}).c_str());
                continue;
            }

            int result = chatManager.AddMessage(stoi(command[1]), stoi(command[2]), command[3]);

            if (result == 0)
            {
                server.SendMessage(mpsen({"SUCCESS", "Message sent"}).c_str());
            }
            else
            {
                server.SendMessage(mpsen({"ERROR", "Database error"}).c_str());
            }
        }
        else if (command[0] == "GETMSGS")
        {
            if (command.size() < 2 || !isNumber(command[1]))
            {
                server.SendMessage(mpsen({"ERROR", "Invalid arguments"}).c_str());
                continue;
            }

            vector<string> messages = chatManager.GetMessages(stoi(command[1]));

            for (const string& message : messages)
            {
                server.SendMessage(mpsen({message}).c_str());
            }

            server.SendMessage(mpsen({"ENDMSGS"}).c_str());
        }
        else
        {
            server.SendMessage(mpsen({"ERROR", "The command is not recognized"}).c_str());
        }
    }

    server.StopServer();

    return 0;
}
