#include "Socket.h"
#include "Server_Socket_GUI.h"
#include <qdebug.h>
list<User_Info> UserList;
extern Server_Socket_GUI *w;

/**
* ��������Server_Socket;
* �����ã�Server::start()��
* ���ã�ϵͳ���� ::WSAtartup(), ::listen(), ::accept(), ::recv(), strcpy(), sprintf(),
***	   QT�е��Ժ��� qDebug()��
***	   ��ͨ���� Socket_Type()��
***	   �� User_Info, fstream,
***	   ���� Server_Socket_GUI *w(ȫ�ֱ���);
***	   �źź��� Server_Socket_GUI::signal_case();
* ���룺�ޣ�
* ������ޣ�
* �����������Կͻ��˵ĸ�������
* �ֲ�����Ԫ�أ�nRC����¼API����ֵ���� ClientAddr������client_sock�� IP �� port����
* ע�ͣ�������������ͻ���������̣߳�
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
	initial(in_f, 16);//��������ֵ�����ڴ�����
	emit w->signal_fresh();
	in_f.close();
	nRC = WSAStartup(0x0101, &wsaData);
	//��ʼ��ʱ���socket��Ϣ��⣻
	if (nRC == WSASYSNOTREADY) {
		qDebug() << "�ײ�������ϵͳû��׼����" << endl;
		return;
	}
	else if (nRC == WSAVERNOTSUPPORTED) {
		qDebug() << "Winsock�汾�źŲ�֧��" << endl;
		return;
	}
	else if (nRC == WSAEINPROGRESS) {
		qDebug() << "����ʽWinsock1.1�����ڽ�����" << endl;
		return;
	}
	else if (nRC == WSAEPROCLIM) {
		qDebug() << "�Ѿ��ﵽWinsockʹ����������" << endl;
		return;
	}
	else if (nRC == WSAEFAULT) {
		qDebug() << "lpWSAData����һ����Ч��ָ��" << endl;
		return;
	}
	if (wsaData.wVersion != 0x0101) {
		qDebug() << "�汾�Ų���ȷ" << endl;
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
		qDebug() << "����������ʧ�ܣ�" << endl;
		closesocket(ServerSocket);
		WSACleanup();
		return;
	}
	qDebug() << "�������Ѿ���ʼ����..." << endl;
	while (true) {
		SOCKET n_Socket = accept(ServerSocket, (LPSOCKADDR)&ClientAddr, &nAddrLen);
		sprintf(recvBUF, "");
		recv(n_Socket, recvBUF, BUFLEN, 0);
		if (recvBUF[0] == '\1') { // 1-login;
			list<User_Info>::iterator i;
			for (i = UserList.begin(); i != UserList.end(); i++) {
				if (!strcmp((*i).getUsername(), recvBUF + 1) && !strcmp((*i).getPassword(), recvBUF + 17)) {//�û���������Ƚϣ���ͬ�����if block��
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
					sprintf(BUF, "%c", '\1');	// ��ʾ�Ѵ��ڸ��û���
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
					sprintf(BUF, "%c%s", '\2', (*i).getPassword());	// ��ʾȷʵ���ڸ��û���
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
						qDebug() << "�޷����ӵ��ͻ��˼�����" << endl;
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
		if (recvBUF[0] == '\21') {//�������Լ�ʹ�õ��˳�����
			break;
		}
		closesocket(n_Socket);
	}
	closesocket(ServerSocket);
	WSACleanup();
	return;
}

/**
* ��������Socket_Type;
* �����ã�����ʹ����socket�ĺ�����
* ���ã�ϵͳAPI�ӿ� ::bind(), ::socket() �� QT�е��Ժ��� qDebug();
* ���룺Addr�󶨵�ַ��Port�󶨶˿ڣ� Type��false-TCP��true-UDP, sock��������
* �����true-�ɹ���false-ʧ�ܣ�
* �����������socket�󶨺�IP��port�������ú�sockģʽ��TCP / UDP����
* �ֲ�����Ԫ�أ�nRC����¼API����ֵ���� sockAddr������sock�� IP �� port����
* ע�ͣ��Զ���Ľϵײ�ĺ����������ģ����ã�
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
	if (Type) {//UDP�󶨣�
		sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		nRC = ::bind(sock, (LPSOCKADDR)&sockAddr, sizeof(sockAddr));
		if (nRC == SOCKET_ERROR) {
			qDebug() << "Socket_UDP ��ʧ��" << endl;
			return false;
		}
		qDebug() << "Socket_UDP �󶨳ɹ�" << endl;
	}
	else {//TCP�󶨣�
		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		nRC = ::bind(sock, (LPSOCKADDR)&sockAddr, sizeof(sockAddr));
		if (nRC == SOCKET_ERROR) {
			qDebug() << "Socket_TCP ��ʧ��" << endl;
			return false;
		}
		qDebug() << "ClientSocket_TCP �󶨳ɹ�" << endl;
	}
	return true;
}

/**
* ��������initial;
* �����ã�Server_Socket()��
* ���ã�User_Info(class), fstream(class), UserList(list);
* ���룺in_f(�û���Ϣ�ļ�ָ��), len���û��������ޣ���
* �����true-�ɹ���false-ʧ�ܣ�
* �������û������ļ����Ҽ��ص�UserList�У�
* �ֲ�����Ԫ�أ�_User(���ڻ����û�����)��
* ע�ͣ���ʼ���û��б�
*/
bool initial(ifstream &in_f, int len) {
	User_Info _User;
	for (int i(0); (in_f.peek() != EOF) && i < len; i++){//�ļ�β�����ļ��û����ࣨ�����£��˳���
		char temp[16];
		in_f.read(temp, 16);
		encode(temp, 16);		//���ܣ�
		if (_User.setUsername(temp) == false)
			return false;
		in_f.read(temp, 16);
		encode(temp, 16);
		if (_User.setPassword(temp) == false)
			return false;
		_User.setType('\1');
		_User.setIP("");
		_User.setPort(0);
		UserList.insert(UserList.end(), _User);			//����UserList��
	}
	return true;
}

/**
* ��������encode;
* �����ã�initial(), Server_Socket()��
* ���ã���;
* ���룺�ַ��� in�� �ַ������� len��
* ��������ܣ����ܣ�����ַ�����
* ������������ַ����������ĳ��ȼ��ܽ��ܣ�
* �ֲ�����Ԫ�أ������� i��
* ע�ͣ����ܽ��ܺ�����
*/
void encode(char *in, int len) {
	for (int i(0); i < len; i++) {
		in[i] ^= 0xFF;//Key, ���ܵ�key����Ȼ��ʮ�ּ򵥵������ܣ���
	}
}