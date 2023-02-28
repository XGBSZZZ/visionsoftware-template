
#include "Useme.h"

#include "MainWindows.h"
#include <QtWidgets/QApplication>
#include "QLoadingWidget.h"
#include <QMetaType>
#include "Useopencv.h"
#include "DalsaCameraInterface.h"

#include "qfile.h"
#include "FriendFuntion.h"

int main(int argc, char *argv[])
{
    //cv::Mat zzzzzzzzzz;

    //if (zzzzzzzzzz.empty())
    //{
    //    qDebug() << "四大皆空111111";
    //    return 0;
    //}

    //cv::imshow("d", zzzzzzzzzz);
    //cv::waitKey();

    //zzzzzzzzzz.release();

    //if (zzzzzzzzzz.empty())
    //{
    //    qDebug() << "四大皆空222222";
    //}

    //return 0;

   // Q_DECLARE_METATYPE(cv::Mat);
   
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<Alooutputdata>("Alooutputdata");
    qRegisterMetaType<std::string>("std::string");
//    QTranslator* translator = new QTranslator;
//  //  qApp->removeTranslator(translator);
////	delete trans;
// 
//   // translator = new QTranslator;
//	translator->load("E://TF7/QTandOpencv/x64/Debug/qtandopencv_en.qm");
//	qApp->installTranslator(translator);
//	//ui.retranslateUi(this); //刷新界面字符串
    QApplication a(argc, argv);
 
    cv::FileStorage UsersDataread("workdata/languageData.yml", cv::FileStorage::READ);

    std::string language;

    UsersDataread["language"] >> language;

    UsersDataread.release();

    if (language == "English")
    {
        //切换语言
        QTranslator* translator = new QTranslator;
#ifdef _DEBUG
        translator->load("x64/Debug/qtandopencv_en.qm");
#else  
        translator->load("x64/Release/qtandopencv_en.qm");
#endif // DEBUG
        a.installTranslator(translator);
    }


    MainWindows w;


    return a.exec();
}
