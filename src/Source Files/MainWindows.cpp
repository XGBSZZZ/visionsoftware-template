#include "Useme.h"
#include "MainWindows.h"



MainWindows::MainWindows(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	this->setWindowTitle(tr("Theme of "));
	show_Text(tr("加载中......"));

	qloadingwidget->show();

	Init_MyWorkWindow();
	Init_conn();
	Init_TestTimer();
	Init_Ui();

	Init_communication();

	this->showMaximized();
	qloadingwidget->close();
	delete qloadingwidget;
	if (!quicklytest)
	{
		userwindow->show();
	}

	//快速测试通讯模块
	//on_actionCommunication_triggered(); 


	show_Text(tr("加载完毕!!!!!!"));
}

MainWindows::~MainWindows()
{
	testqtimer->stop();
	delete communicationadmini;
	userwindow->deleteLater();

	save_warning_message("");


}

void MainWindows::on_ReceivedData(const QString& name, const QString& data)
{

	if (data == "Demo")
	{
		ui.actionAllUseDemo->setChecked(true);
		on_actionAllUseDemo_triggered(true);  //当信号为Demo时 触发压力测试
	}

	QStringList datalist = data.split(";");
}

void MainWindows::set_saveworkdatapath(const std::string& path)
{
	for (auto& i : myworkwindow)
	{
		QByteArray cdata = i->WorkName.toLocal8Bit();
		std::string stdpath = std::string(cdata);

		i->qsaveworkdatapath = QString::fromStdString(path) + "/" + i->WorkName;
		i->productionwindows->savepath = i->qsaveworkdatapath;
		i->saveworkdatapath = path + "/" + stdpath;

		creat_workfile(path + "/" + stdpath);
	}
}

void MainWindows::set_saveworkdatapath(const QString& qpath)
{
	QByteArray cdata = qpath.toLocal8Bit();
	std::string path = std::string(cdata);
	set_saveworkdatapath(path);
}

void MainWindows::creat_workfile(const std::string& path)
{
	//创建文件夹
	FriendFuntion::isDirExist(path); QApplication::processEvents();
	FriendFuntion::isDirExist(path + "/OK"); QApplication::processEvents();
	FriendFuntion::isDirExist(path + "/NG"); QApplication::processEvents();
	FriendFuntion::isDirExist(path + "/ProductionStatistics"); QApplication::processEvents();
	FriendFuntion::isDirExist(path + "/ProductionData"); QApplication::processEvents();
	//friendfunciotn->isFileExist(path + "/calibdata.txt"); QApplication::processEvents();
	//friendfunciotn->isFileExist(path + "/Parametric.txt"); QApplication::processEvents();
}

void MainWindows::creat_workfile(const QString& qpath)
{
	QByteArray cdata = qpath.toLocal8Bit();
	std::string path = std::string(cdata);
	creat_workfile(path);
}

void MainWindows::on_Userok(const FriendFuntion::MyUser& zz, const bool& z)
{
	QString tempq;


	if (zz == FriendFuntion::MyUser::Engineer)
	{
		tempq = tr("工程师");
	}
	else if (zz == FriendFuntion::MyUser::Player)
	{
		tempq = tr("操作员");
	}
	if (z)
	{
		show_Text(tempq + tr("登录"));
	}
	else
	{
		if (ui.actionUseradmini->isEnabled())
		{
			show_Text(tempq + "退出");
		}
	}


	userwindow->ui.pwdLEd->setText("");  //输入密码初始化
	for (int i = 0; i < worknumb; i++)
	{
		myworkwindow[i]->butten_changge(zz, z);
	}
	if (zz == FriendFuntion::MyUser::Engineer)
	{

		ui.actionCommunication->setEnabled(z);
		ui.actionUseradmini->setEnabled(z);
		ui.actionSetting->setEnabled(z);
		ui.actionAllUseDemo->setEnabled(z);
		ui.actionSetting->setEnabled(z);
		ui.actionStart->setEnabled(z);
	}
	else if (zz == FriendFuntion::MyUser::Player)
	{
		ui.actionCommunication->setEnabled(z);
		ui.actionSetting->setEnabled(z);
		ui.actionStart->setEnabled(z);
	}



}

void MainWindows::on_PasssWordChange(const std::string& z, const std::string& zz)
{

	if (z == "Engineer")
	{
		userwindow->EngineerPassword = zz;
	}
	else if (z == "Player")
	{
		userwindow->PlayerPassword = zz;
	}

	show_Text(QString::fromStdString(z) + tr(" 密码更改"));
}

