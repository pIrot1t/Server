#include "UserManager.h"

using namespace std;

UserManager::UserManager()
{
    mysql_init(&mysql);
	if (&mysql == nullptr)
    {
		cout << "Error: can't create MySQL-descriptor" << endl;
	}
 
	if (!mysql_real_connect(&mysql, "localhost", "root", "a657833zb", "testdb", NULL, NULL, 0)) 
    {
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}
	else 
    {
		cout << "Success!" << endl;
	}
 
	mysql_set_character_set(&mysql, "utf8");
	cout << "connection characterset: " << mysql_character_set_name(&mysql) << endl;
}

UserManager::~UserManager()
{
    mysql_close(&mysql);
}
