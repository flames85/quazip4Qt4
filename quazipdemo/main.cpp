#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <QDebug>
//! for zip
#include <quazipfile.h>

bool GetZip(const QStringList &listWrapper, const QString &strZipName)
{
    QuaZip zip(strZipName);
    zip.setFileNameCodec("utf-8");

    if(!zip.open(QuaZip::mdCreate))
    {
        qDebug() << "error .... 1";
        return -1;
    }

    QuaZipFile outFile(&zip);
    for(int i = 0 ; i < listWrapper.size() ; i++)
    {
        qDebug() << listWrapper.at(i);
        QFileInfo file(listWrapper.at(i));

        if(file.exists())
        {
            QFile inFile ;
            QFile inFileTemp ;

            qDebug() << file.fileName();
            qDebug() << file.filePath();

            inFileTemp.setFileName(file.fileName());
            inFile.setFileName(file.filePath());

            if(!inFile.open(QIODevice::ReadOnly))
            {
                qDebug() << "error ....2";
                qDebug() << inFile.errorString().toLocal8Bit().constData();
                return false;
            }

            if(!outFile.open(QIODevice::WriteOnly , QuaZipNewInfo(inFileTemp.fileName(), inFile.fileName())))
            {
                qDebug() << "error ...3";
                return false;
            }

            outFile.write(inFile.readAll());

            if(outFile.getZipError() != UNZ_OK)
            {
                qDebug() << "error ...4";
                return false;
            }

            outFile.close();

            if(outFile.getZipError() != UNZ_OK)
            {
                qDebug() << "error ....5";
                return false;
            }

            inFile.close();
        }
    }
    zip.close();

    if(zip.getZipError())
    {
        qDebug() << "error ...6";
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    return a.exec();
}
