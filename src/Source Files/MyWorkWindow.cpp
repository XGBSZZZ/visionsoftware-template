#include "Useme.h"
#include "MyWorkWindow.h"
#include <QMenu>
#include <QContextMenuEvent>
#include <QDateTime>


using namespace cv;
using namespace std;

MyWorkWindow::MyWorkWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint);


	setAttribute(Qt::WA_QuitOnClose, false);
	Init();
	this->statusBar()->setSizeGripEnabled(false);//隐藏状态栏上的拖动图标
	//this->centralWidget()->layout()->setMargin(0);
	//Init_ui();
}

MyWorkWindow::~MyWorkWindow()
{
	qDebug() << "workgg";
	save_production_data();

	delete mycamer;
	for (auto &i : alopool)
	{
		delete i;
	}
	delete workparametricwindow;
}



void MyWorkWindow::on_actionOpencamera_triggered()
{
	if (mycamer->isopen())
	{
		show_text(tr("相机已经打开"));
		camerstate->setStyleSheet(QString("border-image:url(myfile/images/bind.png);background-color: rgb(200, 230, 255);color: rgb(100, 170, 255);"));
		return;
	}
	else
	{
		QString errcode = "OK";
		try
		{
			errcode = mycamer->open_camer();
			if (errcode != "OK")
			{
				show_text(tr("相机打开失败:   ") + errcode);
				qDebug()<< "err of Cameropen: " + errcode;
				camerstate->setStyleSheet(QString("border-image:url(myfile/images/gg.png);background-color: rgb(200, 230, 255);color: rgb(100, 170, 255);"));
				ui.actionOpencamera->setChecked(false);
			}

		}
		catch (const std::exception& err)
		{
		        qDebug()<<err.what();
		}
	}

	if (mycamer->isopen())
	{
		show_text(tr("相机打开成功"));
		camerstate->setStyleSheet(QString("border-image:url(myfile/images/bind.png);background-color: rgb(200, 230, 255);color: rgb(100, 170, 255);"));
		//camerstate
	}


}

void MyWorkWindow::show_data(const QString& data)
{ 
	QStringList datalish = data.split(",");
	datadisplay->insertRow(now_datarow);

	if (now_datarow >= max_datarow)
	{
		int deletenumb = datadisplay->rowCount() - max_datarow ;
		for (int tf = 0; tf < deletenumb; tf++)
		{
			datadisplay->removeRow(0);
		}
		now_datarow = max_datarow - 1;
	}


	for (int i = 0; i < datalish.size() - 1; i++)
	{
		datadisplay->setItem(now_datarow, i, new QTableWidgetItem(datalish[i]));
	}
//
//private:
//	int now_datarow = 0;
//	int max_datarow = 0;
	now_datarow = now_datarow >= max_datarow ? max_datarow : now_datarow + 1;


}

void MyWorkWindow::show_image(const cv::Mat& threadimage)
{
	if ((nowngprice == (ngprice - 1) && imageisok == "NG") ||  (nowokprice == (okprice - 1) && imageisok == "OK") || (imageisok == "UNKNOW"))
	{
		image_view = threadimage;
		//显示图片
		displayimagethreadpool[nowdisplaylabel]->displayimages = threadimage;
		displayimagethreadpool[nowdisplaylabel]->uisize = ui.picturedisplay->size();
		if (displayimagethreadpool[nowdisplaylabel]->isRunning())
		{
			save_warning_message(tr("显示线程暂时堵塞"));
			qDebug() << "errofdisplayimage";
			displayimagethreadpool[nowdisplaylabel]->wait();
		}
		displayimagethreadpool[nowdisplaylabel]->start(QThread::NormalPriority);

		nowngprice = imageisok == "NG" ?  0 : nowngprice;
		nowokprice = imageisok == "OK" ?  0 : nowokprice;
	}
	else
	{
		nowngprice = ( imageisok == "NG" && nowngprice < (ngprice - 1)) ? nowngprice + 1 : 0;
		nowokprice = ( imageisok == "OK" && nowokprice < (okprice - 1))  ? nowokprice + 1 : 0;
	}


	//显示OK/NG
	if (imageisok == "OK")   productionwindows->is_ok();
	else if (imageisok == "NG")   productionwindows->is_ng();

	imageisok = "UNKNOW";
	nowdisplaylabel = (nowdisplaylabel == (displayimagethreadpool.size() - 1)) ? 0 : nowdisplaylabel + 1;
}

