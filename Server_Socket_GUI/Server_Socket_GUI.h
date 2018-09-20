#pragma once
#include <qheaderview.h>
#include "Socket.h"
#include "ui_Server_Socket_GUI.h"
#include <thread>
#include <QtWidgets/QWidget>
#include <qmessagebox.h>
#include <qevent.h>

class Server_Socket_GUI : public QWidget
{
	Q_OBJECT

public:
	Server_Socket_GUI(QWidget *parent = Q_NULLPTR);

private:
	Ui::Server_Socket_GUIClass ui;
	private slots:
	void Start();
	void Stop();
	void Close();
	void Fresh();
	void Case(char *, int, char *Target = nullptr);
	void closeEvent(QCloseEvent *event);
signals:
	void signal_fresh();
	void signal_case(char *, int, char *Target = nullptr);
};

/*服务器监听端的线程封装；*/
class Server {
	bool State;
	::thread *server;
public:
	Server();
	bool start();
	bool stop();
	bool state();
	~Server();
};