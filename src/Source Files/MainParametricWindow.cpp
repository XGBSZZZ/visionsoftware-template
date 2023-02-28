#include "MainParametricWindow.h"
#include "qlabel.h"
#include "qlineedit.h"
#include "warning.h"

MainParametricWindow::MainParametricWindow( unsigned int worknumb, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_QuitOnClose, false);
	//ѹ������Ƶ��
	timeer = new QLineEdit(this);
	timeer->setText("45");
	timeer->setGeometry(140, 80 + 13, 60, 20);
	timeer->setFont((QFont("Timers", 13, QFont::Normal)));
	timeer->setAlignment(Qt::AlignCenter);
	timeer->setObjectName("timer");

	//OK / NG ͼ��ʾƵ��
	connect(ui.okprice, SIGNAL(stateChanged(int)), this, SLOT(mainparametric_change(int)));
	connect(ui.ngprice, SIGNAL(stateChanged(int)), this, SLOT(mainparametric_change(int)));


	worknumb++;
	for (int i = 0; i < worknumb; i++)
	{
		QLabel* qlabel = new QLabel(ui.groupBox);
		qlabel->setGeometry(10, 125 + 40 * i, 150, 25);
		qlabel->setFont((QFont("Timers", 13, QFont::Normal)));
		qlabel->setAlignment(Qt::AlignCenter);
		if (i == 0)
		{
			qlabel->setText(tr("ǿ�Ʊ���ȡͼ"));
		}
		else
		{
            qlabel->setText(tr("��λ") + QString::number(i) + tr(" ����"));
		}
		
		QCheckBox* qcheckbox = new QCheckBox(ui.groupBox);
		qcheckbox->setGeometry(180, 128 + 40 * i, 60, 20);
		qcheckbox->setChecked(false);
		qcheckbox->setObjectName("qche" + QString::number(i));
		allqcheckbox.push_back(qcheckbox);
	}

	//��ҹ���ʼ��
	connect(ui.isdaynight, SIGNAL(stateChanged(int)), this, SLOT(mainparametric_change(int)));
	connect(ui.day_House, SIGNAL(valueChanged(int)), this, SLOT(mainparametric_change(int)));
	connect(ui.day_min, SIGNAL(valueChanged(int)), this, SLOT(mainparametric_change(int)));
	connect(ui.night_House, SIGNAL(valueChanged(int)), this, SLOT(mainparametric_change(int)));
	connect(ui.night_min, SIGNAL(valueChanged(int)), this, SLOT(mainparametric_change(int)));
  
	//�Զ����ͳ�����ݳ�ʼ��
	connect(ui.autoclear_production, SIGNAL(stateChanged(int)), this, SLOT(mainparametric_change(int)));

	//���ͼ���ʼ��
	connect(ui.isclearpicture, SIGNAL(stateChanged(int)), this, SLOT(mainparametric_change(int)));
	connect(ui.ismustclear, SIGNAL(stateChanged(int)), this, SLOT(mainparametric_change(int)));
	connect(ui.picture_day, SIGNAL(valueChanged(int)), this, SLOT(mainparametric_change(int)));
	connect(ui.mustsize, SIGNAL(valueChanged(int)), this, SLOT(mainparametric_change(int)));
	


	//���ش浵
	wedgetsaveload = new QtWidgetsSaveLoad("workdata/MainParametric.txt");
	wedgetsaveload->loadFromFile(this);

	//��ʼ���洢·��
	if (ui.workdata_savepath->text() == "")
	{
		ui.workdata_savepath->setText("workdata");
	}


}

MainParametricWindow::~MainParametricWindow()
{
	wedgetsaveload->saveToFile(this);
}

void MainParametricWindow::closeEvent(QCloseEvent* e)
{
	if (ui.night_House->value() < ui.day_House->value())
	{
		//����              ����                OK��ť  
		QMessageBox::warning(this, tr("����!"), tr("ҹ��ʱ��С�ڰװ�ʱ��!"), QMessageBox::Yes);
	}


	wedgetsaveload->saveToFile(this);
}

void MainParametricWindow::on_workdata_savapath_change_clicked()
{

	QString filename;
	try
	{
		filename = QFileDialog::getExistingDirectory(this, tr("��ѡ��ͼƬ����·��..."), "./");
		if (filename.isEmpty())
		{
			return;
		}
		else
		{
			ui.workdata_savepath->setText(filename);
			emit savapathchange(filename);
		}
	}
	catch (const std::exception& err)
	{
		
	}
}

void MainParametricWindow::mainparametric_change(int state)
{
	emit day_night_change();
}


