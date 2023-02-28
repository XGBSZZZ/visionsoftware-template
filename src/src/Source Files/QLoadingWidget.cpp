#include "Useme.h"
#include "QLoadingWidget.h"

QLoadingWidget::QLoadingWidget(QWidget *parent)
	: QWidget(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setWindowFlags(Qt::FramelessWindowHint);//无边框
    setAttribute(Qt::WA_TranslucentBackground);//背景透明

    //屏幕居中显示
    int frmX = width();
    int frmY = height();

    QDesktopWidget w;
    int deskWidth = w.width();
    int deskHeight = w.height();

    QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
    move(movePoint);

    //加载gif图片
    QMovie* movie = new QMovie("myfile/lodingimage.gif");
    
    movie->setScaledSize(QSize(200,200));
    ui.label->setMovie(movie);
    movie->start();

}

QLoadingWidget::~QLoadingWidget()
{
    qDebug() << "QLoadingWidget out";
}
