#include "CommunicationAdd.h"
#include "qdebug.h"

CommunicationAdd::CommunicationAdd(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//setAttribute(Qt::WA_DeleteOnClose);

	//�����ڹرվ�����
	setAttribute(Qt::WA_QuitOnClose, false);

	 /*
     * ����֣����� \ ,��ӡ�ı�Ϊ \\,
     * ������ʽ��\\\\ ����ʾ\\ ����ֹ�û� ���� \
    */
	QRegExp rx = QRegExp("[^\\\\/:*?\"<>|]*"); //�����������������lineEdit�е�����
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
