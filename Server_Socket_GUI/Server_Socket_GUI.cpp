#include "Server_Socket_GUI.h"
Server server;
extern list<User_Info> UserList;
extern Server_Socket_GUI *w;

/**
* 类：Server_Socket_GUI；
* 函数名：Server_Socket_GUI;
* 基类：Ui_Server_Socket_GUIClass（包含ui的初始化）， QWidget；
* 派生类:无；
* 输入：Server_Socket_GUI *this， QWidget *parent；
* 输出：无；
* 处理：构造界面， 初始化状态， 参数；
* 注释：构造函数；
*/
Server_Socket_GUI::Server_Socket_GUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	QStringList header;
	header << QString::fromLocal8Bit("用户") << QString::fromLocal8Bit("在线状态") << QString::fromLocal8Bit("IP地址") << QString::fromLocal8Bit("端口号");
	this->ui.tableWidget->setHorizontalHeaderLabels(header);
	this->ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
	this->setWindowTitle(QString::fromLocal8Bit("服务器休息中..."));
	QObject::connect(this, SIGNAL(signal_case(char *, int, char *)), this, SLOT(Case(char *, int, char *)));
	QObject::connect(this, SIGNAL(signal_fresh()), this, SLOT(Fresh()));
}

/**
* 类：Server_Socket_GUI；
* 函数名：Start;
* 基类：Ui_Server_Socket_GUIClass（包含ui的初始化）， QWidget；
* 派生类:无；
* 输入：Server_Socket_GUI *this；
* 输出：无；
* 处理：调用Server::start()；
* 注释：启动监听；
*/
void Server_Socket_GUI::Start() {
	if (server.start() == false) {
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("服务器已经处于运行中!"));
		return;
	}
	this->setWindowTitle(QString::fromLocal8Bit("服务器运行中..."));
	return;
}

/**
* 类：Server_Socket_GUI；
* 函数名：Stop;
* 基类：Ui_Server_Socket_GUIClass（包含ui的初始化）， QWidget；
* 派生类:无；
* 输入：Server_Socket_GUI *this；
* 输出：无；
* 处理：调用Server::stop()；
* 注释：终止监听；
*/
void Server_Socket_GUI::Stop() {
	if (server.stop() == false) {
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("服务器已经停止运行!"));
		return;
	}
	this->setWindowTitle(QString::fromLocal8Bit("服务器休息中..."));
	return;
}

/**
* 类：Server_Socket_GUI；
* 函数名：Close;
* 基类：Ui_Server_Socket_GUIClass（包含ui的初始化）， QWidget；
* 派生类:无；
* 输入：Server_Socket_GUI *this；
* 输出：无；
* 处理：调用close（）；
* 注释：关闭窗口；
*/
void Server_Socket_GUI::Close() {
	close();
}

/**
* 类：Server_Socket_GUI；
* 函数名：Fresh;
* 基类：Ui_Server_Socket_GUIClass（包含ui的初始化）， QWidget；
* 派生类:无；
* 输入：Server_Socket_GUI *this；
* 输出：无；
* 处理：刷新用户列表显示；
* 注释：刷新；
*/
void Server_Socket_GUI::Fresh() {
	list<User_Info>::iterator i;
	int j = 1;
	this->ui.tableWidget->setRowCount(UserList.size() + 1);
	for (i = UserList.begin(); i != UserList.end(); i++) {
		if ((*i).getType() == 0) {
			this->ui.tableWidget->setItem(j, 0, new QTableWidgetItem(QString((*i).getUsername()), Qt::AlignCenter));
			this->ui.tableWidget->setItem(j, 1, new QTableWidgetItem(QString::fromLocal8Bit("在线"), Qt::AlignCenter));
			this->ui.tableWidget->setItem(j, 2, new QTableWidgetItem(QString((*i).getIP()), Qt::AlignCenter));
			this->ui.tableWidget->setItem(j, 3, new QTableWidgetItem(QString::number((*i).getPort()), Qt::AlignCenter));
			j++;
		}
	}
	for (i = UserList.begin(); i != UserList.end(); i++) {
		if ((*i).getType() == 1) {
			this->ui.tableWidget->setItem(j, 0, new QTableWidgetItem(QString((*i).getUsername()), Qt::AlignCenter));
			this->ui.tableWidget->setItem(j, 1, new QTableWidgetItem(QString::fromLocal8Bit("离线"), Qt::AlignCenter));
			this->ui.tableWidget->setItem(j, 2, new QTableWidgetItem(QString("0.0.0.0"), Qt::AlignCenter));
			this->ui.tableWidget->setItem(j, 3, new QTableWidgetItem(QString::number(0), Qt::AlignCenter));
			j++;
		}
	}
}

