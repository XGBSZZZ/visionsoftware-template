#include "TCPclient.h"

TCPclient::TCPclient(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//�����ڹرվ�����
	setAttribute(Qt::WA_QuitOnClose, false);
}

TCPclient::~TCPclient()
{
}
