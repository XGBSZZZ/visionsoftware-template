#include "CommunicationAdd.h"
#include "qdebug.h"

CommunicationAdd::CommunicationAdd(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//setAttribute(Qt::WA_DeleteOnClose);

	//主窗口关闭就无了
	setAttribute(Qt::WA_QuitOnClose, false);

	 /*
     * 很奇怪：输入 \ ,打印文本为 \\,
     * 正则表达式用\\\\ 来表示\\ ，防止用户 输入 \
    */
	QRegExp rx = QRegExp("[^\\\\/:*?\"<>|]*"); //限制以下特殊符号在lineEdit中的输入
	QRegExpValidator* validator = new QRegExpValidator(rx);
	//ui->lineEdit_templateName_->setValidator(validator);
	ui.communicationName->setValidator(validator);
}

CommunicationAdd::~CommunicationAdd()
{
	qDebug() << "delete communicationadd";
}

void CommunicationAdd::on_GG_clicked()
{
	emit GG_Change();
}

void CommunicationAdd::on_OK_clicked() 
{
	emit Add_Change(ui.communicationName->text(),ui.communicationtype->currentIndex());
}
