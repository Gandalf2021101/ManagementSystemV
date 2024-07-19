#include "mainwindow.h"
#include "adminwindow.h"
#include "stuwindow.h"
#include "teawindow.h"

#include<tdialoglogin.h>
#include <QApplication>
#include <QDialog>
#include <QFile>
#include <QDebug>
#include <QString>


extern int userRight;
extern QString ID;
int main(int argc, char *argv[])
{



    QApplication a(argc,argv);
    TDialogLogin *td=new TDialogLogin();

    QFile qssFile(":/style.qss");//1.
    if(qssFile.open(QFile::ReadOnly)){//2.
        a.setStyleSheet(qssFile.readAll());//3.
        printf("suc");
    }

    qssFile.close();//4.


//    StuWindow s;
//    s.show();
//    return a.exec();





    if(td->exec()==QDialog::Accepted){

        if(userRight==1){
            adminWindow w;
            w.show();
            return a.exec();
        }
        else if(userRight==2){
            TeaWindow t;
            t.show();
            return a.exec();
        }
        else{
            StuWindow s;
            s.show();
            return a.exec();
        }
    }
    else
        return 0;

}


////在main中调用Slash
//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    TDialogLogin *td=new TDialogLogin();
//    if(td->exec()==QDialog::Accepted){
//        MainWindow w;
//        w.show();
//        return a.exec();
//    }
//    else
//        return 0;

//}
