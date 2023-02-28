#include "UDPserver.h"

UDPserver::UDPserver(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_QuitOnClose, false);
	
	ui.use_send_message->setEnabled(false);

	//加载保存文件

    //关联读数据信号readyread
    connect(UdpServer, SIGNAL(readyRead()), this, SLOT(receive_data()));

	//天使の心跳
	m_heartbeat->setInterval(ui.angel_beats_timer->value() * 1000);
	connect(ui.angel_beats_timer, SIGNAL(valueChanged(int)), this, SLOT(on_angel_beats_timer_change(int)));
	connect(m_heartbeat, SIGNAL(timeout()), this, SLOT(on_angelbeats()));
}



UDPserver::~UDPserver()
{
	m_heartbeat->stop();
	UdpServer->close();
	UdpServer->deleteLater();
}


void UDPserver::receive_data()
{
	 QByteArray array;
     array.resize(UdpServer->bytesAvailable()); //将接收数据的array设置成为要接收数据的大小
	 QHostAddress recvaddress;
	 quint16 port;
     UdpServer->readDatagram(array.data(), array.size(), &recvaddress, &port); //读取数据
	 QString data = QString::fromLocal8Bit(array);
	 show_localtext(tr("[接收]: ") + data);

	 emit sig_ReceivedData(data);

	 //如果数据等于 绑定接收信号且未绑定 那么就表示已经绑定且开启定时器
	 if (data == ui.recive_bind->text() && !connectionisok && ui.recive_bind->text() != "")
	 {
		 connectionisok = true;
		 if (ui.use_angel_beats->isChecked())
		 {
			 m_heartbeat->start();
			 show_localtext(tr("心跳已启动"));
		 }
		 show_localtext(tr("绑定成功"));
		 sig_Communicationstate(true);
		 ui.use_send_message->setEnabled(true);
	 }

	 //刷新定时器
	 if (connectionisok && ui.use_angel_beats->isChecked() && data == ui.recive_angel_beats->text())
	 {
		 m_heartbeat->stop();
		 m_heartbeat->start();
		 send_data(ui.send_angel_beats->text());
	 }
	 
}

void UDPserver::on_clear_text_clicked()
{
	ui.plainTextEdit->clear();
}

void UDPserver::on_use_bind_clicked()
{
	UdpServer->close();
	send_data(ui.send_bind->text());
	qDebug() << ui.send_bind->text();
}

void UDPserver::on_use_send_message_clicked()
{
	send_data(ui.send_message->text());
}

void UDPserver::on_join_receive_team_clicked()
{
  	if (UdpServer->joinMulticastGroup(QHostAddress(ui.recive_team_join_adress->text()))) //加入组播
       {
		    ui.recive_team_exit_adress->addItem(ui.recive_team_join_adress->text());		    
       }
    else
	   {
		    show_localtext(tr("加入组播失败,请修改ip后继续加入"));	         
	   }
}

void UDPserver::on_exit_receive_team_clicked()
{
	UdpServer->leaveMulticastGroup(QHostAddress(ui.recive_team_exit_adress->currentIndex()));//退出组播地址列表当前的组播
	ui.recive_team_exit_adress->removeItem(ui.recive_team_exit_adress->currentIndex()); //删除组播地址列表中当前的组播地址
}

void UDPserver::on_use_angel_beats_clicked(bool angel_beats)
{
	if (angel_beats && connectionisok) { m_heartbeat->start();  show_localtext(tr("心跳已启动")); }
	else   m_heartbeat->stop();
}

void UDPserver::on_angel_beats_timer_change(int)
{
    m_heartbeat->setInterval(ui.angel_beats_timer->value() * 1000);
}

void UDPserver::on_angelbeats()
{
	show_localtext(tr("心跳停止信号触发"));
	m_heartbeat->stop();
	UdpServer->close();
	connectionisok = false;
	ui.use_send_message->setEnabled(false);
	sig_Communicationstate(false);
}

void UDPserver::send_data(const QString& data)
{
	int port = ui.spinBox->value();

	switch (ui.use_whatplay->currentIndex())
	{
	case 0: {UdpServer->writeDatagram(data.toLocal8Bit(), QHostAddress(ui.send_onece_adress->text()), port);
		break; }

	case 1: {UdpServer->writeDatagram(data.toLocal8Bit(), QHostAddress(ui.send_onece_adress->text()), port);
		break; }

	case 2: {UdpServer->writeDatagram(data.toLocal8Bit(), QHostAddress::Broadcast, port);
		break; }

	default: break;
	}

	show_localtext(tr("[发送]: ") + data);

}

void UDPserver::show_localtext(const QString& text)
{
	ui.plainTextEdit->appendPlainText(text);
}