void MyWorkWindow::show_image_thread(const QImage& img)
{	
	ui.picturedisplay->setPixmap(QPixmap::fromImage(img));
}

void MyWorkWindow::on_ImageView(const bool&)
{
	ImageView* imageview;
	if (!image_view.empty())
	{

		cv::Mat displaytemp = image_view.clone();
		if (displaytemp.channels() == 3)
		{
			cv::cvtColor(displaytemp, displaytemp, CV_BGR2RGB);
		}
		else if (displaytemp.channels() == 1)
		{
			cv::cvtColor(displaytemp, displaytemp, CV_GRAY2RGB);
		}
		QImage img = QImage((const unsigned char*)(displaytemp.data), displaytemp.cols, displaytemp.rows, displaytemp.step, QImage::Format_RGB888);
		imageview = new ImageView(img, 0);
	}
	else
	{
		QPixmap pix;
		pix = ui.picturedisplay->grab(ui.picturedisplay->rect());
		imageview = new ImageView(pix.toImage(), 0);
	}

		//connect(imageview, SIGNAL(finished()), imageview, SLOT(deleteLater()));
		imageview->show();
}

void MyWorkWindow::on_SaveImage(const bool&) {


	QPixmap pix;
	pix = ui.picturedisplay->grab(ui.picturedisplay->rect());
	QImage img;
	img = pix.toImage();
    QString filename;
		try
	    {
		filename = QFileDialog::getExistingDirectory(this, tr("请选择图片保存路径..."), "./");
		if (filename.isEmpty())
		{
			return;
		}
		QString current_datefile = QDateTime::currentDateTime().toString("hh-mm-ss");
		img.save(filename +"/"+ current_datefile +".jpg");
	}
	catch (const std::exception& err)
	{
		qDebug() << "errofsaveimage";
	}
}

void MyWorkWindow::on_savaimage(const cv::Mat& saveimage, const std::string& imageisoks, const bool& issouces)
{
	// newSaveDataThread(cv::Mat saveimage, std::string imageisoks,
	//	std::string saveimagepaths, bool issouces, std::string savekid,
	//	int savegoodnumbs, QObject * parent = 0);

	std::string whatlast;
	int goodnumb = 95;

	if (imageisoks == "OK" && issouces)
	{
		whatlast = workparametricwindow->ui.ck_oksouce_last->currentText().toStdString();
		goodnumb =  (workparametricwindow->ui.li_oksouce_good->text()).toInt();
	}
	else if (imageisoks == "OK" && !issouces)
	{
		whatlast = workparametricwindow->ui.ck_okresult_last->currentText().toStdString();
		goodnumb = (workparametricwindow->ui.li_okresult_good->text()).toInt();
	}
	else if (imageisoks == "NG" && issouces)
	{
		whatlast = workparametricwindow->ui.ck_oksouce_last_2->currentText().toStdString();
		goodnumb = (workparametricwindow->ui.li_ngsouce_good->text()).toInt();
	}
	else if (imageisoks == "NG" && !issouces)
	{
		whatlast = workparametricwindow->ui.ck_ngresult_last->currentText().toStdString();
		goodnumb = (workparametricwindow->ui.li_ngresult_good->text()).toInt();
	}

	newSaveDataThread* newsavedatathread = new newSaveDataThread(saveimage, imageisoks, saveworkdatapath, issouces, whatlast,goodnumb);
	newsavedatathread->set_dayandnight(m_isdaynight, m_daytimehouse, m_daytimemin, m_nighttimehouse, m_nighttimemin);
	newsavedatathread->set_deleteday(m_needdelete, m_mustdelete, m_maxday, m_maxsize);
	connect(newsavedatathread, SIGNAL(finished()), newsavedatathread, SLOT(deleteLater()));
	newsavedatathread->start(QThread::LowestPriority);

}

void MyWorkWindow::on_workparametric_state(const bool& z, const bool& zz,const bool& zzz, const bool& zzzz)
{
	for (auto &i: alopool)
	{
		i->saveoksouce = z;
		i->saveokresult = zz;
		i->savengsouce = zzz;
		i->savengresult = zzzz;
	}
	
}

void MyWorkWindow::on_aloparametric_change()
{
	for (auto& j : alopool)
	{
		j->aloinputdata.aloparmetric.clear();
		for (int i = 0; i < workparametricwindow->all_parametric.size(); i++)
		{
			if (workparametricwindow->all_parametric[i]->text().size() > 0)
			{
				j->aloinputdata.aloparmetric.push_back(workparametricwindow->all_parametric[i]->text().toDouble());
			}
			else
			{
				j->aloinputdata.aloparmetric.push_back(0.0);
			}
		}

	}
}

