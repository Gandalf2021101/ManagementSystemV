#include "teawindow.h"
#include "ui_teawindow.h"
#include <qpixmap.h>
#include <QFile>
#include <QDebug>
#include<QMessageBox>
#include <QtSql/QSqlError>
#include <QSqlQuery>
#include <QThread>
#include <QTimer>
#include<QSqlError>
#include<QMessageBox>
#include<QSqlRecord>
#include<QSqlTableModel>
#include<QFileDialog>
#include<QtSql/QSqlRelationalDelegate>
extern QString ID;
QString tID;
QString cID;

TeaWindow::TeaWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TeaWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setStyleSheet("background-color: rgba(255,246,143,100);");
    ui->frame->setStyleSheet("background-color:white;");
    ui->lineEditpwd->setEchoMode(QLineEdit::Password);
    ui->lineEdit_pwdn->setEchoMode(QLineEdit::Password);
    ui->lineEdit_pwdn2->setEchoMode(QLineEdit::Password);

    ui->tableView_g->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView_g->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_g->setAlternatingRowColors(true);
    ui->lineEdit_g->setReadOnly(true);

    openTable();

}

TeaWindow::~TeaWindow()
{
    delete ui;
}

void TeaWindow::do_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
    ui->pushButton_submit->setEnabled(tabModel_g->isDirty());
    ui->pushButton_revert->setEnabled(tabModel_g->isDirty());
}

void TeaWindow::do_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
}

void TeaWindow::on_pushButtonpwd_clicked()
{
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
    QString uname=ID;
    QString pwd=ui->lineEditpwd->text();

    myQuery.prepare("SELECT * FROM user WHERE uname=? AND upswd=? AND type=?;");
    myQuery.bindValue(0, uname);
    myQuery.bindValue(1, pwd);
    myQuery.bindValue(2, 2);
    myQuery.exec();
    //DB.close();  //不可提前关闭
    if(myQuery.next()){

        if(ui->lineEdit_pwdn->text().contains(' ')){
            QMessageBox::information(this,"提示","新密码只能由字母，数字，下划线组成");
            ui->lineEdit_pwdn->clear();
            ui->lineEdit_pwdn2->clear();
            ui->lineEdit_pwdn->setFocus();

        }
        else if(ui->lineEdit_pwdn->text().size()<6){
            QMessageBox::information(this,"提示","新密码应大于等于6位");
            ui->lineEdit_pwdn->clear();
            ui->lineEdit_pwdn2->clear();
            ui->lineEdit_pwdn->setFocus();
        }
        else if(ui->lineEdit_pwdn->text()!=ui->lineEdit_pwdn2->text()){
            QMessageBox::information(this,"提示","两次输入密码不一致");
            ui->lineEdit_pwdn2->clear();
            ui->lineEdit_pwdn2->setFocus();

        }

        else{
    myQuery.prepare("update user set upswd=? WHERE uname=?;");
    myQuery.bindValue(1, pwd);
    myQuery.bindValue(0, uname);
    if(myQuery.exec()){
      QMessageBox::information(this,"提示","密码修改成功");
    }
    else{
      QMessageBox::critical(this,"报错",DB.lastError().text());
    }

        }

    }
    else{
            QMessageBox::information(this,"提示","请重新输入前密码");
            ui->lineEditpwd->clear();
            ui->lineEdit_pwdn->clear();
            ui->lineEdit_pwdn2->clear();
            ui->lineEditpwd->setFocus();
    }

}

void TeaWindow::openTable()
{
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
        //qDebug()<<"okk";
    }

    //----
    tabModel_g=new QSqlRelationalTableModel(this,DB);
    tabModel_g->setTable("sc");
    tabModel_g->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tabModel_g->setSort(tabModel_g->fieldIndex("userID"),Qt::AscendingOrder);
    if(!tabModel_g->select()){
        QMessageBox::critical(this, "错误信息",
                              "打开数据表错误,错误信息:\n"+tabModel_g->lastError().text());
                                  return;
    }


    //tabModel_g->setFilter("userID="+ID);
    getID();
    QSqlQuery myQuery;
    myQuery.prepare("SELECT courseName FROM course WHERE teacherID=?;");
    myQuery.bindValue(0, tID);
    qDebug()<<"==="<<tID;
    myQuery.exec();
    myQuery.next();
    QString cName=myQuery.value(0).toString();


    tabModel_g->setFilter("courseid='"+cID+"'");

    qDebug()<<"courseName='"+cName+"'";
    tabModel_g->setHeaderData(tabModel_g->fieldIndex("userID"),Qt::Horizontal,"学生学号");
    tabModel_g->setHeaderData(tabModel_g->fieldIndex("courseid"),Qt::Horizontal,"科目编号");
    tabModel_g->setHeaderData(tabModel_g->fieldIndex("grade"),Qt::Horizontal,"录入成绩");
    ui->lineEdit_g->setText(cName);
    //tabModel_g->setRelation(tabModel_g->fieldIndex("userID"),QSqlRelation("student","userID","userName"));

    //有此做不到
    //tabModel_g->setRelation(tabModel_g->fieldIndex("courseid"),QSqlRelation("course","courseID","courseName"));

    selModel_g=new QItemSelectionModel(tabModel_g,this);
    connect(selModel_g,&QItemSelectionModel::currentChanged,
            this,&TeaWindow::do_currentChanged);
    //当前行变化时发射currentRowChanged()信号
    connect(selModel_g,&QItemSelectionModel::currentRowChanged,
            this,&TeaWindow::do_currentRowChanged);

    ui->tableView_g->setModel(tabModel_g);
    ui->tableView_g->setSelectionModel(selModel_g);
    //ui->tableView_g->setItemDelegate(new QSqlRelationalDelegate(ui->tableView_g));