void MainWindows::Init_MyWorkWindow()
{
	worknumb = works.size();

	do
	{
		if (worknumb == 0)
		{
			throw tr("初始化失败,未输入工位数量");
		}
		if (works.size() < worknumb)
		{
			throw tr("初始化失败,输入的工位数量超出设定的范围");
		}

	} while (false);

	QApplication::processEvents();


	FriendFuntion::isDirExist("workdata");  QApplication::processEvents();
	FriendFuntion::isFileExist("workdata/MainParametric.txt"); QApplication::processEvents();


	QApplication::processEvents();

	int camerstate_x = 0;
	int camerstate_y = 0;

	QHBoxLayout* databtn_layout = new QHBoxLayout;
	QHBoxLayout* data_layout = new QHBoxLayout;
	data_layout->setSpacing(0);
	data_layout->setMargin(0);
	for (int i = 0; i < worknumb; i++)
	{
		QByteArray cdata = works[i].workname.toLocal8Bit();
		std::string zds = std::string(cdata);

		//创建文件夹
		creat_workfile("workdata/" + zds);
		FriendFuntion::isDirExist("workdata/" + zds); QApplication::processEvents();
		FriendFuntion::isDirExist("workdata/Daybook"); QApplication::processEvents();
		FriendFuntion::isDirExist("workdata/WarningMessage"); QApplication::processEvents();
		FriendFuntion::isDirExist("workdata/" + zds + "/Products"); QApplication::processEvents();
		FriendFuntion::isFileExist("workdata/" + zds + "/calibdata.txt"); QApplication::processEvents();
		FriendFuntion::isFileExist("workdata/" + zds + "/Parametric.txt"); QApplication::processEvents();

		MyWorkWindow* zzzzzz = new MyWorkWindow;

		zzzzzz->ui.picturedisplay_2->setText(tr("工位") + QString::number(i + 1) + ": " + works[i].workname);
		zzzzzz->qsaveworkdatapath = "workdata/" + works[i].workname;
		zzzzzz->saveworkdatapath = "workdata/" + zds;
		zzzzzz->WorkName = works[i].workname;
		zzzzzz->mycamer->camname = works[i].camername;
		zzzzzz->AlgorithmName = works[i].algname;
		zzzzzz->Workcalib = works[i].mycalib;
		zzzzzz->worklabel = i;
		//zzzzzz->savedatathread->workname = zds; 
		zzzzzz->stdworkname = zds;
		zzzzzz->alonumb = alonumb;
		for (int displaynumb = 0; displaynumb < displaybuffer; displaynumb++)
		{
			displayimageThread* displayimagethread = new displayimageThread;
			connect(displayimagethread, &displayimageThread::show_image_thread, zzzzzz, &MyWorkWindow::show_image_thread);
			zzzzzz->displayimagethreadpool.push_back(displayimagethread);
		}


		zzzzzz->workparametricwindow = new WorkParametricWindow(zzzzzz->qsaveworkdatapath);
		if (works[i].camername != FriendFuntion::MyCameranames::Line_Scan_Cameras)
		{

		}

		connect(zzzzzz, &MyWorkWindow::showtext, this, &MainWindows::show_Text); QApplication::processEvents();
		//connect(zzzzzz, &MyWorkWindow::savewarningmessage, this, &MainWindows::save_warning_message); QApplication::processEvents();
		connect(zzzzzz, SIGNAL(savewarningmessage(const QString&)), this, SLOT(save_warning_message(const QString&)));



		//WorkWindow生产统计uiの添加
		zzzzzz->productionwindows = new ProductionWindows(ui.groupBox_production);
		zzzzzz->productionwindows->setGeometry(0, 150 * i, 390, 150);

		//WorkWindow工位uiの添加
		ui.zzzlyout->addWidget(zzzzzz); QApplication::processEvents();

		//WorkWindow相机状态uiの添加
		zzzzzz->camerstate = new QLabel(ui.groupBox_Camer);
		zzzzzz->camerstate->setText(zzzzzz->WorkName);
		zzzzzz->camerstate->setStyleSheet(QString("border-image:url(myfile/images/gg.png);background-color: rgb(200, 230, 255);color: rgb(100, 170, 255);"));
		zzzzzz->camerstate->setFont((QFont("Timers", 9, QFont::Normal)));
		zzzzzz->camerstate->setAlignment(Qt::AlignCenter);
		zzzzzz->camerstate->setScaledContents(true);
		zzzzzz->camerstate->setWordWrap(true);
		int addbianjie = 15;
		zzzzzz->camerstate->setGeometry(camerstate_x * (120 + addbianjie), camerstate_y * (120 + addbianjie),
			100 + addbianjie, 100 + addbianjie);
		camerstate_y = camerstate_x == 2 ? camerstate_y + 1 : camerstate_y + 0;
		camerstate_x = camerstate_x == 2 ? camerstate_x = 0 : camerstate_x + 1;

		//WorkWindow算法数据显示uiの添加
		QPushButton* databtn = new QPushButton();
		databtn->setText("工位" + QString::number(i + 1));
		QSize databtn_size = QSize(115, 30);
		databtn->setMinimumSize(databtn_size);
		databtn->setMaximumSize(databtn_size);
		databtn->setFont((QFont("Timers", 13, QFont::Normal)));
		databtn->setIcon(QIcon(QPixmap("myfile/images/Mabe/box.jpg")));
		databtn->setIconSize(QSize(20, 20));
		databtn->setStyleSheet(QString("QPushButton{background-color: rgb(200, 230, 255);}\
			QPushButton:checked{background-color:rgb(85, 170, 255);}"));

		//allSelect->setStyleSheet("QPushButton{border-image: url(:/1.png) 0 0 0 0;border:none;color:rgb(255, 255, 255);}"
		//	"QPushButton:hover{background-color: rgb(20, 62, 134);border:none;color:rgb(255, 255, 255);}"
		//	"QPushButton:checked{background-color: rgb(20, 62, 134);border:none;color:rgb(255, 255, 255);}");

		databtn->setCheckable(true);
		databtn->setAutoExclusive(true);
		if (i == 0) databtn->setChecked(true);
		connect(databtn, &QPushButton::clicked, [=](bool) {
			for (auto& tf_work : myworkwindow)
			{
				tf_work->datadisplay->hide();
			}
			zzzzzz->datadisplay->show();
			});

		databtn_layout->addWidget(databtn);

		zzzzzz->datadisplay = new QTableWidget();
		data_layout->addWidget(zzzzzz->datadisplay);

		//相信我,你也可以变成光
		myworkwindow.push_back(zzzzzz); QApplication::processEvents();
	}

	ui.groupBox_databtn->setLayout(databtn_layout);
	ui.groupBox_datatable->setLayout(data_layout);

	//数据Ui初始化
	for (auto& tf_work : myworkwindow)
	{
		tf_work->datadisplay->hide();
	}
	myworkwindow[0]->datadisplay->show();

}

