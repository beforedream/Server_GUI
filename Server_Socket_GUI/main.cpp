/*ϵͳ���û�����ͻ��ˣ�
**���ߣ�U201514607_CS1505_���Σ�
**���ڣ�2017_11_19��
*/
#include "Server_Socket_GUI.h"
#include "User_Info.h"
#include <QtWidgets/QApplication>

//�����棻
Server_Socket_GUI *w;
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	w = new Server_Socket_GUI();
	w->show();
	a.exec();
	return 0;
}
