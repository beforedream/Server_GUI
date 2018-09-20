#include "Socket.h"
#include "Server_Socket_GUI.h"
#include <qdebug.h>
list<User_Info> UserList;
extern Server_Socket_GUI *w;

/**
* 函数名：Server_Socket;
* 被调用：Server::start()；
* 调用：系统函数 ::WSAtartup(), ::listen(), ::accept(), ::recv(), strcpy(), sprintf(),
***	   QT中调试函数 qDebug()，
***	   普通函数 Socket_Type()，
***	   类 User_Info, fstream,
***	   对象 Server_Socket_GUI *w(全局变量);
***	   信号函数 Server_Socket_GUI::signal_case();
* 输入：无；
* 输出：无；
* 处理：处理来自客户端的各种请求；
* 局部数据元素：nRC（记录API返回值）， ClientAddr（设置client_sock的 IP 和 port）；
* 注释：单独用来处理客户端请求的线程；
*/
void Server_Socket() {
	UserList.clear(); 
	WSADATA wsaData;
	int nRC;
	char BUF[BUFLEN];
	char recvBUF[BUFLEN];
	sockaddr_in ClientAddr;
	SOCKET ServerSocket;
	int nAddrLen = sizeof(sockaddr);
	ifstream in_f("Users.txt", ios::in | ios::binary);
	// initial ;
	initial(in_f, 16);//设立的阈值，用于错误处理
	emit w->signal_fresh();
	in_f.close();
	nRC = WSAStartup(0x0101, &wsaData);
	//初始化时候的socket信息检测；
	if (nRC == WSASYSNOTREADY) {
		qDebug() << "底层网络子系统没有准备好" << endl;
		return;
	}
	else if (nRC == WSAVERNOTSUPPORTED) {
		qDebug() << "Winsock版本信号不支持" << endl;
		return;
	}
	else if (nRC == WSAEINPROGRESS) {
		qDebug() << "组赛式Winsock1.1存在于进程中" << endl;
		return;
	}
	else if (nRC == WSAEPROCLIM) {
		qDebug() << "已经达到Winsock使用量的上限" << endl;
		return;
	}
	else if (nRC == WSAEFAULT) {
		qDebug() << "lpWSAData不是一个有效的指针" << endl;
		return;
	}
	if (wsaData.wVersion != 0x0101) {
		qDebug() << "版本号不正确" << endl;
		WSACleanup();
		return;
	}
	qDebug() << "Server's Winsocket initialed!" << endl;

	// work for TCP;
	if (nRC = Socket_Type(ServerSocket, 5050) == false) {
		closesocket(ServerSocket);
		WSACleanup();
		return;
	}
	nRC = listen(ServerSocket, 5);
	if (nRC == SOCKET_ERROR) {
		qDebug() << "服务器监听失败！" << endl;
		closesocket(ServerSocket);
		WSACleanup();
		return;
	}
	qDebug() << "服务器已经开始监听..." << endl;
	while (true) {
		SOCKET n_Socket = accept(ServerSocket, (LPSOCKADDR)&ClientAddr, &nAddrLen);
		sprintf(recvBUF, "");
		recv(n_Socket, recvBUF, BUFLEN, 0);
		if (recvBUF[0] == '\1') { // 1-login;
			list<User_Info>::iterator i;
			for (i = UserList.begin(); i != UserList.end(); i++) {
				if (!strcmp((*i).getUsername(), recvBUF + 1) && !strcmp((*i).getPassword(), recvBUF + 17)) {//用户名和密码比较，相同则进入if block；
					if ((*i).getType() == '\0') {
						emit (*w).signal_case((*i).getUsername(), 01);
						sprintf(BUF, "%c", '\2');
						send(n_Socket, BUF, 255, 0);
						sprintf(recvBUF, "");
						break;
					}
					(*i).setIP(inet_ntoa((ClientAddr).sin_addr));
					(*i).setType(0);// 0-online, 1-offline, 2-unused;
					emit (*w).signal_case((*i).getUsername(), 02);
					sprintf(BUF, "%c", '\3');
					send(n_Socket, BUF, 255, 0);
					sprintf(recvBUF, "");
					break;
				}
			}
			if (i == UserList.end()) {
				emit (*w).signal_case(recvBUF + 1, 03);
				sprintf(BUF, "%c", '\1');
				send(n_Socket, BUF, 255, 0);
				sprintf(recvBUF, "");
			}
		}
		if (recvBUF[0] == '\2') { //2-SignUp;
			list<User_Info>::iterator i;
			for (i = UserList.begin(); i != UserList.end(); i++) {
				if (!strcmp((*i).getUsername(), recvBUF + 1)) {
					emit (*w).signal_case((*i).getUsername(), 11);
					sprintf(BUF, "%c", '\1');	// 表示已存在该用户；
					send(n_Socket, BUF, 255, 0);
					break;
				}
			}
			if (i == UserList.end()) {
				sprintf(BUF, "%c", '\2');
				User_Info User_Add;
				User_Add.setUsername(recvBUF + 1);
				User_Add.setPassword(recvBUF + 17);
				User_Add.setType('\1');
				User_Add.setIP("");
				User_Add.setPort(0);
				UserList.insert(UserList.end(), User_Add);
				send(n_Socket, BUF, 255, 0);
				char temp1[16];
				strcpy(temp1, User_Add.getUsername());
				encode(temp1, 16);
				char temp2[16];
				strcpy(temp2, User_Add.getPassword());
				encode(temp2, 16);
				ofstream of("Users.txt", ios::app | ios::binary);
				of.write(temp1, 16);
				of.write(temp2, 16);
				of.close();
				emit (*w).signal_case(User_Add.getUsername(), 12);
				emit w->signal_fresh();
			}
		}
		if (recvBUF[0] == '\3') { //3-findPassword;
			list<User_Info>::iterator i;
			for (i = UserList.begin(); i != UserList.end(); i++) {
				if (!strcmp((*i).getUsername(), recvBUF + 1)) {
					emit (*w).signal_case((*i).getUsername(), 21);
					sprintf(BUF, "%c%s", '\2', (*i).getPassword());	// 表示确实存在该用户；
					send(n_Socket, BUF, 255, 0);
					sprintf(recvBUF, "");
					break;
				}
			}
			if (i == UserList.end()) {
				emit (*w).signal_case(recvBUF + 1, 22);
				sprintf(BUF, "%c", '\1');
				send(n_Socket, BUF, 255, 0);
				sprintf(recvBUF, "");
			}
		}
		if (recvBUF[0] == '\4') { // 4-get Users list;
			list<User_Info>::iterator i;
			int j(2);
			int k(0);
			for (i = UserList.begin(); i != UserList.end(); i++, j += 17, k++) {
				sprintf(BUF + j, "%c%s", (*i).getType(), (*i).getUsername());
			}
			sprintf(BUF, "%c", k);
			send(n_Socket, BUF, 255, 0);
			sprintf(recvBUF, "");
		}
		if (recvBUF[0] == '\5') { // 5-setPort;
			list<User_Info>::iterator i;
			for (i = UserList.begin(); i != UserList.end(); i++) {
				if (!strcmp((*i).getUsername(), recvBUF + 1)) {
					(*i).setPort(atoi(recvBUF + 17));
					emit w->signal_fresh();
					sprintf(recvBUF, "");
					break;
				}
			}
		}
		if (recvBUF[0] == '\6') { // 6-get IP & Port;
			list<User_Info>::iterator i;
			for (i = UserList.begin(); i != UserList.end(); i++) {
				if (!strcmp((*i).getUsername(), recvBUF + 1)) {
					if ((*i).getType() == 0) {
						sprintf(BUF, "\2");
						emit (*w).signal_case(recvBUF + 1, 31, (*i).getUsername());
					}
					else {
						sprintf(BUF, "\3");
						emit (*w).signal_case(recvBUF + 1, 32, (*i).getUsername());
					}
					sprintf(BUF + 1, "%s", (*i).getIP());
					sprintf(BUF + 17, "%d", (*i).getPort());
					break;
				}
			}
			if (i == UserList.end()) {
				sprintf(BUF, "\1");
				emit (*w).signal_case(recvBUF + 1, 33, (*i).getUsername());
			}
			send((n_Socket), BUF, BUFLEN, 0);
		}
		if (recvBUF[0] == '\7') { // 7-get offline message;
			list<User_Info>::iterator i;
			for (i = UserList.begin(); i != UserList.end(); i++) {
				if (!strcmp((*i).getUsername(), recvBUF + 1)) {
					ifstream of(recvBUF + 1, ios::in | ios::binary | ios::beg);
					while (of.peek() != EOF) {
						sprintf(BUF, "\1");
						of.read(BUF + 1, 16);
						encode(BUF + 1, 16);
						of.read(BUF + 17, BUFLEN - 33);
						encode(BUF + 17, BUFLEN - 33);
						send((n_Socket), BUF, BUFLEN, 0);
					}
					sprintf(BUF, "\2");
					send((n_Socket), BUF, BUFLEN, 0);
					of.close();
					ofstream a(recvBUF + 1, ios::binary);
					a.close();
					break;
				}
			}
			sprintf(recvBUF, "");
		}
		if (recvBUF[0] == '\10') { // 10-save offline message;
			list<User_Info>::iterator i;
			emit w->signal_case(recvBUF + 1, 41, recvBUF + 17);
			for (i = UserList.begin(); i != UserList.end(); i++) {
				if (!strcmp((*i).getUsername(), recvBUF + 17)) {
					break;
				}
			}
			if ((*i).getType() == '\0') {
				sprintf(BUF, "\2%s", (*i).getIP());
				sprintf(BUF + 17, "%d", (*i).getPort());
				send(n_Socket, BUF, 255, 0);
				sprintf(recvBUF, "");
			}
			else {
				ofstream of(recvBUF + 17, ios::app | ios::binary);
				encode(recvBUF + 1, 16);
				encode(recvBUF + 33, BUFLEN - 33);
				of.write(recvBUF + 1, 16);
				of.write(recvBUF + 33, BUFLEN - 33);
				of.close();
				sprintf(BUF, "\1");
				send(n_Socket, BUF, 255, 0);
				sprintf(recvBUF, "");
			}
		}
		if (recvBUF[0] == '\20') { // 9-offline;
			list<User_Info>::iterator i;
			for (i = UserList.begin(); i != UserList.end(); i++) {
				if (!strcmp((*i).getUsername(), recvBUF + 1)) {
					(*i).setType(1);
					emit (*w).signal_fresh();
					emit (*w).signal_case((*i).getUsername(), 51);
					sprintf(recvBUF, "");
					sprintf(BUF, "%c", '\20');
					SOCKET sock;
					sockaddr_in Addr;
					Addr.sin_family = AF_INET;
					Addr.sin_port = htons((*i).getPort());
					Addr.sin_addr.S_un.S_addr = ClientAddr.sin_addr.S_un.S_addr;
					Socket_Type(sock);
					if (connect(sock, (SOCKADDR *)&Addr, (int)sizeof(Addr)) == SOCKET_ERROR) {
						qDebug() << "无法连接到客户端监听端" << endl;
						closesocket(sock);
						break;
					}
					else {
						send(sock, BUF, 255, 0);
					}
					closesocket(sock);
				}
			}
			sprintf(recvBUF, "");
		}
		if (recvBUF[0] == '\21') {//服务器自己使用的退出命令
			break;
		}
		closesocket(n_Socket);
	}
	closesocket(ServerSocket);
	WSACleanup();
	return;
}