void MainWindows::Init_communication()
{
	connect(communicationadmini, &CommunicationAdmini::sig_Add_Change, [=](const QString& name, const QString& type) {
		bool ishide = ui.groupBox_Comunication->isHidden();
		if (!ishide)
		{
			ui.groupBox_Comunication->hide();
		}
		QLabel* communicationstate = new QLabel(ui.groupBox_Comunication);
		communicationstate->setText(type + "\r\n" + name);
		communicationstate->setFont((QFont("Timers", 9, QFont::Normal)));
		communicationstate->setWordWrap(true);
		communicationstate->setAlignment(Qt::AlignCenter);
		communicationstate->setStyleSheet(QString("border-image:url(myfile/images/gg.png); \
                                          background-color: rgb(200, 230, 255);color: rgb(100, 170, 255);"));
		allcommunicationstate.insert(name, communicationstate);
		reset_communicationstate();

		if (!ishide)
		{
			ui.groupBox_Comunication->show();
		}

		});

	connect(communicationadmini, &CommunicationAdmini::sig_Delete_Change, [=](const QString& name, const QString& type) {
		delete allcommunicationstate[name];
		allcommunicationstate.remove(name);
		reset_communicationstate();
		});

	for (auto& i : myworkwindow)
	{
		i->OtherInit(savebuffer); QApplication::processEvents();
		connect(communicationadmini, &CommunicationAdmini::sig_Add_Change, i->workparametricwindow, &WorkParametricWindow::communication_chage);

		connect(communicationadmini, &CommunicationAdmini::sig_Delete_Change, i->workparametricwindow, &WorkParametricWindow::communication_delete);

		connect(i, &MyWorkWindow::sig_SendData_TCP, communicationadmini, &CommunicationAdmini::Send_Data);
		connect(i, &MyWorkWindow::sig_SendData_UDP, communicationadmini, &CommunicationAdmini::Send_Data);
	}

	//初始化PB模型
	myworkwindow[0]->alopool[0]->PbInit();

	communicationadmini->loaddata();
	for (auto& i : myworkwindow)
	{
		i->workparametricwindow->otherinit();
		i->on_aloparametric_change();
		i->savealoerrcodetext->set_title(false, "");
	}
	for (auto i : communicationadmini->allcommunication)
	{
		connect(i, &CommunicationBtn::sig_ReceivedData, this, &MainWindows::on_ReceivedData);
		connect(i, &CommunicationBtn::sig_Communicationstate, [=](const QString& name, const bool& state) {
			if (state)
			{
				allcommunicationstate[name]->setStyleSheet(QString("border-image:url(myfile/images/bind.png);background-color: rgb(200, 230, 255);color: rgb(100, 170, 255);"));

			}
			else
			{
				allcommunicationstate[name]->setStyleSheet(QString("border-image:url(myfile/images/gg.png);background-color: rgb(200, 230, 255);color: rgb(100, 170, 255);"));
			}
			});

	}

	connect(communicationadmini, &CommunicationAdmini::sig_Show_Text, this, &MainWindows::show_Text);

	on_btnPruduction_clicked();
}

