#include "WorkParametricWindow.h"
#include "qfont.h"
#include <warning.h>

WorkParametricWindow::WorkParametricWindow(QString savepaths, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_QuitOnClose, false);
	savepath = savepaths;

	//QPalette pal = this->palette();
	//QBrush brush = pal.background();
	//QColor col = brush.color();
	//qDebug() << col;

	connect(ui.ok_souce, SIGNAL(clicked()), this, SLOT(On_checkBox()));
	connect(ui.ok_result, SIGNAL(clicked()), this, SLOT(On_checkBox()));
	connect(ui.ng_souce, SIGNAL(clicked()), this, SLOT(On_checkBox()));
	connect(ui.ng_result, SIGNAL(clicked()), this, SLOT(On_checkBox()));



	ui.li_oksouce_good->setMaxLength(3);  //最多输入3个字符
	ui.li_okresult_good->setMaxLength(3);
	ui.li_ngsouce_good->setMaxLength(3);
	ui.li_ngresult_good->setMaxLength(3);
	//QRegExp regx("[1-9][0-9][0]");

	//QRegExp rx("^  (  ([1-9])  |   ([1-9][0-9])   |   (100)   )   $");//输入范围为【0-100】
	QRegExp rx("^(\[1-9]|[1-9]\\d|(1[0][0]))$");//输入范围为【0-100】
	QValidator* validator = new QRegExpValidator(rx);


	ui.li_oksouce_good->setValidator(validator);
	ui.li_okresult_good->setValidator(validator);
	ui.li_ngsouce_good->setValidator(validator);
	ui.li_ngresult_good->setValidator(validator);

	on_btn_alo_input_clicked();
	ui.scrollArea->setFrameShape(QFrame::NoFrame);	
}

WorkParametricWindow::~WorkParametricWindow()
{
	save_Data();
}

