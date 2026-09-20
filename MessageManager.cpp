#include <string>
#include <vector>

using namespace std;

vector<string> mprec(string message)
{
    vector<string> command;

    string section;

    int i = 0;
    while (i < message.size())
    {
        if (message[i] == *"|")
        {
            command.push_back(section);
            i++;
        }

        section += message[i];

        if (section == "\n")
        {
            break;
        }

        i++;
    }

    return command;
}

string mpsen(vector<string> result)
{
    string message;

    for (vector<string>::iterator it = result.begin(); it < result.end(); it++)
    {
        message += *it + "|";
    }

    message += "\n";

    return message;
}
