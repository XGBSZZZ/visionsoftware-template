#pragma once

#include <QObject>
#include <QtWidgets>
#include <QList>
#include <QDataStream>
#include <qcombobox.h>
#include "qspinbox.h"
#include "Useme.h"

class WidgetInfo
{
public:
    QString name;
    QString value;

    //ÖØÔØÔËËã·û
    friend QDataStream& operator << (QDataStream& dataStream, const WidgetInfo& wi) {
        dataStream << wi.name << wi.value;
        return dataStream;
    }

    friend QDataStream& operator >> (QDataStream& dataStream, WidgetInfo& wi) {
        dataStream >> wi.name >> wi.value;
        return dataStream;
    }
};

class QtWidgetsSaveLoad : public QObject
{
	Q_OBJECT

public:
	QtWidgetsSaveLoad(QString path , QObject *parent = 0);
	~QtWidgetsSaveLoad();
    void addWidget(QLineEdit* w);

    void saveToFile(QWidget* parent);
    void loadFromFile(QWidget* parent, const bool& loadlist = true, const bool& loadchelist = true, 
        const bool& loadcomlist = true, const bool& loadspilist = true);


    QList <WidgetInfo> widgetInfoList;
    QString allpath;
protected:

   

    void gatherChildrenWidgetInfo(QWidget* parent);
    void setChildrenWidgetValue(QWidget* parent, const bool& loadlist = true, const bool& loadchelist = true, const bool& loadcomlist = true, const bool& loadspilist = true);

};