void MainWindows::Init_conn()
{
	connect(userwindow, &UserWindow::hahaha, this, &MainWindows::on_Userok); QApplication::processEvents();
	//connect(userwindow, &UserWindow::sig_Show_Text, this, &MainWindows::show_Text); QApplication::processEvents();
}

void MainWindows::Init_Ui()
{
	//QPushButton* q = new QPushButton("32",ui.groupBox_production);
	//q->setGeometry(10 , 50 , 110, 25);
	//QPushButton* q1 = new QPushButton("12",ui.groupBox_production);
	//ui.production->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	if (!quicklytest)
	{
		on_Userok(FriendFuntion::MyUser::Engineer, false);
	}

	this->centralWidget()->layout()->setMargin(0);
	this->statusBar()->setSizeGripEnabled(false);//隐藏状态栏上的拖动图标
	ui.groupBox_right->setStyleSheet("QGroupBox{border:none}");

	//主参数界面初始化
	mainparametricwindow = new MainParametricWindow(worknumb);
	connect(mainparametricwindow, SIGNAL(savapathchange(const QString&)), this, SLOT(set_saveworkdatapath(const QString&))); QApplication::processEvents();
	connect(mainparametricwindow, &MainParametricWindow::day_night_change, this, &MainWindows::on_mainParametric); QApplication::processEvents();
	connect(mainparametricwindow->ui.max_data, SIGNAL(valueChanged(int)), this, SLOT(reset_maxdatanumb(int)));
	reset_maxdatanumb(mainparametricwindow->ui.max_data->value());
	set_saveworkdatapath(mainparametricwindow->ui.workdata_savepath->text());
	on_mainParametric();
	on_btnPruduction_clicked();
	on_production_clicked();
	ui.btnPruduction->setChecked(true);
	ui.btnPruduction->setStyleSheet(QString("QPushButton{background-color: rgb(200, 230, 255);}\
			QPushButton:checked{background-color:rgb(85, 170, 255);}"));
	ui.btnCommunication->setStyleSheet(QString("QPushButton{background-color: rgb(200, 230, 255);}\
			QPushButton:checked{background-color:rgb(85, 170, 255);}"));
	ui.btnCammer->setStyleSheet(QString("QPushButton{background-color: rgb(200, 230, 255);}\
			QPushButton:checked{background-color:rgb(85, 170, 255);}"));

}

void MainWindows::Init_TestTimer()
{
	testqtimer = new QTimer;
	testqtimer->setTimerType(Qt::PreciseTimer);
	connect(testqtimer, SIGNAL(timeout()), this, SLOT(on_testtimeout()));
}

void MainWindows::show_Text(const QString& z)
{
	//QString current_date = QDateTime::currentDateTime().toString("hh:mm:ss");
	// std::string tempzz = z.toStdString();  
	// QString tempz = z.toUtf8();  
	QString current_date = QDateTime::currentDateTime().toString("yyyy-MM-dd"); //  yyyy表示年；MM表示月；dd表示日； hh表示小时；mm表示分；ss表示秒；zzz表示毫秒；ddd表示周几
	QString current_date2 = QDateTime::currentDateTime().toString("hh:mm:ss"); //  yyyy表示年；MM表示月；dd表示日； hh表示小时；mm表示分；ss表示秒；zzz表示毫秒；ddd表示周几
	QString savedata = current_date2 + ": " + (z);
	ui.textdisplay->appendPlainText(savedata);

	FriendFuntion::isFileExist("workdata/Daybook/" + current_date.toStdString() + ".txt");
	Daybookfile.setFileName("workdata/Daybook/" + current_date + ".txt");
	if (!Daybookfile.open(QIODevice::Append | QIODevice::Text)) {
		qDebug() << "文件打开失败";
	}
	QTextStream SaveDaybook(&Daybookfile);
	SaveDaybook << savedata + "\n";

	Daybookfile.close();
}

