#include "QtWidgetsSaveLoad.h"
#include <QDataStream>


QtWidgetsSaveLoad::QtWidgetsSaveLoad(QString path,QObject *parent)
	: QObject(parent)
{
    allpath = path;
}

QtWidgetsSaveLoad::~QtWidgetsSaveLoad()
{
    qDebug() << "QtWidgetsSaveLoad out";
}

void QtWidgetsSaveLoad::addWidget(QLineEdit* w)
{

    WidgetInfo wi;
    widgetInfoList.clear();

    wi.name = w->objectName();
    wi.value = w->text();

    widgetInfoList.append(wi);

}

void  QtWidgetsSaveLoad::gatherChildrenWidgetInfo(QWidget* parent)
{
    widgetInfoList.clear();

    // �ռ��ؼ���Ϣ�����浽 QList  WidgetInfo
    QList <QLineEdit*> list = parent->findChildren<QLineEdit*>();
    for (int i = 0; i < list.size(); i++)
    {
        QLineEdit* le = list.at(i);
        if (le->objectName() == "")
            qDebug() << "widget QLineEdit should run setObjectName() to set a name;";
        else {
            WidgetInfo wi;
            wi.name = le->objectName();
            wi.value = le->text();
            widgetInfoList.append(wi);
        }
    }

    QList<QCheckBox*> chelist = parent->findChildren<QCheckBox*>();
    for (int i = 0; i < chelist.size(); i++)
    {
        QCheckBox* le = chelist.at(i);
        if (le->objectName() == "")
            qDebug() << "widget QCheckBox should run setObjectName() to set a name;";
        else {
            WidgetInfo wi;
            wi.name = le->objectName();
            wi.value = le->isChecked() ? "1" : "0";
            widgetInfoList.append(wi);
        }
    }

    QList<QComboBox*> comlist = parent->findChildren<QComboBox*>();
    for (int i = 0; i < comlist.size(); i++)
    {
        QComboBox*  le = comlist.at(i);
        if (le->objectName() == "")
            qDebug() << "widget QComboBox should run setObjectName() to set a name;";
        else {
            WidgetInfo wi;
            wi.name = le->objectName();
            wi.value = QString::number(le->currentIndex());
            widgetInfoList.append(wi);
        }
    }

    QList<QSpinBox*> spilist = parent->findChildren<QSpinBox*>();
    for (int i = 0; i < spilist.size(); i++)
    {
        QSpinBox* le = spilist.at(i);
        if (le->objectName() == "")
            qDebug() << "widget QSpinBox should run setObjectName() to set a name;";
        else {
            WidgetInfo wi;
            wi.name = le->objectName();
            wi.value = QString::number(le->value());
            widgetInfoList.append(wi);
        }
    }


}

void QtWidgetsSaveLoad::setChildrenWidgetValue(QWidget* parent,const bool& loadlist, const bool& loadchelist, 
    const bool& loadcomlist, const bool& loadspilist)
{
    QList <QLineEdit*> list = parent->findChildren<QLineEdit*>();
    QList<QCheckBox*> chelist = parent->findChildren<QCheckBox*>();
    QList<QComboBox*> comlist = parent->findChildren<QComboBox*>();
    QList<QSpinBox*> spilist = parent->findChildren<QSpinBox*>();

    for (int i = 0; i < widgetInfoList.size(); i++) {
        if (loadlist)
        {
            for (int j = 0; j < list.size(); j++)
            {
                QLineEdit* le = list.at(j);
                if (le->objectName() == widgetInfoList.at(i).name)
                    le->setText(widgetInfoList.at(i).value);
            }
        }

        if (loadchelist)
        {
            for (int j = 0; j < chelist.size(); j++)
            {
                QCheckBox* ck = chelist.at(j);
                if (ck->objectName() == widgetInfoList.at(i).name)
                    ck->setChecked(widgetInfoList.at(i).value == "1");
            }
        }

        if (loadcomlist)
        {
            for (int j = 0; j < comlist.size(); j++)
            {
                QComboBox* cb = comlist.at(j);
                if (cb->objectName() == widgetInfoList.at(i).name)
                    cb->setCurrentIndex(widgetInfoList.at(i).value.toInt());
            }
        }

        if (loadspilist)
        {
            for (int j = 0; j < spilist.size(); j++)
            {
                QSpinBox* lsp = spilist.at(j);
                if (lsp->objectName() == widgetInfoList.at(i).name)
                    lsp->setValue(widgetInfoList.at(i).value.toInt());
            }
        }
    }



    widgetInfoList.clear();

}

// ����Ҫ�����widget�������� setObjectName() ���ù����֣��ڴ���widgetʱ����
// ��Ϊ w->objectName() ����������ܷ��ض��������������
// �����ص�ֻ�� setObjectName() ���õ����� , ��QDesigner�����Ľ��棬�Ѿ�������setObjectName

void  QtWidgetsSaveLoad::saveToFile(QWidget* parent)
{

    // �ռ��ؼ���Ϣ�����浽 QList  WidgetInfo ��
    gatherChildrenWidgetInfo(parent);

    // ��  QList  WidgetInfo ���л� ���ļ���
    QFile file(allpath);

    if (file.open(QIODevice::ReadWrite)) {
        QDataStream stream(&file);
        stream << widgetInfoList;
        file.close();
    }
    else {
        qDebug() << allpath + " open error!";
    }
}

void  QtWidgetsSaveLoad::loadFromFile(QWidget* parent, const bool& loadlist, const bool& loadchelist, 
    const bool& loadcomlist, const bool& loadspilist)
{
    // ͨ�� ���л� ��ȡ�ļ�����Ϣ�� QList  WidgetInfo
    QFile file(allpath);

    if (file.open(QIODevice::ReadOnly)) {
        QDataStream stream(&file);
        stream >> widgetInfoList;
        file.close();
    }
    else {
        qDebug() << allpath + " open error!";
    }

    setChildrenWidgetValue(parent, loadlist, loadchelist, loadcomlist,loadspilist);
}
