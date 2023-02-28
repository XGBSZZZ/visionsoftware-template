#include "Useme.h"
#include "FriendFuntion.h"
#include <fstream>


using namespace std;

QImage FriendFuntion::cvMat2QImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1  
    if (mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)  
        image.setColorCount(256);
        for (int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat  
        uchar* pSrc = mat.data;
        for (int row = 0; row < mat.rows; row++)
        {
            uchar* pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3  
    else if (mat.type() == CV_8UC3)
    {
        // Copy input Mat  
        const uchar* pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat  
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if (mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        // Copy input Mat  
        const uchar* pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat  
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

cv::Mat FriendFuntion::QImage2cvMat(const QImage& image , const int& conn ) 
{
    cv::Mat mat;
    qDebug() << image.format();
    switch (image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        if (conn == 3)
        {
            cv::cvtColor(mat, mat, CV_RGBA2RGB);//CV_RGBA2RGB表示4通道转成3通道
        }
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        if (conn == 3)
        {
            cv::cvtColor(mat, mat, CV_GRAY2RGB);
        }
        break;
    }
    return mat;
}

void FriendFuntion::isDirExist(const std::string filename)
{	//判断文件夹是否存在，不存在则创建
    const char* dir = filename.data();
    //const char* dir = "D:\\ASDF\\123.txt";   //_access也可用来判断文件是否存在

    if (_access(dir, 0) == -1)
    {
        _mkdir(dir);
       // qDebug() << "该文件夹不存在，已自动创建" << endl;
    }
    else
    {
      //  qDebug() << "文件夹AS已经存在" << endl;
    }
}

bool FriendFuntion::isFileExist(const std::string filename)
{
    bool isopen = false;
    const char* fname = filename.data();

    fstream fs;
    fs.open(fname, ios::in);

    if (!fs)
    {
      //  cout << "不存在该文件" << endl;

        //创建文件
        ofstream fout(fname);
        if (fout)
        {
            // 如果创建成功
         //     fout << "写入内容，也可以不写入" << endl;

            // 执行完操作后关闭文件句柄
            fout.close();
        }
    }
    else
    {
        isopen = true;
      //  cout << "该文件已经存在" << endl;
    }
    
    return isopen;
}

QString FriendFuntion::str2qstr(const std::string str)
{
    return QString::fromLocal8Bit(str.data());
}

std::string FriendFuntion::qstr2str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return std::string(cdata);
}