#pragma once
#include <string>
using namespace std;
class User_Info {
	char type;			//0-inline, 1-offline, 2-unused
	char Username[16];
	char Password[16];
	int Port;
	char IP[16];
public:
	User_Info();
	User_Info(char *_Username, char *_Password);
	bool setUsername(char *_Username);
	char* getUsername();
	bool setPassword(char *_Password);
	char* getPassword();
	void setType(char _Type);
	char getType();
	bool setPort(int _Port);
	int getPort();
	bool setIP(char *_IP);
	char *getIP();
	~User_Info();
};