void MyWorkWindow::show_text(const QString& text) 
{
	 emit showtext(WorkName+": "+ text);
}

void MyWorkWindow::get_image(const int& label, const cv::Mat& z)
{

	if (usecontinulabel || !usealolabel)
	{
		show_image(z);
	}
	else
	{
			if (alopool[nowalonumb]->isRunning())
			{
				qDebug() << "********************err of Alothread low numb";
				save_warning_message(tr("错误:算法线程数溢出,请增加算法线程的再重试"));
				show_text(tr("错误:算法线程数溢出,请增加算法线程的再重试"));
				alopool[nowalonumb]->wait();
			}
			alopool[nowalonumb]->alooutputdata.label = label;
	    	alopool[nowalonumb]->aloinputdata.inputimage = z;
	    	alopool[nowalonumb]->start(QThread::TimeCriticalPriority);
	    	nowalonumb = (nowalonumb + 1) == alopool.size() ? 0: nowalonumb + 1;
			//nowpushdata = nowpushdata == (databuffernumb - 1) ? 0 : ++nowpushdata;
	}
}


void MyWorkWindow::get_data(const Alooutputdata& z)
{
	//要是你给UNKNOW就干死你
	if (imageisok != "UNKNOW")
	{
		qDebug() << "err of result";
		save_warning_message(tr("结果异常"));
	}

	//当前的时间  时:分:秒:毫秒
	QString current_date = QDateTime::currentDateTime().toString("hh:mm:ss:zzz");

	//产品码
	QString SN_numb = workparametricwindow->ui.comboBox_products->currentText();
	//如果错误代码不为 OK 则 imageok 为NG
	imageisok = z.errcode == "OK" ? "OK" : "NG";

	//判断缓存中的当前索引是否发送出去   如果发送出去就将数据加入缓存 否则 嘿嘿嘿
	if (databuffer[z.label].isnull)
	{

		databuffer[z.label].isnull = false;

		databuffer[z.label].savebuffer.clear();
		databuffer[z.label].tcpbuffer.clear();
		databuffer[z.label].udpbuffer.clear();

		databuffer[z.label].savebuffer = outputsave[0] ? databuffer[z.label].savebuffer + (QString::number(z.label) + ",") : databuffer[z.label].savebuffer;
		databuffer[z.label].savebuffer = outputsave[1] ? databuffer[z.label].savebuffer + (current_date + ",") : databuffer[z.label].savebuffer;
		databuffer[z.label].savebuffer = outputsave[2] ? databuffer[z.label].savebuffer + (SN_numb + ",") : databuffer[z.label].savebuffer;
		databuffer[z.label].savebuffer = outputsave[3] ? databuffer[z.label].savebuffer + (imageisok + ",") : databuffer[z.label].savebuffer;

		databuffer[z.label].tcpbuffer = outputtcp[0] ? databuffer[z.label].tcpbuffer + (QString::number(z.label) + ";") : databuffer[z.label].tcpbuffer;
		databuffer[z.label].tcpbuffer = outputtcp[1] ? databuffer[z.label].tcpbuffer + (current_date + ";") : databuffer[z.label].tcpbuffer;
		databuffer[z.label].tcpbuffer = outputtcp[2] ? databuffer[z.label].tcpbuffer + (SN_numb + ";") : databuffer[z.label].tcpbuffer;
		databuffer[z.label].tcpbuffer = outputtcp[3] ? databuffer[z.label].tcpbuffer + (imageisok + ";") : databuffer[z.label].tcpbuffer;

		databuffer[z.label].udpbuffer = outputudp[0] ? databuffer[z.label].udpbuffer + (QString::number(z.label) + ";") : databuffer[z.label].udpbuffer;
		databuffer[z.label].udpbuffer = outputudp[1] ? databuffer[z.label].udpbuffer + (current_date + ";") : databuffer[z.label].udpbuffer;
		databuffer[z.label].udpbuffer = outputudp[2] ? databuffer[z.label].udpbuffer + (SN_numb + ";") : databuffer[z.label].udpbuffer;
		databuffer[z.label].udpbuffer = outputudp[3] ? databuffer[z.label].udpbuffer + (imageisok + ";") : databuffer[z.label].udpbuffer;


		for (int i = 4; i < outputtext.size(); i++)
		{
			if (outputsave[i])
			{
				databuffer[z.label].savebuffer += (QString::number(z.aloresult[i - 4]) + ",");
			}

			if (outputtcp[i])
			{
				databuffer[z.label].tcpbuffer += (QString::number(z.aloresult[i - 4]) + ";");
			}

			if (outputudp[i])
			{
				databuffer[z.label].udpbuffer += (QString::number(z.aloresult[i - 4]) + ";");
			}
		}


	}
	else
	{
		//save_warning_message(tr("错误!数据缓存区溢出,请加大缓存区"));
	}

	//发送数据  将缓存的数据作判断发送  这里写这么多是为了按顺序发
	int oknumb = 0;
	int tempnow = nowsendresult;
	while (!databuffer[tempnow].isnull)
	{
		tempnow = (tempnow == (databuffernumb - 1)) ? 0 : tempnow + 1;
		oknumb++;
	}

	if (!databuffer[nowsendresult].isnull)
	{
		for (int i = 0; i < oknumb; i++)
		{
			databuffer[nowsendresult].isnull = true;

			//显示数据
			show_data(databuffer[z.label].savebuffer);

			//保存数据
			save_production_data(databuffer[nowsendresult].savebuffer);

			//TCP发送
			if (workparametricwindow->ui.Communication_TCP->currentIndex() != 0)
			{
				Send_Data_TCP(databuffer[nowsendresult].tcpbuffer);
			}

			//UDP发送
			if (workparametricwindow->ui.Communication_UDP->currentIndex() != 0)
			{
				Send_Data_UDP(databuffer[nowsendresult].udpbuffer);
			}

			nowsendresult = (nowsendresult == (databuffernumb - 1)) ? 0 : nowsendresult + 1;
		}
	}

	//显示结果图
	show_image(z.outputimage); 

}


