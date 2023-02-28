#pragma once

#include <QWidget>
#include "ui_ProductsAdd.h"
#include "Useme.h"

class ProductsAdd : public QWidget
{
	Q_OBJECT

public:
	ProductsAdd(QComboBox*, QWidget *parent = Q_NULLPTR);
	~ProductsAdd();

public:
	Ui::ProductsAdd ui;
};