//    dataMapper_g=new QDataWidgetMapper(this);
//    dataMapper_g->setModel(tabModel_g);
//    dataMapper_g->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
//    dataMapper_g->toFirst();


    tabModel_g->select();

    //--教授课程

    tabModel_c=new QSqlRelationalTableModel(this,DB);
    tabModel_c->setTable("course");
    tabModel_c->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tabModel_c->setSort(tabModel_c->fieldIndex("courseID"),Qt::AscendingOrder);
    if(!tabModel_c->select()){
        QMessageBox::critical(this, "错误信息",
                              "打开数据表错误,错误信息:\n"+tabModel_c->lastError().text());
                                  return;
    }

    tabModel_c->setFilter("teacherID="+tID);
    tabModel_c->setHeaderData(tabModel_c->fieldIndex("courseName"),Qt::Horizontal,"课程名称");
    tabModel_c->setHeaderData(tabModel_c->fieldIndex("time"),Qt::Horizontal,"开课学期");
    tabModel_c->setHeaderData(tabModel_c->fieldIndex("hours"),Qt::Horizontal,"时间");
    tabModel_c->setHeaderData(tabModel_c->fieldIndex("credit"),Qt::Horizontal,"学分");
    tabModel_c->setHeaderData(tabModel_c->fieldIndex("cpno"),Qt::Horizontal,"先修课程");

    //tabModel_g->setRelation(tabModel_g->fieldIndex("userID"),QSqlRelation("student","userID","userName"));
    //tabModel_c->setRelation(tabModel_c->fieldIndex("teacherID"),QSqlRelation("teacher","teacherID","teacherName"));


    selModel_c=new QItemSelectionModel(tabModel_c,this);
    connect(selModel_c,&QItemSelectionModel::currentChanged,
            this,&TeaWindow::do_currentChanged);
    //当前行变化时发射currentRowChanged()信号
    connect(selModel_c,&QItemSelectionModel::currentRowChanged,
            this,&TeaWindow::do_currentRowChanged);

    ui->tableView_c->setModel(tabModel_c);
    ui->tableView_c->setSelectionModel(selModel_c);
    ui->tableView_c->setItemDelegate(new QSqlRelationalDelegate(ui->tableView_c));
    //ui->tableView_c->setColumnHidden(tabModel_c->fieldIndex("teacherID"),true);

    tabModel_c->select();

    //--学生
    tabModel_s=new QSqlRelationalTableModel(this,DB);
    tabModel_s->setTable("sc");
    tabModel_s->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tabModel_s->setSort(tabModel_s->fieldIndex("userID"),Qt::AscendingOrder);
    if(!tabModel_s->select()){
        QMessageBox::critical(this, "错误信息",
                              "打开数据表错误,错误信息:\n"+tabModel_c->lastError().text());
                                  return;
    }


    QString cn=tabModel_c->data(tabModel_c->index(0,tabModel_c->fieldIndex("courseName"))).toString();
    tabModel_s->setFilter("courseName='"+cn+"'");
    qDebug()<<"cname="<<cn;
    tabModel_s->setHeaderData(tabModel_s->fieldIndex("courseid"),Qt::Horizontal,"所选择课程");
    tabModel_s->setHeaderData(tabModel_s->fieldIndex("userid"),Qt::Horizontal,"学生姓名");
    tabModel_s->setRelation(tabModel_s->fieldIndex("userID"),QSqlRelation("student","userID","userName"));
    tabModel_s->setRelation(tabModel_s->fieldIndex("courseid"),QSqlRelation("course","courseID","courseName"));




    selModel_s=new QItemSelectionModel(tabModel_s,this);
    connect(selModel_s,&QItemSelectionModel::currentChanged,
            this,&TeaWindow::do_currentChanged);
    //当前行变化时发射currentRowChanged()信号
    connect(selModel_s,&QItemSelectionModel::currentRowChanged,
            this,&TeaWindow::do_currentRowChanged);

    ui->tableView_s->setModel(tabModel_s);
    ui->tableView_s->setSelectionModel(selModel_s);
    ui->tableView_s->setItemDelegate(new QSqlRelationalDelegate(ui->tableView_s));
    ui->tableView_s->setColumnHidden(tabModel_s->fieldIndex("grade"),true);

    tabModel_s->select();



}

void TeaWindow::getID()
{
    QSqlQuery tQuery;
    tQuery.prepare("SELECT teacherID FROM teacher WHERE employeeID=?;");
    tQuery.bindValue(0, ID);
    tQuery.exec();
    tQuery.next();//要调用！！！
    tID=tQuery.value(0).toString();

    QSqlQuery cQuery;
    cQuery.prepare("SELECT courseID FROM course WHERE teacherID=?;");
    cQuery.bindValue(0, tID);
    cQuery.exec();
    cQuery.next();//要调用！！！
    cID=cQuery.value(0).toString();
    qDebug()<<"******"<<cID;


}





void TeaWindow::on_pushButton_submit_clicked()
{
    bool flag=tabModel_g->submitAll();
    if(!flag){
        QMessageBox::information(this,"消息","数据保存错误，错误信息：\t"+tabModel_g->lastError().text());
    }
    else{
        ui->pushButton_submit->setEnabled(false);
        ui->pushButton_revert->setEnabled(false);
    }
}


void TeaWindow::on_pushButton_revert_clicked()
{
    tabModel_g->revertAll();
    ui->pushButton_revert->setEnabled(false);
    ui->pushButton_submit->setEnabled(false);
}

