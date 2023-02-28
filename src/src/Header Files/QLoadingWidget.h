#pragma once

#include <QWidget>
#include "ui_QLoadingWidget.h"
#include <QDialog>
#include <QDesktopWidget>
#include <QMovie>

class QLoadingWidget : public QWidget
{
	Q_OBJECT

public:
	explicit QLoadingWidget(QWidget *parent = Q_NULLPTR );
	~QLoadingWidget();

private:
	Ui::QLoadingWidget ui;
};
