#include "MessageManager.h"

using namespace std;

vector<string> mprec(string message)
{
    vector<string> command;

    string section;

    for (char sim : message)
    {
        if (sim == '|')
        {
            command.push_back(section);
            section.clear();
        }
        else if (sim == '\n')
        {
            break;
        }
        else
        {
            section += sim;
        }
    }

    if (!section.empty())
    {
        command.push_back(section);
    }

    return command;
}

string mpsen(vector<string> result)
{
    string message;

    for (vector<string>::iterator it = result.begin(); it < result.end(); it++)
    {
        message += *it + '|';
    }

    message += '\n';

    return message;
}