void MyWorkWindow::Init()
{
	//标定初始化
	switch (Workcalib)
	{
	case FriendFuntion::Area_Scan_Calib: {

		break;
	}								
	case FriendFuntion::Line_Scan_Calib:
		break;
	case FriendFuntion::Three_Dimension_Calib:
		break;
	default:
		break;
	}

}

void MyWorkWindow::OtherInit(const int& buffernumb)
{
	    Qt::ConnectionType conn = Qt::ConnectionType::QueuedConnection;
     
	    //相机线程初始化
	    mycamer->Otherinit();
		connect(mycamer, &MyCamer::give_image, this, &MyWorkWindow::get_image, conn);
		connect(mycamer, &MyCamer::show_text, this, &MyWorkWindow::show_text, conn);
		connect(mycamer, &MyCamer::save_warning_message, this, &MyWorkWindow::save_warning_message, conn);

		//算法线程池初始化
		for (int i = 0; i < alonumb; i++)
		{
		UseAlgorithm* usealgorithm = new UseAlgorithm;  //算法线程
	    usealgorithm->myalgrithms = AlgorithmName;
		usealgorithm->otherInit(buffernumb);
		connect(usealgorithm, &UseAlgorithm::show_text, this, &MyWorkWindow::show_text, conn);
		connect(usealgorithm, &UseAlgorithm::give_data, this, &MyWorkWindow::get_data, conn);
		connect(usealgorithm, &UseAlgorithm::sava_image, this, &MyWorkWindow::on_savaimage, conn);
		connect(usealgorithm, &UseAlgorithm::save_warning_message, this, &MyWorkWindow::save_warning_message, conn);
		alopool.push_back(usealgorithm);
		}
		qDebug() << "Openalo" << alonumb;
		//显示线程初始化
		//connect(displayimagethread, &displayimageThread::show_image_thread, this, &MyWorkWindow::show_image_thread, conn);

		//存储线程初始化
		//connect(savedatathread, &saveDataThread::show_text, this, &MyWorkWindow::show_text, conn);
		/*QByteArray cdata = WorkName.toLocal8Bit();
		std::string zds = std::string(cdata);
		savedatathread->workname = zds;*/
	

		//画面初始化
		ui.picturedisplay->setScaledContents(true);
		Mat displayimage = cv::imread("myfile/bg.jpg");
		if (displayimage.channels() == 3)
		{
			cv::cvtColor(displayimage, displayimage, CV_BGR2RGB);
		}
		else if (displayimage.channels() == 1)
		{
			cv::cvtColor(displayimage, displayimage, CV_GRAY2RGB);
		}
		cv::resize(displayimage, displayimage, cv::Size(ui.picturedisplay->width() - 1, ui.picturedisplay->height() - 1));
		QImage img = QImage((const unsigned char*)(displayimage.data), displayimage.cols, displayimage.rows, displayimage.step, QImage::Format_RGB888);
		show_image_thread(img);
		//ui.picturedisplay->setScaledContents(false);

		//算法结果存储初始化
		savealoerrcodetext = new saveTextThread("", true);

		//参数界面初始化
		connect(workparametricwindow, &WorkParametricWindow::workparametric_state, this, &MyWorkWindow::on_workparametric_state);
		connect(workparametricwindow, &WorkParametricWindow::aloparametric_change, this, &MyWorkWindow::on_aloparametric_change);
		connect(workparametricwindow, &WorkParametricWindow::show_text, this, &MyWorkWindow::show_text);
		//QVector<QString> outputtext;
		//QVector<bool> outputsave;
		//QVector<bool> outputtcp;
		//QVector<bool> outputudp;
		connect(workparametricwindow, &WorkParametricWindow::sig_output, [=](){
			outputtext.clear(); outputsave.clear(); outputtcp.clear(); outputudp.clear();
		
			if (workparametricwindow->all_outputname.size() != workparametricwindow->all_save.size()||
				workparametricwindow->all_save.size() != workparametricwindow->all_tcp.size()||
				workparametricwindow->all_tcp.size() != workparametricwindow->all_udp.size())
			{
				save_warning_message(tr("算法输出参数大小不一致"));
			}

			for (auto &i : workparametricwindow->all_outputname)
			{
				outputtext.push_back(i->text());
			}

			for (auto& i : workparametricwindow->all_save)
			{
				outputsave.push_back(i->isChecked());
			}

			for (auto& i : workparametricwindow->all_tcp)
			{
				outputtcp.push_back(i->isChecked());
			}

			for (auto& i : workparametricwindow->all_udp)
			{
				outputudp.push_back(i->isChecked());
			}

			});

		connect(workparametricwindow, &WorkParametricWindow::sig_title, [=]() {
			QString title;
			QStringList datatitle;
			int column = 0;

			for (int i = 0; i < workparametricwindow->all_outputname.size(); i++)
			{
				if (workparametricwindow->all_save[i]->isChecked())
				{
					title += (workparametricwindow->all_outputname[i]->text() + ",");
					column++;
				}
			}
			savealoerrcodetext->set_title(true, title);
			datadisplay->setColumnCount(column);
			datadisplay->setHorizontalHeaderLabels(title.split(","));
			datadisplay->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
			datadisplay->clearContents();
			});

		//结果数据缓存初始化
		databuffer.resize(databuffernumb);

		QString UNKNOW("UNKNOW");
		for (auto& i : databuffer) {
			i.isnull = true;
			i.savebuffer = UNKNOW;
			i.tcpbuffer = UNKNOW;
			i.udpbuffer = UNKNOW;
		}

		//产品统计初始化
		productionwindows->ui.worknamelabel->setText(WorkName);
		productionwindows->savepath = qsaveworkdatapath;
		productionwindows->m_isdaynight = m_isdaynight;
		productionwindows->m_daytimehouse = m_daytimehouse;
		productionwindows->m_daytimemin = m_daytimemin;
		productionwindows->m_nighttimehouse = m_nighttimehouse;
		productionwindows->m_nighttimemin = m_nighttimemin;
		productionwindows->load_data();

        //数据显示初始化
		datadisplay->setEditTriggers(QAbstractItemView::NoEditTriggers);
		datadisplay->setAlternatingRowColors(true);
}

