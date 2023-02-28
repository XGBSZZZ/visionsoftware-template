#include "CommunicationAdmini.h"


CommunicationAdmini::CommunicationAdmini(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	FriendFuntion::isDirExist("workdata/communicationdata");


	//主窗口关闭就无了
	setAttribute(Qt::WA_QuitOnClose, false);

	for (size_t i = 0; i < 6; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			btn = new CommunicationBtn(this);
			btn->setGeometry(i * 150, j * 150, 150, 150);
			connect(btn, &CommunicationBtn::sig_Add_Change,[=](const QString& name, const QString& type){
				emit sig_Add_Change(name,type);
				emit sig_Show_Text(tr("新增通讯端口: ") + type + "   " + name);
				});
			connect(btn, &CommunicationBtn::sig_Delete_Change, [=](const QString& name, const QString& type) {
				emit sig_Delete_Change(name, type);
				emit sig_Show_Text(tr("删除通讯端口: ") + type + "   " + name);
				});

			allcommunication.push_back(btn);
		}		
	}

	
}

CommunicationAdmini::~CommunicationAdmini()
{
	savedata();
	for (auto &i : allcommunication)
	{
		delete i;
	}
}

void CommunicationAdmini::loaddata()
{
	int tf_what_index = 0;
	int tf_communication_index = 0;

	QFile CommunicationAdminifile;
	CommunicationAdminifile.setFileName("workdata/communicationdata/CommunicationAdmini.txt");
	if (!CommunicationAdminifile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "通讯文件加载失败";
		emit sig_Show_Text(tr("通讯文件加载失败"));
	}
	QTextStream SaveCommunicationAdmini(&CommunicationAdminifile);

	QString name = "unknow";
	//一直读，直至读取失败
	while (!SaveCommunicationAdmini.atEnd()) {

		int tf_type = -999;

		QString str;
		//从文件中读取一个字符串
		SaveCommunicationAdmini >> str;

		if ((tf_what_index % 2) == 0)
		{
			allcommunication[tf_communication_index]->communicationname = str;
			name = str;
		}

		else if ((tf_what_index % 2) == 1)
		{
			if (str == "TCPserver")
			{
				tf_type = 0;
			}
			else if (str == "TCPclient")
			{
				tf_type = 1;
			}
			else if (str == "UDP")
			{
				tf_type = 2;
			}

			allcommunication[tf_communication_index]->communicationtype = str;

			if (allcommunication[tf_communication_index]->communicationname != "unknow" && str != "unknow")
			{
				allcommunication[tf_communication_index]->Add_Change(name, tf_type);

				saveloadwidget = new QtWidgetsSaveLoad("workdata/communicationdata/" + name + ".txt");

				if (str == "TCPserver")
				{
					saveloadwidget->loadFromFile(allcommunication[tf_communication_index]->tcpserver);
				}
				else if (str == "TCPclient")
				{
					saveloadwidget->loadFromFile(allcommunication[tf_communication_index]->tcpclient);
				}
				else if (str == "UDP")
				{
					saveloadwidget->loadFromFile(allcommunication[tf_communication_index]->udp);
				}

				allcommunication[tf_communication_index]->Start();

				delete saveloadwidget;

			}

			
			tf_communication_index++;
			name = "unknow";

			if (tf_communication_index >= allcommunication.size())
			{
				break;
			}
		}


		tf_what_index++;

	}

	CommunicationAdminifile.close();
}

void CommunicationAdmini::Send_Data(const QString& name, const QString& data)
{
	for (auto &i : allcommunication)
	{
		if (i->communicationname == name)
		{
			if (i->communicationtype == "TCPserver")
			{
				i->tcpserver->SendData(data);
			}
			else if (i->communicationtype == "TCPclient")
			{
				i->tcpclient->Send_Data(data);
			}
			else if (i->communicationtype == "UDP")
			{
				i->udp->send_data(data);
			}
			return;
		}
	}
}


void CommunicationAdmini::savedata()
{
	QFile CommunicationAdminifile;
	CommunicationAdminifile.setFileName("workdata/communicationdata/CommunicationAdmini.txt");
	if (!CommunicationAdminifile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
		qDebug() << "通讯文件加载失败";
	}
	QTextStream SaveCommunicationAdmini(&CommunicationAdminifile);
	for (auto& i : allcommunication) {
		SaveCommunicationAdmini << i->communicationname + "\n";
		SaveCommunicationAdmini << i->communicationtype + "\n";

		saveloadwidget = new QtWidgetsSaveLoad("workdata/communicationdata/" + i->communicationname + ".txt");
		if (i->communicationtype == "TCPserver")
		{
			saveloadwidget->saveToFile(i->tcpserver);
		}
		else if (i->communicationtype == "TCPclient")
		{
			saveloadwidget->saveToFile(i->tcpclient);
		}
		else if (i->communicationtype == "UDP")
		{
			saveloadwidget->saveToFile(i->udp);
		}

		delete saveloadwidget;
	}
	CommunicationAdminifile.close();
}



void CommunicationAdmini::closeEvent(QCloseEvent* e)
{
	savedata();
}