void WorkParametricWindow::otherinit()
{
	int z = 0;

	//算法输入参数Ui 布局

	const int parametric_x = 15; //算法输入  整体X轴偏移
	const int parametric_y = 0; //算法输入  整体X轴偏移
	const int label_dx = 30;  //算法输入   间距偏移

	for (int j = 0; j < 12; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			//参数名初始化
			QLineEdit* label_edit = new QLineEdit(ui.groupBox_alo);
			//QFont* labelfont = new QFont();
			label_edit->setFont((QFont("Timers", 13, QFont::Normal)));
			label_edit->setGeometry(10 + i * (280 + label_dx ) + parametric_x, 50 + j * 50 + parametric_y, 110 + label_dx, 25);
			label_edit->setAlignment(Qt::AlignRight);
			label_edit->setStyleSheet("background:transparent;border-width:0;border-style:outset");
			label_edit->setPlaceholderText(tr("未启用"));
			label_edit->setObjectName("label"+QString::number(z));
			//label_edit->setText("难受啊");
			//all_label.push_back(label_edit);


			//参数初始化
			QLineEdit* parametric_edit = new QLineEdit(ui.groupBox_alo);
			parametric_edit->setPlaceholderText("0.000");
			//QFont* labelfont = new QFont();
			parametric_edit->setFont((QFont("Timers", 13, QFont::Normal)));
			parametric_edit->setGeometry(130 + i * (280 + label_dx) + label_dx + parametric_x , 50 + j * 50 + parametric_y, 150, 25);
			parametric_edit->setAlignment(Qt::AlignCenter);
			parametric_edit->setObjectName("numb" + QString::number(z));
			//parametric_edit->setText("这个世界还能不能好了");
			connect(parametric_edit, SIGNAL(textChanged(const QString&)), this, SLOT(on_aloparametric(const QString&)));
			parametric_edit->setMaxLength(13);
			QDoubleValidator* validator = new QDoubleValidator(-999999999, 999999999,6);
			parametric_edit->setValidator(validator);
			all_parametric.push_back(parametric_edit);
			z++;
		}
	}
		
	int zz = 0;

	for (int j = 0; j < 12; j++)
	{
		for (int i = 0; i < 2; i++)
		{
			//参数名初始化
			QLineEdit* name_edit = new QLineEdit(ui.groupBox);
			//QFont* labelfont = new QFont();
			name_edit->setFont((QFont("Timers", 13, QFont::Normal)));
			name_edit->setGeometry(70 + i * 440, 50 + 50 * j, 200, 25);
			name_edit->setAlignment(Qt::AlignHCenter);
			name_edit->setStyleSheet("background:transparent;border-width:0;border-style:outset");
			name_edit->setPlaceholderText(tr("未启用"));
			if (zz <= 3)
			{
				switch (zz)
				{

				case(0):
				{
					name_edit->setText(tr("序号"));
					name_edit->setReadOnly(true);
					break;
				}
				case(1):
				{
					name_edit->setText(tr("时间"));
					name_edit->setReadOnly(true);
					break;
				}
				case(2):
				{
					name_edit->setText(tr("产品码"));
					name_edit->setReadOnly(true);
					break;
				}
				case(3):
				{
					name_edit->setText(tr("结果"));
					name_edit->setReadOnly(true);
					break;
				}
				}
			}
			else
			{
				name_edit->setObjectName("name" + QString::number(z));
			}
			all_outputname.push_back(name_edit);
			connect(name_edit, &QLineEdit::textChanged, [=](const QString&) {
				emit sig_output();
				emit sig_title();
				});

			QCheckBox* save_checkbox = new QCheckBox(ui.groupBox);
			save_checkbox->setFont((QFont("Timers", 13, QFont::Normal)));
			save_checkbox->setGeometry(290 + i * 440, 50 + 50 * j, 55, 25);
			save_checkbox->setText(tr("保存"));
			save_checkbox->setObjectName("save" + QString::number(z));
			all_save.push_back(save_checkbox);
			connect(save_checkbox, &QCheckBox::stateChanged, [=](int z) {
				if (z == 0)
				{
					save_checkbox->setStyleSheet("background-color: rgb(240, 240, 240); color: rgb(0, 0, 0); ");
				}
				else
				{
					save_checkbox->setStyleSheet("background-color: rgb(200, 230, 255); color: rgb(100, 170, 255); ");
				}
				emit sig_output();
				emit sig_title();
				});

			QCheckBox* tcp_checkbox = new QCheckBox(ui.groupBox);
			tcp_checkbox->setFont((QFont("Timers", 13, QFont::Normal)));
			tcp_checkbox->setGeometry(350 + i * 440, 50 + 50 * j, 55, 25);
			tcp_checkbox->setText(tr("TCP"));
			tcp_checkbox->setObjectName("tcp" + QString::number(z));
			all_tcp.push_back(tcp_checkbox);
			connect(tcp_checkbox, &QCheckBox::stateChanged, [=](int z) {
				if (z == 0)
				{
					tcp_checkbox->setStyleSheet("background-color: rgb(240, 240, 240); color: rgb(0, 0, 0); ");
				}
				else
				{
					tcp_checkbox->setStyleSheet("background-color: rgb(200, 230, 255); color: rgb(100, 170, 255); ");
				}
				emit sig_output(); });

			QCheckBox* udp_checkbox = new QCheckBox(ui.groupBox);
			udp_checkbox->setFont((QFont("Timers", 13, QFont::Normal)));
			udp_checkbox->setGeometry(410 + i * 440, 50 + 50 * j, 55, 25);
			udp_checkbox->setText(tr("UDP"));
			udp_checkbox->setObjectName("udp" + QString::number(z));
			all_udp.push_back(udp_checkbox);
			connect(udp_checkbox, &QCheckBox::stateChanged, [=](int z) {
				if (z == 0)
				{
					udp_checkbox->setStyleSheet("background-color: rgb(240, 240, 240); color: rgb(0, 0, 0); ");
				}
				else
				{
					udp_checkbox->setStyleSheet("background-color: rgb(200, 230, 255); color: rgb(100, 170, 255); ");
				}
				emit sig_output(); });




			z++;
			zz++;
		
		}
	}


	load_Data();
	On_checkBox();
	connect(ui.comboBox_products, SIGNAL(activated(int)), this, SLOT(on_comboxproducts(int)));
	emit sig_output();


}

void WorkParametricWindow::closeEvent(QCloseEvent* e)
{
	save_Data();
}

void WorkParametricWindow::save_Data()
{
	QString savename = (ui.comboBox_products->currentText() == "") ? "default" : ui.comboBox_products->currentText();

	QFile ProductsAdminifile;
	ProductsAdminifile.setFileName(savepath + "/Products/ProductsAdmini.txt");
	if (!ProductsAdminifile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
		qDebug() << "产品通讯文件保存失败";
		emit show_text(tr("产品文件保存失败"));
	}
	QTextStream SaveProductsAdmini(&ProductsAdminifile);
	qDebug() << "看我保存了什么  " + savename;
	SaveProductsAdmini << savename + "\n";

	for (int i = 0; i < ui.comboBox_products->count(); i++)
	{
		SaveProductsAdmini << ui.comboBox_products->itemText(i) + "\n";
	}



	ProductsAdminifile.close();

	wedgetsaveload = std::make_shared<QtWidgetsSaveLoad>(savepath + "/Products/" + "tfwleft.txt");
	wedgetsaveload->saveToFile(ui.widget_left);

	wedgetsaveload = std::make_shared<QtWidgetsSaveLoad>(savepath + "/Products/" + savename + ".txt");
	wedgetsaveload->saveToFile(ui.widget_right);
}

