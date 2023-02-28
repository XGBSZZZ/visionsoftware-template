#include "ProductionWindows.h"

ProductionWindows::ProductionWindows(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_QuitOnClose, false);
	//this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint  | Qt::WindowStaysOnTopHint | Qt::SubWindow );
	//this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::CustomizeWindowHint);
	//hide();
	connect(saveproductionthread, &saveProductionThread::change_shifts, this, &ProductionWindows::on_pushButton_clicked);
}

ProductionWindows::~ProductionWindows()
{
	savepricenumb = 999999999;
	save_data();
}

void ProductionWindows::load_data()
{
	QString current_date = QDateTime::currentDateTime().toString("yyyy-MM-dd"); //  yyyy��ʾ�ꣻMM��ʾ�£�dd��ʾ�գ� hh��ʾСʱ��mm��ʾ�֣�ss��ʾ�룻zzz��ʾ���룻ddd��ʾ�ܼ�
	QFile Productionfile;
	QString savepaths = savepath + "/ProductionStatistics/";

	QByteArray cdata = savepaths.toLocal8Bit();
	std::string path = std::string(cdata);
	savepaths += current_date;

	if (m_isdaynight)
	{
		QString dayandnightname;

		int house = QDateTime::currentDateTime().toString("hh").toInt(); //  hh��ʾСʱ
		int min = QDateTime::currentDateTime().toString("mm").toInt(); // mm��ʾ��
		if ((house >= m_daytimehouse && min >= m_daytimemin && house < m_nighttimehouse)
			|| (house == m_nighttimehouse && min <= m_daytimemin))
		{
			dayandnightname = "   Day shift";
		}
		else
		{
			dayandnightname = "   Night shift";
		}

		savepaths += dayandnightname;

	
	}
	else
	{	
	}

	Productionfile.setFileName(savepaths + ".csv");
	if (!Productionfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "�ļ���ʧ��";
		return;
	}
	QTextStream ReadProduction(&Productionfile);

	int wd = 0;
	while (!ReadProduction.atEnd()) {
		QString str;
		//���ļ��ж�ȡһ���ַ���
		ReadProduction >> str;


		switch (wd)
		{
		case 1:  ui.lineEdit_Sum->setText(str); break;
		case 3:  ui.lineEdit_Nice->setText(str); break;
		case 5:  ui.lineEdit_OK->setText(str); break;
		case 7:  ui.lineEdit_NG->setText(str); break;
		default:
			break;
		}

		wd++;

	}


	Productionfile.close();
}

void ProductionWindows::save_data(const bool& mustday)
{
	if (savepricenumb >= 100)
	{
		savepricenumb = 0;
		if (!saveproductionthread->isRunning())
		{
			saveproductionthread->set_pargram(savepath, m_isdaynight, m_daytimehouse, m_daytimemin, m_nighttimehouse, m_nighttimemin,
				ui.lineEdit_Sum->text(), ui.lineEdit_Nice->text(), ui.lineEdit_OK->text(), ui.lineEdit_NG->text());
			saveproductionthread->start(QThread::LowestPriority);
		}

	}
	else
	{
		savepricenumb++;
	}



	//QString current_date = QDateTime::currentDateTime().toString("yyyy-MM-dd"); //  yyyy��ʾ�ꣻMM��ʾ�£�dd��ʾ�գ� hh��ʾСʱ��mm��ʾ�֣�ss��ʾ�룻zzz��ʾ���룻ddd��ʾ�ܼ�
	//QString savepaths = savepath + "/ProductionStatistics/";
	//QByteArray cdata = savepaths.toLocal8Bit();
	//std::string path = std::string(cdata);

	//QFile Productionfile;

	//savepaths += current_date;

	//if (m_isdaynight)
	//{
	//	QString dayandnightname;

	//	int house = QDateTime::currentDateTime().toString("hh").toInt(); //  hh��ʾСʱ
	//	int min = QDateTime::currentDateTime().toString("mm").toInt(); // mm��ʾ��
	//	if ((house >= m_daytimehouse && min >= m_daytimemin && house < m_nighttimehouse)
	//		|| (house == m_nighttimehouse && min <= m_daytimemin))
	//	{
	//		dayandnightname = "   Day shift";
	//	}
	//	else
	//	{
	//		dayandnightname = "   Night shift";
	//	}
	//	dayandnightname = mustday ? "   Day shift" : "   Night shift";

	//	savepaths += dayandnightname;

	//}
	//else
	//{

	//}

	//Productionfile.setFileName(savepaths + ".txt");
	//if (!Productionfile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
	//	qDebug() << "�ļ���ʧ��";
	//}
	//QTextStream SaveProduction(&Productionfile);
	//SaveProduction << "����: " + ui.lineEdit_Sum->text() + "\n";
	//SaveProduction << "����: " + ui.lineEdit_Nice->text() + "\n";
	//SaveProduction << "OK��: " + ui.lineEdit_OK->text() + "\n";
	//SaveProduction << "NG����" + ui.lineEdit_NG->text();

	//Productionfile.close();
}

void ProductionWindows::is_ok()
{
	int ok = ui.lineEdit_OK->text().toInt();
	int sum = ui.lineEdit_Sum->text().toInt();
	double nice;

	++ok; ++sum;
	nice = double(ok) / double(sum) * 100;

	ui.lineEdit_OK->setText(QString::number(ok));
	ui.lineEdit_Sum->setText(QString::number(sum));
	ui.lineEdit_Nice->setText(QString::number(nice ,'f', 2)+"%");

	save_data();
}

void ProductionWindows::is_ng()
{
	int sum = ui.lineEdit_Sum->text().toInt();
	int ng = ui.lineEdit_NG->text().toInt();
	double nice;

	++ng; ++sum;
	nice =  double(sum - ng) / double(sum) * 100;

	ui.lineEdit_NG->setText(QString::number(ng));
	ui.lineEdit_Sum->setText(QString::number(sum));
	ui.lineEdit_Nice->setText(QString::number(nice, 'f', 2)+"%");

	save_data();
}


void ProductionWindows::on_pushButton_clicked() {

	ui.lineEdit_OK->setText("0");
	ui.lineEdit_NG->setText("0");
	ui.lineEdit_Sum->setText("0");
	ui.lineEdit_Nice->setText("100.00%");
}