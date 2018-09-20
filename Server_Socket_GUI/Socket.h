#pragma once
#include <cstdio>
#include <winsock2.h>
#include <list>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "User_Info.h"
#define MAXCONN 5
#define BUFLEN 255

using namespace std;
void Server_Socket();
void encode(char *, int);
bool Socket_Type(SOCKET &sock, int Port = 0, bool Type = false, char *Addr = INADDR_ANY);
bool initial(ifstream &in_f, int len);
//
//typedef list<SOCKET> ListCONN;
//typedef list<SOCKET> ListConErr;
//
//class my_Socket {
//	SOCKET socket;
//	SOCKET(*Socket)(int, int, int);
//public:
//	my_Socket();
//	
//	my_Socket(const char *type = "TCP");
//
//};
