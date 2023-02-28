#include "ProductsAdd.h"

ProductsAdd::ProductsAdd(QComboBox* combox,QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//setAttribute(Qt::WA_DeleteOnClose);
	setAttribute(Qt::WA_QuitOnClose, false);

	for (int i = 0; i < combox->count(); i++)
	{
		ui.comboBox->addItem(combox->itemText(i));
	}
}

ProductsAdd::~ProductsAdd()
{
	qDebug() << "ProductsAdd out";
}
