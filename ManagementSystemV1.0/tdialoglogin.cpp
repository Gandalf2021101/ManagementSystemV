#include "tdialoglogin.h"
#include "ui_tdialoglogin.h"
#include <qpixmap.h>
#include <QFile>
#include <QDebug>
#include<QMessageBox>
#include <QtSql/QSqlError>
#include <QSqlQuery>
#include <QThread>
#include <QTimer>


int userRight=-1;
QString ID=0;

TDialogLogin::TDialogLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TDialogLogin)
{

    ui->setupUi(this);
    ui->lineEditName->setPlaceholderText("请输入用户名");
    ui->lineEditPwd->setEchoMode(QLineEdit::Password);
    ui->lineEditPwd->setPlaceholderText("请输入密码");
    ui->login_button->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                    "border-radius: 12px; }"
                                    "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                                    "QPushButton:pressed{background-color: rgb(17,171,164);}");//pressed
    ui->lineEditPwd->setStyleSheet("font: 25 14pt '微软雅黑 Light';" //字体
                                "color: rgb(31,31,31);"		//字体颜色
                                "padding-left:20px;"       //内边距-字体缩进
                                "background-color: rgb(255, 255, 255);" //背景颜色
                                "border:2px solid rgb(20,196,188);border-radius:15px;");//边框粗细-颜色-圆角设置
    ui->lineEditName->setStyleSheet("font: 25 14pt '微软雅黑 Light';" //字体
                                   "color: rgb(31,31,31);"		//字体颜色
                                   "padding-left:20px;"       //内边距-字体缩进
                                   "background-color: rgb(255, 255, 255);" //背景颜色
                                   "border:2px solid rgb(20,196,188);border-radius:15px;");//边框粗细-颜色-圆角设置
    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(251,102,102, 200), stop:1 rgba(20,196,188, 210));");
    ui->label_logo->setStyleSheet("image: url(:/new/prefix1/images/login.png);");
    this->setStyleSheet("background: url(:/new/prefix1/images/login.png);");
    ui->frameA->setStyleSheet("border:2px solid rgb(255,255,255);border-radius:15px;"
                              "background-color: rgb(255, 255, 255);");
    ui->radioButton_stu->setChecked(true);
    ui->label_tip->setStyleSheet("color: rgb(209,26,45);");

}

TDialogLogin::~TDialogLogin()
{
    delete ui;
}

void TDialogLogin::on_login_button_clicked()
{
    if(ui->lineEditName->text().trimmed()==""){
        ui->label_tip->setText("请输入用户名");
        ui->lineEditName->setFocus();
    }
    else if(ui->lineEditPwd->text().trimmed()==""){
        ui->label_tip->setText("请输入密码");
        ui->lineEditPwd->setFocus();
    }
    else{
    DB=QSqlDatabase::addDatabase("QODBC");
    DB.setDatabaseName("managesystem");//此处需要与ODBC中的数据源名称一致
    DB.setHostName("localhost");   //我连到了名字为main数据的库中，这个由odbc中的database:main决定
    DB.setPort(13306);
    DB.setUserName("root");
    DB.setPassword("271828");
    if(!DB.open()){
        QMessageBox::warning(this,"错啦！","数据库打开失败！:"+DB.lastError().text());
    }
    else{
        qDebug()<<"打开成功！";
    }
    QSqlQuery myQuery;
    QString uname=ui->lineEditName->text();
    QString pwd=ui->lineEditPwd->text();
    int type=3;
    if(ui->radioButton_stu->isChecked()){
        type=3;
        userRight=3;
    }
    else if(ui->radioButton_tea->isChecked()){
        type=2;
        userRight=2;
    }
    else
    {
        type=1;
        userRight=1;

    }
    myQuery.prepare("SELECT * FROM user WHERE uname=? AND upswd=? AND type=?;");
    myQuery.bindValue(0, uname);
    myQuery.bindValue(1, pwd);
    myQuery.bindValue(2, type);
    qDebug()<<myQuery.lastQuery();
    myQuery.exec();
    //DB.close();  //不可提前关闭
    if(myQuery.next()){
            ui->label_tip->setText("登录成功");
            ID=uname;
            DB.close();
            this->accept();


    }
    else{
            ui->label_tip->setText("登录失败，用户名或密码错误");
            ui->lineEditName->clear();
            ui->lineEditPwd->clear();
            ui->lineEditName->setFocus();
            DB.close();
    }
    }


}