/**
* 函数名：Socket_Type;
* 被调用：所有使用了socket的函数；
* 调用：系统API接口 ::bind(), ::socket() 和 QT中调试函数 qDebug();
* 输入：Addr绑定地址，Port绑定端口， Type：false-TCP，true-UDP, sock操作对象；
* 输出：true-成功，false-失败；
* 处理：将输入的socket绑定好IP，port并且设置好sock模式（TCP / UDP）；
* 局部数据元素：nRC（记录API返回值）， sockAddr（设置sock的 IP 和 port）；
* 注释：自定义的较底层的函数，被多个模块调用；
*/
bool Socket_Type(SOCKET &sock, int Port, bool Type, char *Addr) {
	int nRC(0);
	SOCKADDR_IN sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(Port);
	if (Addr == INADDR_ANY) {
		sockAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	}
	else
		sockAddr.sin_addr.S_un.S_addr = inet_addr(Addr);
	if (Type) {//UDP绑定；
		sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		nRC = ::bind(sock, (LPSOCKADDR)&sockAddr, sizeof(sockAddr));
		if (nRC == SOCKET_ERROR) {
			qDebug() << "Socket_UDP 绑定失败" << endl;
			return false;
		}
		qDebug() << "Socket_UDP 绑定成功" << endl;
	}
	else {//TCP绑定；
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		nRC = ::bind(sock, (LPSOCKADDR)&sockAddr, sizeof(sockAddr));
		if (nRC == SOCKET_ERROR) {
			qDebug() << "Socket_TCP 绑定失败" << endl;
			return false;
		}
		qDebug() << "ClientSocket_TCP 绑定成功" << endl;
	}
	return true;
}

