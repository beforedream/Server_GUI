#include "Server_Socket_GUI.h"
Server server;
extern list<User_Info> UserList;
extern Server_Socket_GUI *w;

/**
* �ࣺServer_Socket_GUI��
* ��������Server_Socket_GUI;
* ���ࣺUi_Server_Socket_GUIClass������ui�ĳ�ʼ������ QWidget��
* ������:�ޣ�
* ���룺Server_Socket_GUI *this�� QWidget *parent��
* ������ޣ�
* ����������棬 ��ʼ��״̬�� ������
* ע�ͣ����캯����
*/
Server_Socket_GUI::Server_Socket_GUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	QStringList header;
	header << QString::fromLocal8Bit("�û�") << QString::fromLocal8Bit("����״̬") << QString::fromLocal8Bit("IP��ַ") << QString::fromLocal8Bit("�˿ں�");
	this->ui.tableWidget->setHorizontalHeaderLabels(header);
	this->ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
	this->setWindowTitle(QString::fromLocal8Bit("��������Ϣ��..."));
	QObject::connect(this, SIGNAL(signal_case(char *, int, char *)), this, SLOT(Case(char *, int, char *)));
	QObject::connect(this, SIGNAL(signal_fresh()), this, SLOT(Fresh()));
}

/**
* �ࣺServer_Socket_GUI��
* ��������Start;
* ���ࣺUi_Server_Socket_GUIClass������ui�ĳ�ʼ������ QWidget��
* ������:�ޣ�
* ���룺Server_Socket_GUI *this��
* ������ޣ�
* ��������Server::start()��
* ע�ͣ�����������
*/
void Server_Socket_GUI::Start() {
	if (server.start() == false) {
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�������Ѿ�����������!"));
		return;
	}
	this->setWindowTitle(QString::fromLocal8Bit("������������..."));
	return;
}

/**
* �ࣺServer_Socket_GUI��
* ��������Stop;
* ���ࣺUi_Server_Socket_GUIClass������ui�ĳ�ʼ������ QWidget��
* ������:�ޣ�
* ���룺Server_Socket_GUI *this��
* ������ޣ�
* ��������Server::stop()��
* ע�ͣ���ֹ������
*/
void Server_Socket_GUI::Stop() {
	if (server.stop() == false) {
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�������Ѿ�ֹͣ����!"));
		return;
	}
	this->setWindowTitle(QString::fromLocal8Bit("��������Ϣ��..."));
	return;
}

/**
* �ࣺServer_Socket_GUI��
* ��������Close;
* ���ࣺUi_Server_Socket_GUIClass������ui�ĳ�ʼ������ QWidget��
* ������:�ޣ�
* ���룺Server_Socket_GUI *this��
* ������ޣ�
* ��������close������
* ע�ͣ��رմ��ڣ�
*/
void Server_Socket_GUI::Close() {
	close();
}

/**
* �ࣺServer_Socket_GUI��
* ��������Fresh;
* ���ࣺUi_Server_Socket_GUIClass������ui�ĳ�ʼ������ QWidget��
* ������:�ޣ�
* ���룺Server_Socket_GUI *this��
* ������ޣ�
* ����ˢ���û��б���ʾ��
* ע�ͣ�ˢ�£�
*/
void Server_Socket_GUI::Fresh() {
	list<User_Info>::iterator i;
	int j = 1;
	this->ui.tableWidget->setRowCount(UserList.size() + 1);
	for (i = UserList.begin(); i != UserList.end(); i++) {
		if ((*i).getType() == 0) {
			this->ui.tableWidget->setItem(j, 0, new QTableWidgetItem(QString((*i).getUsername()), Qt::AlignCenter));
			this->ui.tableWidget->setItem(j, 1, new QTableWidgetItem(QString::fromLocal8Bit("����"), Qt::AlignCenter));
			this->ui.tableWidget->setItem(j, 2, new QTableWidgetItem(QString((*i).getIP()), Qt::AlignCenter));
			this->ui.tableWidget->setItem(j, 3, new QTableWidgetItem(QString::number((*i).getPort()), Qt::AlignCenter));
			j++;
		}
	}
	for (i = UserList.begin(); i != UserList.end(); i++) {
		if ((*i).getType() == 1) {
			this->ui.tableWidget->setItem(j, 0, new QTableWidgetItem(QString((*i).getUsername()), Qt::AlignCenter));
			this->ui.tableWidget->setItem(j, 1, new QTableWidgetItem(QString::fromLocal8Bit("����"), Qt::AlignCenter));
			this->ui.tableWidget->setItem(j, 2, new QTableWidgetItem(QString("0.0.0.0"), Qt::AlignCenter));
			this->ui.tableWidget->setItem(j, 3, new QTableWidgetItem(QString::number(0), Qt::AlignCenter));
			j++;
		}
	}
}