/**
* 类：Server_Socket_GUI；
* 函数名：Case;
* 基类：Ui_Server_Socket_GUIClass（包含ui的初始化）， QWidget；
* 派生类:无；
* 输入：Server_Socket_GUI *this， char *username， 事件c， char *Target（用户操作对象）；
* 输出：无；
* 处理：输出相应的日志信息；
* 注释：系统日志；
*/
void Server_Socket_GUI::Case(char *username, int c, char *Target) {
	if (c == 01) {
		this->ui.textBrowser->append(QString(username) + QString::fromLocal8Bit(": 登录失败：该用户已经在别处登录！\n"));
	}
	else if (c == 02) {
		this->ui.textBrowser->append(QString(username) + QString::fromLocal8Bit(": 登录成功！\n"));
	}
	else if (c == 03) {
		this->ui.textBrowser->append(QString(username) + QString::fromLocal8Bit(": 登录失败：用户不存在或密码错误！\n"));
	}
	else if (c == 11) {
		this->ui.textBrowser->append(QString(username) + QString::fromLocal8Bit(": 注册失败：该用户已经存在！\n"));
	}
	else if (c == 12) {
		this->ui.textBrowser->append(QString(username) + QString::fromLocal8Bit(": 注册成功！\n"));
	}
	else if (c == 21) {
		this->ui.textBrowser->append(QString(username) + QString::fromLocal8Bit(": 找回密码成功！\n"));
	}
	else if (c == 22) {
		this->ui.textBrowser->append(QString(username) + QString::fromLocal8Bit(": 找回密码失败：该用户不存在！\n"));
	}
	else if (c == 31) {
		this->ui.textBrowser->append(QString::fromLocal8Bit("未知名用户") + QString::fromLocal8Bit("请求获取") + QString(Target) + QString::fromLocal8Bit(" 的IP和端口成功！\n"));
	}
	else if (c == 32) {
		this->ui.textBrowser->append(QString::fromLocal8Bit("未知名用户") + QString::fromLocal8Bit("请求获取") + QString(Target) + QString::fromLocal8Bit(" 的IP和端口失败：未在线！\n"));
	}
	else if (c == 33) {
		this->ui.textBrowser->append(QString::fromLocal8Bit("未知名用户") + QString::fromLocal8Bit("请求获取") + QString(Target) + QString::fromLocal8Bit(" 的IP和端口失败：查无此用户！\n"));
	}
	else if (c == 41) {
		this->ui.textBrowser->append(QString(username) + QString::fromLocal8Bit("向") + QString(Target) + QString::fromLocal8Bit("发送离线消息！\n"));
	}
	else if (c == 51) {
		this->ui.textBrowser->append(QString(username) + QString::fromLocal8Bit(": 下线！\n"));
	}
}

/**
* 类：Server_Socket_GUI；
* 函数名：closeEvent;
* 基类：Ui_Server_Socket_GUIClass（包含ui的初始化）， QWidget；
* 派生类:无；
* 输入：Server_Socket_GUI *this， QCloseEvent *event；
* 输出：无；
* 处理：询问是否退出， YES-终止监听线程并且关闭窗口， NO-忽略关闭信号；
* 注释：重载窗口关闭函数；
*/
void Server_Socket_GUI::closeEvent(QCloseEvent *event) {
	QMessageBox::StandardButton button;
	button = QMessageBox::question(this, QString::fromLocal8Bit("退出程序"), QString::fromLocal8Bit("确认退出程序"), QMessageBox::Yes | QMessageBox::No);
	if (button == QMessageBox::No)
	{
		event->ignore(); // 忽略退出信号，程序继续进行  
	}
	else if (button == QMessageBox::Yes)
	{
		server.stop();
		event->accept(); // 接受退出信号，程序退出  
	}
	return;
}

/*
* 类：Server；
* 函数名：Server;
* 基类：无；
* 派生类:无；
* 输入：server *this；
* 输出：无；
* 处理：构造Server，初始化一些参数，获得存储空间；
* 注释：构造函数；
*/
Server::Server() :State(false), server(nullptr) {};

/**
* 类：Server；
* 函数名：start;
* 基类：无；
* 派生类:无；
* 输入：Server *this；
* 输出：true-成功开启线程， false-开启线程失败（原本已经开启）；
* 处理：线程没有开启则开启监听线程，否则直接返回；
* 注释：线程启动函数；
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
* 类：Server；
* 函数名：state;
* 基类：无；
* 派生类:无；
* 输入：server *this；
* 输出：true-开启状态， false-关闭状态；
* 处理：获取监听线程状态；
* 注释：线程状态函数；
*/
bool Server::state() {
	return this->State;
}

/**
* 类：Server；
* 函数名：stop;
* 基类：无；
* 派生类:无；
* 输入：server *this；
* 输出：true-关闭线程成功， false-关闭线程失败；
* 处理：终止监听线程；
* 注释：线程终止函数；
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
* 类：Server；
* 函数名：~Server;
* 基类：无；
* 派生类:无；
* 输入：server *this；
* 输出：无；
* 处理：析构Server对象；
* 注释：析构函数；
*/
Server::~Server() {
	delete this->server;
}