/**
* 函数名：initial;
* 被调用：Server_Socket()；
* 调用：User_Info(class), fstream(class), UserList(list);
* 输入：in_f(用户信息文件指针), len（用户个数上限）；
* 输出：true-成功，false-失败；
* 处理：打开用户数据文件并且加载到UserList中；
* 局部数据元素：_User(用于缓冲用户数据)；
* 注释：初始化用户列表；
*/
bool initial(ifstream &in_f, int len) {
	User_Info _User;
	for (int i(0); (in_f.peek() != EOF) && i < len; i++){//文件尾或者文件用户过多（错误导致）退出；
		char temp[16];
		in_f.read(temp, 16);
		encode(temp, 16);		//解密；
		if (_User.setUsername(temp) == false)
			return false;
		in_f.read(temp, 16);
		encode(temp, 16);
		if (_User.setPassword(temp) == false)
			return false;
		_User.setType('\1');
		_User.setIP("");
		_User.setPort(0);
		UserList.insert(UserList.end(), _User);			//加入UserList；
	}
	return true;
}

/**
* 函数名：encode;
* 被调用：initial(), Server_Socket()；
* 调用：无;
* 输入：字符串 in， 字符串长度 len；
* 输出：加密（解密）后的字符串；
* 处理：将输入的字符串按给定的长度加密解密；
* 局部数据元素：计数器 i；
* 注释：加密解密函数；
*/
void encode(char *in, int len) {
	for (int i(0); i < len; i++) {
		in[i] ^= 0xFF;//Key, 加密的key（虽然是十分简单的异或加密）；
	}
}