/**
* �ࣺServer_Socket_GUI��
* ��������Case;
* ���ࣺUi_Server_Socket_GUIClass������ui�ĳ�ʼ������ QWidget��
* ������:�ޣ�
* ���룺Server_Socket_GUI *this�� char *username�� �¼�c�� char *Target���û��������󣩣�
* ������ޣ�
* ���������Ӧ����־��Ϣ��
* ע�ͣ�ϵͳ��־��
*/
void Server_Socket_GUI::Case(char *username, int c, char *Target) {
	if (c == 01) {
		this->ui.textBrowser->append(QString(username) + QString::fromLocal8Bit(": ��¼ʧ�ܣ����û��Ѿ��ڱ𴦵�¼��\n"));
	}
	else if (c == 02) {
		this->ui.textBrowser->append(QString(username) + QString::fromLocal8Bit(": ��¼�ɹ���\n"));
	}
	else if (c == 03) {
		this->ui.textBrowser->append(QString(username) + QString::fromLocal8Bit(": ��¼ʧ�ܣ��û������ڻ��������\n"));
	}
	else if (c == 11) {
		this->ui.textBrowser->append(QString(username) + QString::fromLocal8Bit(": ע��ʧ�ܣ����û��Ѿ����ڣ�\n"));
	}
	else if (c == 12) {
		this->ui.textBrowser->append(QString(username) + QString::fromLocal8Bit(": ע��ɹ���\n"));
	}
	else if (c == 21) {
		this->ui.textBrowser->append(QString(username) + QString::fromLocal8Bit(": �һ�����ɹ���\n"));
	}
	else if (c == 22) {
		this->ui.textBrowser->append(QString(username) + QString::fromLocal8Bit(": �һ�����ʧ�ܣ����û������ڣ�\n"));
	}
	else if (c == 31) {
		this->ui.textBrowser->append(QString::fromLocal8Bit("δ֪���û�") + QString::fromLocal8Bit("�����ȡ") + QString(Target) + QString::fromLocal8Bit(" ��IP�Ͷ˿ڳɹ���\n"));
	}
	else if (c == 32) {
		this->ui.textBrowser->append(QString::fromLocal8Bit("δ֪���û�") + QString::fromLocal8Bit("�����ȡ") + QString(Target) + QString::fromLocal8Bit(" ��IP�Ͷ˿�ʧ�ܣ�δ���ߣ�\n"));
	}
	else if (c == 33) {
		this->ui.textBrowser->append(QString::fromLocal8Bit("δ֪���û�") + QString::fromLocal8Bit("�����ȡ") + QString(Target) + QString::fromLocal8Bit(" ��IP�Ͷ˿�ʧ�ܣ����޴��û���\n"));
	}
	else if (c == 41) {
		this->ui.textBrowser->append(QString(username) + QString::fromLocal8Bit("��") + QString(Target) + QString::fromLocal8Bit("����������Ϣ��\n"));
	}
	else if (c == 51) {
		this->ui.textBrowser->append(QString(username) + QString::fromLocal8Bit(": ���ߣ�\n"));
	}
}

/**
* �ࣺServer_Socket_GUI��
* ��������closeEvent;
* ���ࣺUi_Server_Socket_GUIClass������ui�ĳ�ʼ������ QWidget��
* ������:�ޣ�
* ���룺Server_Socket_GUI *this�� QCloseEvent *event��
* ������ޣ�
* ����ѯ���Ƿ��˳��� YES-��ֹ�����̲߳��ҹرմ��ڣ� NO-���Թر��źţ�
* ע�ͣ����ش��ڹرպ�����
*/
void Server_Socket_GUI::closeEvent(QCloseEvent *event) {
	QMessageBox::StandardButton button;
	button = QMessageBox::question(this, QString::fromLocal8Bit("�˳�����"), QString::fromLocal8Bit("ȷ���˳�����"), QMessageBox::Yes | QMessageBox::No);
	if (button == QMessageBox::No)
	{
		event->ignore(); // �����˳��źţ������������  
	}
	else if (button == QMessageBox::Yes)
	{
		server.stop();
		event->accept(); // �����˳��źţ������˳�  
	}
	return;
}

/*
* �ࣺServer��
* ��������Server;
* ���ࣺ�ޣ�
* ������:�ޣ�
* ���룺server *this��
* ������ޣ�
* ��������Server����ʼ��һЩ��������ô洢�ռ䣻
* ע�ͣ����캯����
*/
Server::Server() :State(false), server(nullptr) {};

/**
* �ࣺServer��
* ��������start;
* ���ࣺ�ޣ�
* ������:�ޣ�
* ���룺Server *this��
* �����true-�ɹ������̣߳� false-�����߳�ʧ�ܣ�ԭ���Ѿ���������
* �����߳�û�п������������̣߳�����ֱ�ӷ��أ�
* ע�ͣ��߳�����������
*/
bool Server::start() {
	if (this->State == false) {
		this->State = true;
		this->server = new ::thread(&Server_Socket);
		return true;
	}
	return false;
}

/**
* �ࣺServer��
* ��������state;
* ���ࣺ�ޣ�
* ������:�ޣ�
* ���룺server *this��
* �����true-����״̬�� false-�ر�״̬��
* ������ȡ�����߳�״̬��
* ע�ͣ��߳�״̬������
*/
bool Server::state() {
	return this->State;
}

/**
* �ࣺServer��
* ��������stop;
* ���ࣺ�ޣ�
* ������:�ޣ�
* ���룺server *this��
* �����true-�ر��̳߳ɹ��� false-�ر��߳�ʧ�ܣ�
* ������ֹ�����̣߳�
* ע�ͣ��߳���ֹ������
*/
bool Server::stop() {
	if (this->state() == true) {
		UserList.clear();
		emit w->signal_fresh();
		SOCKET sock;
		sockaddr_in addr;
		int len = sizeof(addr);
		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		addr.sin_port = htons(5050);
		Socket_Type(sock);
		if (::connect(sock, (LPSOCKADDR)&addr, len) == SOCKET_ERROR) {
			int a = GetLastError();
			return false;
		}
		char BUF[BUFLEN] = "\21";
		send(sock, BUF, BUFLEN, 0);
		this->server->join();
		this->server = nullptr;
		this->State = false;
		return true;
	}
	return false;
}

/**
* �ࣺServer��
* ��������~Server;
* ���ࣺ�ޣ�
* ������:�ޣ�
* ���룺server *this��
* ������ޣ�
* ��������Server����
* ע�ͣ�����������
*/
Server::~Server() {
	delete this->server;
}