void WorkParametricWindow::load_Data()
{
	int tf_index = 0;

	QFile ProductsAdminifile;
	ProductsAdminifile.setFileName(savepath + "/Products/ProductsAdmini.txt");
	if (!ProductsAdminifile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "产品文件加载失败";
		emit show_text(tr("产品文件加载失败"));
	}
	QTextStream SaveProductsAdmini(&ProductsAdminifile);

	QString name = "default";
	//一直读，直至读取失败
	while (!SaveProductsAdmini.atEnd()) {

		QString str;
		//从文件中读取一个字符串
		SaveProductsAdmini >> str;

		if (tf_index == 0)
		{
			name = str;
		}
		else
		{
			if (str != "")
			{
				ui.comboBox_products->addItem(str);
			}
		}

		tf_index++;
	}

	tempname = name;
	ui.comboBox_products->setCurrentText(name);
	qDebug() << savepath + "/Products/" + name + ".txt";

	wedgetsaveload = std::make_shared<QtWidgetsSaveLoad>(savepath + "/Products/" + "tfwleft.txt");
	wedgetsaveload->loadFromFile(ui.widget_left);

	wedgetsaveload = std::make_shared<QtWidgetsSaveLoad>(savepath + "/Products/"+ name +".txt");
	wedgetsaveload->loadFromFile(ui.widget_right);

}

void WorkParametricWindow::on_aloparametric(const QString&)
{
	emit aloparametric_change();
}

void WorkParametricWindow::communication_chage(const QString& name, const QString& type)
{
	if (type == "TCPserver")
	{
		for (int i = 0; i < ui.Communication_TCP->count(); i++)
		{
			if (ui.Communication_TCP->itemText(i) == name)
			{
				return;
			}
		}

		ui.Communication_TCP->addItem(name);
	}
	else if (type == "TCPclient")
	{
		for (int i = 0; i < ui.Communication_TCP->count(); i++)
		{
			if (ui.Communication_TCP->itemText(i) == name)
			{
				return;
			}
		}
		ui.Communication_TCP->addItem(name);
	}
	else if (type == "UDP")
	{
		for (int i = 0; i < ui.Communication_UDP->count(); i++)
		{
			if (ui.Communication_UDP->itemText(i) == name)
			{
				return;
			}
		}
		ui.Communication_UDP->addItem(name);
	}
}

void WorkParametricWindow::communication_delete(const QString& name, const QString& type)
{
	    int tf_Index = -999;

		if (type == "TCPserver")
		{
			if (name == ui.Communication_TCP->currentText())
			{
				ui.Communication_TCP->setCurrentIndex(0);
			}

			for (int i = 0; i < ui.Communication_TCP->count(); i++)
			{
				tf_Index = name == ui.Communication_TCP->itemText(i) ? i : tf_Index + 0;
			}

			if (tf_Index >= 0)
			{
				ui.Communication_TCP->removeItem(tf_Index);
			}
		}
		else if (type == "TCPclient")
		{
			if (name == ui.Communication_TCP->currentText())
			{
				ui.Communication_TCP->setCurrentIndex(0);
			}

			for (int i = 0; i < ui.Communication_TCP->count(); i++)
			{
				tf_Index = name == ui.Communication_TCP->itemText(i) ? i : tf_Index + 0;
			}

			if (tf_Index >= 0)
			{
				ui.Communication_TCP->removeItem(tf_Index);
			}
		}
		else if (type == "UDP")
		{
			if (name == ui.Communication_UDP->currentText())
			{
				ui.Communication_UDP->setCurrentIndex(0);
			}

			for (int i = 0; i < ui.Communication_UDP->count(); i++)
			{
				tf_Index = name == ui.Communication_UDP->itemText(i) ? i : tf_Index + 0;
			}

			if (tf_Index >= 0)
			{
				ui.Communication_UDP->removeItem(tf_Index);
			}
		}


}

void WorkParametricWindow::on_btn_alo_input_clicked()
{
	ui.groupBox_2->setTitle(tr("算法输入参数"));
	ui.groupBox->hide();
	ui.groupBox_alo->show();
}

void WorkParametricWindow::on_btn_alo_output_clicked()
{
	ui.groupBox_2->setTitle(tr("算法输出参数"));
	ui.groupBox_alo->hide();
	ui.groupBox->show();

}

