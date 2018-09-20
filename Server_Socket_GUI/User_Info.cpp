#include "User_Info.h"
User_Info::User_Info() { type = 2; }
User_Info::User_Info(char *_Username, char *_Password) {
	for (int i(0); i < 15; i++) {
		type = 2;
		Username[i] = _Username[i];
		Password[i] = _Password[i];
	}
}
bool User_Info::setUsername(char *_Username) {
	if (((string)_Username).length() > 15) {
		return false;
	}
	for (int i(0); i < 15; i++) {
		if (_Username[i] == '\0') {
			Username[i] = _Username[i];
			break;
		}
		if ((_Username[i] > 47 && _Username[i] < 58) || (_Username[i] > 64 && _Username[i] < 91) || (_Username[i] > 96 && _Username[i] < 123)) {
			Username[i] = _Username[i];
			continue;
		}
		else
			return false;
	}
	return true;
}
char* User_Info::getUsername() {
	return Username;
}
bool User_Info::setPassword(char *_Password) {
	if (((string)_Password).length() > 15) {
		return false;
	}
	for (int i(0); i < 15; i++) {
		if (_Password[i] == '\0') {
			Password[i] = _Password[i];
			break;
		}
		if ((_Password[i] > 47 && _Password[i] < 58) || (_Password[i] > 64 && _Password[i] < 91) || (_Password[i] > 96 && _Password[i] < 123)) {
			Password[i] = _Password[i];
			continue;
		}
		else
			return false;
	}
	return true;
}
char* User_Info::getPassword() {
	return Password;
}
void User_Info::setType(char _Type) {
	type = _Type;
	return;
}
//0-inline, 1-offline, 2-unused
char User_Info::getType() {
	return type;
}
bool User_Info::setIP(char *_IP) {
	if (((string)_IP).length() > 15) {
		return false;
	}
	for (int i(0); i < 15; i++) {
		if (_IP[i] == '\0') {
			IP[i] = _IP[i];
			break;
		}
		IP[i] = _IP[i];
	}
	return true;
}
char* User_Info::getIP() {
	return IP;
}
bool User_Info::setPort(int _Port) {
	Port = _Port;
	return true;
}
int User_Info::getPort() {
	return Port;
}
User_Info::~User_Info() {};