void MyWorkWindow::on_actionSingle_triggered()
{	

	try
	{
		
		if (mycamer->isFinished() == false)
		{
			//qDebug() << "********************Warnig err of getimage";
			//save_warning_message(tr("取图线程暂时堵塞"));
			//mycamer->wait();	
	    }

		mycamer->start(QThread::InheritPriority);

	}
	catch (const std::exception& err)
	{
		qDebug() << err.what();
		save_warning_message(err.what());
	}
}

void MyWorkWindow::on_actioncontinuity_triggered(bool z)
{
	usecontinulabel = z;
	if (z)
	{
       try
	   {
    		if (mycamer->isRunning())
    		{
	     		mycamer->wait();
	    	}
		   mycamer->start(QThread::InheritPriority);
	   }
	        catch (const std::exception& err)
            {
				qDebug() << err.what();
				save_warning_message(err.what());
	        }
	}
}

void MyWorkWindow::butten_changge(const FriendFuntion::MyUser& zz , const bool& z)
{
	if (zz == FriendFuntion::MyUser::Engineer)
	{
		ui.actionOpencamera->setEnabled(z);
		ui.actionSingle->setEnabled(z);
		ui.actioncontinuity->setEnabled(z);
		ui.actionCalib->setEnabled(z);
		ui.actionParameter->setEnabled(z);
		ui.actionUseDemo->setEnabled(z);
		ui.actionSaveimage->setEnabled(z);
		ui.actionuseAlo->setEnabled(z);
	}
	else if (zz == FriendFuntion::MyUser::Player)
	{
		ui.actionOpencamera->setEnabled(z);
		ui.actionParameter->setEnabled(z);
		//ui.actionSingle->setEnabled(z);
		//ui.actioncontinuity->setEnabled(z);
		//ui.actionUseDemo->setEnabled(z);
		//ui.actionSaveimage->setEnabled(z);
	}
	
}

