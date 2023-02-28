#pragma once

#include "Useme.h"


#include <QDialog>
#include "ui_UserAdmini.h"

class UserAdmini : public QDialog
{
	Q_OBJECT

public:
	explicit UserAdmini(QWidget *parent = Q_NULLPTR);
	~UserAdmini();

private:
	Ui::UserAdmini ui;
private slots:
	virtual void on_OK_clicked();
	virtual void on_EX_clicked();
signals:
	void passswordchange(const std::string&, const std::string&);
};
