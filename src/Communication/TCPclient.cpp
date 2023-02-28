#include "TCPclient.h"

TCPclient::TCPclient(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//主窗口关闭就无了
	setAttribute(Qt::WA_QuitOnClose, false);
}

TCPclient::~TCPclient()
{
}
