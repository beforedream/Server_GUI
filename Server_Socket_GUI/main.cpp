/*系统：用户聊天客户端；
**作者：U201514607_CS1505_李涛；
**日期：2017_11_19；
*/
#include "Server_Socket_GUI.h"
#include "User_Info.h"
#include <QtWidgets/QApplication>

//主界面；
Server_Socket_GUI *w;
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	w = new Server_Socket_GUI();
	w->show();
	a.exec();
	return 0;
}