void WorkParametricWindow::on_btn_addproducts_clicked()
{
	//connect(btn, &CommunicationBtn::sig_Add_Change, [=](const QString& name, const QString& type) {
	//	emit sig_Add_Change(name, type);
	//	emit sig_Show_Text(tr("新增通讯端口: ") + type + "   " + name);
	//	});
	if (nullptr == productsadd)
	{
		productsadd = new ProductsAdd(ui.comboBox_products);
	}

	connect(productsadd->ui.btn_add, &QPushButton::clicked, [=](bool) {

		if (productsadd->ui.lineEdit_2->text() == "default")
		{
			emit show_text(tr("不能添加该名称的产品名,请更换后再试"));
			return;
		}

		if (productsadd->ui.lineEdit_2->text().size() == 0)
		{
			emit show_text(tr("新增的产品名不能为空白"));
			return;
		}

		for (int i = 0; i < ui.comboBox_products->count(); i++)
		{
			if (ui.comboBox_products->itemText(i) == productsadd->ui.lineEdit_2->text())
			{
				emit show_text(tr("不能重复添加同一款产品"));
				return;
			}
		}
		emit show_text(tr("产品增加: ") + productsadd->ui.lineEdit_2->text());
		ui.comboBox_products->addItem(productsadd->ui.lineEdit_2->text()); 
		productsadd->ui.comboBox->addItem(productsadd->ui.lineEdit_2->text());
		ui.comboBox_products->setCurrentText(productsadd->ui.lineEdit_2->text());
		productsadd->ui.comboBox->setCurrentText(productsadd->ui.lineEdit_2->text());
		productsadd->ui.lineEdit_2->clear();
		});

	connect(productsadd->ui.btn_delete, &QPushButton::clicked, [=](bool) {

		if (productsadd->ui.comboBox->currentText() == "default")
		{
			emit show_text(tr("不能删除默认的产品名"));
			return;
		}

		int tf_Index = productsadd->ui.comboBox->currentIndex();
		if (tf_Index >= 0)
		{
			productsadd->ui.comboBox->removeItem(tf_Index);
			ui.comboBox_products->removeItem(tf_Index);
		}
		});

	connect(productsadd->ui.btn_change, &QPushButton::clicked, [=](bool) {

		if (productsadd->ui.lineEdit->text().size() == 0)
		{
			emit show_text(tr("更改的名字为空白,产品名更改失败"));
			return;
		}

		if (productsadd->ui.lineEdit->text() == "default")
		{
			emit show_text(tr("不能改变默认的产品名"));
			return;
		}

		for (int i = 0; i < ui.comboBox_products->count(); i++)
		{
			if (ui.comboBox_products->itemText(i) == productsadd->ui.lineEdit->text())
			{
				emit show_text(tr("不能更改为同名产品"));
				return;
			}
		}

		emit show_text(tr("产品名更改: ") + productsadd->ui.comboBox->currentText() + " ->" + productsadd->ui.lineEdit->text());

		int tf_Index = productsadd->ui.comboBox->currentIndex();

		if (tf_Index >= 0)
		{
			productsadd->ui.comboBox->removeItem(tf_Index);
			ui.comboBox_products->removeItem(tf_Index);
		}

		ui.comboBox_products->addItem(productsadd->ui.lineEdit->text());
		productsadd->ui.comboBox->addItem(productsadd->ui.lineEdit->text());
		productsadd->ui.comboBox->setCurrentText(productsadd->ui.lineEdit->text());
		ui.comboBox_products->setCurrentText(productsadd->ui.lineEdit->text());	
		productsadd->ui.lineEdit->clear();
		});

	productsadd->hide();
	productsadd->show();
}

void WorkParametricWindow::on_comboxproducts(int)
{

	qDebug() << "鼠鼠我啊,是上次的来的呢: " + tempname;
	wedgetsaveload = std::make_shared<QtWidgetsSaveLoad>(savepath + "/Products/" + tempname + ".txt");
	wedgetsaveload->saveToFile(ui.widget_right);

	qDebug() << "蜀黍我啊,就要执行了呢: " + ui.comboBox_products->currentText();
	wedgetsaveload = std::make_shared<QtWidgetsSaveLoad>(savepath + "/Products/" + ui.comboBox_products->currentText() + ".txt");
	wedgetsaveload->loadFromFile(ui.widget_right, true, true, true, true);
	tempname = ui.comboBox_products->currentText();
	qDebug() << "叔叔我啊,我马上就要析构了呢";

}

void WorkParametricWindow::On_checkBox()
{
	emit workparametric_state(ui.ok_souce->isChecked(), ui.ok_result->isChecked(), ui.ng_souce->isChecked(), ui.ng_result->isChecked());

}