void MainWindows::show_Image(const int& workname, const cv::Mat& image)
{
	myworkwindow[workname]->show_image(image);
	return;
}

void MainWindows::save_warning_message()
{
	savewarningtext->savewhat = warningdatabuffer;
	savewarningtext->start(QThread::LowestPriority);
	warningdatabuffer.clear();
}

void MainWindows::save_warning_message(const QString& warningdata)
{
	QString current_datefile = QDateTime::currentDateTime().toString("hh:mm:ss:zzz");//  hh表示小时；mm表示分；ss表示秒；zzz表示毫秒;
	warningdatabuffer.push_back(current_datefile + ":   " + warningdata);

	if (!savewarningtext->isRunning() && warningdatabuffer.size() >= 1)
	{
		save_warning_message();
	}


}

void MainWindows::reset_communicationstate()
{
	int x = 0;
	int y = 0;


	QMap<QString, QLabel*>::iterator iter = allcommunicationstate.begin();
	while (iter != allcommunicationstate.end())
	{
		//qDebug() << iter.key();
		//qDebug() << iter.value()->text();
		int addbianjie = 15;
		iter.value()->setGeometry(x * (120 + addbianjie), y * (120 + addbianjie), 100 + addbianjie, 100 + addbianjie);
		y = x == 2 ? y + 1 : y + 0;
		x = x == 2 ? x = 0 : x + 1;
		iter++;
	}

}

void MainWindows::reset_maxdatanumb(int)
{
	for (auto& i : myworkwindow)
	{
		i->max_datarow = mainparametricwindow->ui.max_data->value();
		i->now_datarow = 0;
	}
}

void MainWindows::on_actionStart_triggered(bool z)
{
	int oknumb = 0;

	for (auto& i : myworkwindow)
	{
		oknumb = i->mycamer->setcamerrun(z) ? oknumb + 1 : oknumb + 0;
	}

	if (oknumb == myworkwindow.size())
	{
		ui.actionStart->setChecked(true);
	}
	else
	{
		ui.actionStart->setChecked(false);
	}
}

void MainWindows::on_actionCommunication_triggered()
{
	communicationadmini->hide();
	communicationadmini->show();
}

void MainWindows::on_actionUserSign_triggered()
{
	userwindow->hide();
	userwindow->show();
}

void MainWindows::on_actionExitSign_triggered()
{
	if (ui.actionUseradmini->isEnabled())
	{
		userwindow->ui.pwdLEd->setText("");
	}
	on_Userok(UserName, false);

}

void MainWindows::on_actionUseradmini_triggered() {
	useradmini = new UserAdmini;
	connect(useradmini, &UserAdmini::passswordchange, this, &MainWindows::on_PasssWordChange); QApplication::processEvents();
	useradmini->hide();
	useradmini->show();
}

void MainWindows::on_actionTips_triggered() {

}

void MainWindows::on_actionSetting_triggered() {
	mainparametricwindow->hide();
	mainparametricwindow->show();
}