void MyWorkWindow::save_warning_message(const QString& warning)
{
	emit savewarningmessage(WorkName +":   " + warning);
}

void MyWorkWindow::save_production_data()
{
	savealoerrcodetext->set_dayandnight(m_isdaynight, m_daytimehouse, m_daytimemin, m_nighttimehouse, m_nighttimemin);
	savealoerrcodetext->savepath = qsaveworkdatapath + "/ProductionData/";
	savealoerrcodetext->savewhat = ErrorcodeMessageBuffer;
	savealoerrcodetext->start(QThread::LowestPriority);
	ErrorcodeMessageBuffer.clear();
}

void MyWorkWindow::save_production_data(const QString& errcode)
{
	ErrorcodeMessageBuffer.push_back(errcode);
	if (!savealoerrcodetext->isRunning() && ErrorcodeMessageBuffer.size() >= 1)
	{
		save_production_data();
	}
}

void MyWorkWindow::Send_Data_TCP(const QString& data)
{
	emit sig_SendData_TCP(workparametricwindow->ui.Communication_TCP->currentText(),data);
}

void MyWorkWindow::Send_Data_UDP(const QString& data)
{
	emit sig_SendData_UDP(workparametricwindow->ui.Communication_UDP->currentText(), data);
}

void MyWorkWindow::mouseDoubleClickEvent(QMouseEvent* event)
{
	on_ImageView(true);
}

void MyWorkWindow::contextMenuEvent(QContextMenuEvent* event)
{
		QPoint pos = event->pos();
		pos = this->mapToGlobal(pos);
		QMenu* menu = new QMenu(this);

		QAction* loadImage = new QAction(tr("预览图像"));
		QObject::connect(loadImage, &QAction::triggered, this, &MyWorkWindow::on_ImageView);
		menu->addAction(loadImage);

		QAction* saveImage = new QAction(tr("保存图像"));
		QObject::connect(saveImage, &QAction::triggered, this, &MyWorkWindow::on_SaveImage);
		menu->addAction(saveImage);
		                       
		menu->exec(pos);
}

void MyWorkWindow::on_actionCalib_triggered()
{
	switch (Workcalib)
	{
	case FriendFuntion::Area_Scan_Calib: 
		break;
	case FriendFuntion::Line_Scan_Calib:
		break;
	case FriendFuntion::Three_Dimension_Calib:
		break;
	default:
		break;
	}
	
}

void MyWorkWindow::on_actionUseDemo_triggered()
{	
	QString filename;
	try
	{

		filename = QFileDialog::getOpenFileName(this,
			tr("选择图像"),
			"",
			("Images (*.png *.bmp *.jpg *.tif  )"));
		if (filename.isEmpty())
		{
			return;
		}
		else
		{
			string str = filename.toStdString();  // 将filename转变为string类型；
			Mat image = imread(str);

			if (usealolabel)
			{
				get_image(0,image);
			}
			else
			{
				show_image(image);
			}

		}
	}
	catch (const std::exception& err)
	{
		showtext(tr("测试失败"));
	}
	
}

void MyWorkWindow::on_actionuseAlo_triggered(bool z)
{
	usealolabel = z;
}

void MyWorkWindow::on_actionParameter_triggered()
{
	workparametricwindow->setWindowTitle("参数设置   " + WorkName);
	workparametricwindow->hide();
	workparametricwindow->show();
}