void MainWindows::on_mainParametric()
{
	for (auto& i : myworkwindow)
	{
		//OK/NG的显示
		i->okprice = mainparametricwindow->ui.okprice->text().toInt();
		i->ngprice = mainparametricwindow->ui.ngprice->text().toInt();
		i->nowokprice = 0;
		i->nowngprice = 0;

		//白夜班
		i->m_isdaynight = mainparametricwindow->ui.isdaynight->isChecked();
		i->m_daytimehouse = mainparametricwindow->ui.day_House->value();
		i->m_daytimemin = mainparametricwindow->ui.day_min->value();
		i->m_nighttimehouse = mainparametricwindow->ui.night_House->value();
		i->m_nighttimemin = mainparametricwindow->ui.night_min->value();

		i->productionwindows->m_isdaynight = mainparametricwindow->ui.isdaynight->isChecked();
		i->productionwindows->m_daytimehouse = mainparametricwindow->ui.day_House->value();
		i->productionwindows->m_daytimemin = mainparametricwindow->ui.day_min->value();
		i->productionwindows->m_nighttimehouse = mainparametricwindow->ui.night_House->value();
		i->productionwindows->m_nighttimemin = mainparametricwindow->ui.night_min->value();

		i->productionwindows->saveproductionthread->nowshift = "unknow";
		i->productionwindows->saveproductionthread->m_isdaynight = mainparametricwindow->ui.isdaynight->isChecked();
		i->productionwindows->saveproductionthread->m_daytimehouse = mainparametricwindow->ui.day_House->value();
		i->productionwindows->saveproductionthread->m_daytimemin = mainparametricwindow->ui.day_min->value();
		i->productionwindows->saveproductionthread->m_nighttimehouse = mainparametricwindow->ui.night_House->value();
		i->productionwindows->saveproductionthread->m_nighttimemin = mainparametricwindow->ui.night_min->value();
		i->productionwindows->saveproductionthread->autoclear = mainparametricwindow->ui.autoclear_production->isCheckable();

		//删图
		i->m_needdelete = mainparametricwindow->ui.isclearpicture->isChecked();
		i->m_mustdelete = mainparametricwindow->ui.ismustclear->isChecked();
		i->m_maxday = mainparametricwindow->ui.picture_day->value();
		i->m_maxsize = mainparametricwindow->ui.mustsize->value();
	}
}

void MainWindows::on_actionAllUseDemo_triggered(bool z)
{

	if (z)
	{
		if (mainparametricwindow->allqcheckbox[0]->isChecked())
		{
			for (int i = 0; i < worknumb; i++)
			{
				myworkwindow[i]->mycamer->camname = FriendFuntion::MyCameranames::Null;
			}
		}
		testqtimer->start(mainparametricwindow->timeer->text().toInt());
	}
	else
	{
		for (int i = 0; i < worknumb; i++)
		{

			myworkwindow[i]->mycamer->camname = works[i].camername;

		}

		testqtimer->stop();
	}
}

void MainWindows::on_actionLanguage_triggered()
{

	cv::FileStorage UsersDataread("workdata/languageData.yml", cv::FileStorage::READ);

	std::string language;

	UsersDataread["language"] >> language;

	UsersDataread.release();

	cv::FileStorage UsersDataread2("workdata/languageData.yml", cv::FileStorage::WRITE);

	language = language == "English" ? "Chinese" : "English";

	show_Text(tr("已经切换语言:") + QString::fromStdString(language) + tr("请重启程序"));

	UsersDataread2 << "language" << language;

	UsersDataread2.release();
	//
	//
	//	UsersDataread["language"] >> "English";
	//	//UsersDataread["PlayerPassword"] >> PlayerPassword;
	//
	//	UsersDataread.release();
	//
	//
	//
	//	qApp->removeTranslator(translator);
	////	delete trans;
	//	//m_translator = new QTranslator;
	//	translator->load("x64/Debug/MyWorkWindow_en.qm");
	//	qApp->installTranslator(translator);
	//
	//	//ui.retranslateUi(this); //刷新界面字符串
	//
	//	showText(tr("重启界面生效"));

}

void MainWindows::on_production_clicked()
{
	ui.groupBox_state->show();
	ui.groupBox_textdisplay->hide();
	ui.groupBox_data->hide();
}

void MainWindows::on_dayNoteBook_clicked()
{
	ui.groupBox_state->hide();
	ui.groupBox_textdisplay->show();
	ui.groupBox_data->hide();
}

void MainWindows::on_data_clicked()
{
	ui.groupBox_state->hide();
	ui.groupBox_textdisplay->hide();
	ui.groupBox_data->show();
}

void MainWindows::on_btnPruduction_clicked()
{
	ui.groupBox_production->show();
	ui.groupBox_Comunication->hide();
	ui.groupBox_Camer->hide();
}

void MainWindows::on_btnCommunication_clicked()
{
	ui.groupBox_production->hide();
	ui.groupBox_Comunication->show();
	ui.groupBox_Camer->hide();

}

void MainWindows::on_btnCammer_clicked()
{
	ui.groupBox_production->hide();
	ui.groupBox_Comunication->hide();
	ui.groupBox_Camer->show();
}



void MainWindows::on_testtimeout()
{
	for (size_t i = 0; i < worknumb; i++)
	{
		if (mainparametricwindow->allqcheckbox[i + 1]->isChecked())
		{
			myworkwindow[i]->on_actionSingle_triggered();
		}
	}
}
