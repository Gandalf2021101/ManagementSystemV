#include<QSqlQuery>
#include "stuwindow.h"
#include "ui_stuwindow.h"
#include<QSqlError>
#include<QMessageBox>
#include<QSqlRecord>
#include<QSqlTableModel>
#include<QFile>
#include<QFileDialog>
#include<QtSql/QSqlRelationalDelegate>
#include<QSqlIndex>
extern QString ID;

StuWindow::StuWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StuWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setStyleSheet("background-color: rgba(255,246,143,100);");
    ui->textEdit_i->setReadOnly(true);
    ui->tableView_g->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableView_i->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView_i->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_i->setAlternatingRowColors(true);

    ui->tableView_g->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView_g->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_g->setAlternatingRowColors(true);

    openTable();

}

StuWindow::~StuWindow()
{
    delete ui;
}

void printTableModel(QAbstractTableModel *model)
{
    const int rowCount = model->rowCount();
    const int columnCount = model->columnCount();

    // 打印表头
    for (int column = 0; column < columnCount; ++column) {
        QVariant headerData = model->headerData(column, Qt::Horizontal);
        qDebug() << headerData.toString();
    }

    // 打印表格内容
    for (int row = 0; row < rowCount; ++row) {
        for (int column = 0; column < columnCount; ++column) {
            QModelIndex index = model->index(row, column);
            QVariant data = model->data(index);
            qDebug() << data.toString();
        }
    }
}

void StuWindow::openTable()
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
        qDebug()<<"okk";
    }

    tabModel_i=new QSqlRelationalTableModel(this,DB);
    tabModel_i->setTable("inform");
    tabModel_i->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tabModel_i->setSort(tabModel_i->fieldIndex("informID"),Qt::AscendingOrder);
    if(!tabModel_i->select()){
        QMessageBox::critical(this, "错误信息",
                              "打开数据表错误,错误信息:\n"+tabModel_i->lastError().text());
                                  return;
    }

    tabModel_i->setHeaderData(tabModel_i->fieldIndex("informID"),Qt::Horizontal,"通知编号");
    tabModel_i->setHeaderData(tabModel_i->fieldIndex("releaseDate"),Qt::Horizontal,"发布时间");
    tabModel_i->setHeaderData(tabModel_i->fieldIndex("title"),Qt::Horizontal,"通知标题");
    tabModel_i->setHeaderData(tabModel_i->fieldIndex("content"),Qt::Horizontal,"通知详情");


    selModel_i=new QItemSelectionModel(tabModel_i,this);
    connect(selModel_i,&QItemSelectionModel::currentChanged,
            this,&StuWindow::do_currentChanged);
    //当前行变化时发射currentRowChanged()信号
    connect(selModel_i,&QItemSelectionModel::currentRowChanged,
            this,&StuWindow::do_currentRowChanged);



    ui->tableView_i->setModel(tabModel_i);
    ui->tableView_i->setSelectionModel(selModel_i);
    ui->tableView_i->setColumnHidden(tabModel_i->fieldIndex("content"),true);

    dataMapper_i=new QDataWidgetMapper(this);
    dataMapper_i->setModel(tabModel_i);
    dataMapper_i->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    //dataMapper_->addMapping(ui->dbComboDep,tabModel->fieldIndex("Department"));
    dataMapper_i->addMapping(ui->textEdit_i,tabModel_i->fieldIndex("content"));
    dataMapper_i->toFirst();


    //-----
    tabModel_g=new QSqlRelationalTableModel(this,DB);
    tabModel_g->setTable("sc");
    tabModel_g->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tabModel_g->setSort(tabModel_g->fieldIndex("userID"),Qt::AscendingOrder);
    if(!tabModel_g->select()){
        QMessageBox::critical(this, "错误信息",
                              "打开数据表错误,错误信息:\n"+tabModel_g->lastError().text());
                                  return;
    }


    tabModel_g->setFilter("userID="+ID);

    tabModel_g->setHeaderData(tabModel_g->fieldIndex("userID"),Qt::Horizontal,"学生学号");
    tabModel_g->setHeaderData(tabModel_g->fieldIndex("courseID"),Qt::Horizontal,"科目");
    tabModel_g->setHeaderData(tabModel_g->fieldIndex("grade"),Qt::Horizontal,"成绩");
    //tabModel_g->setRelation(tabModel_g->fieldIndex("userID"),QSqlRelation("student","userID","userName"));
    tabModel_g->setRelation(tabModel_g->fieldIndex("courseid"),QSqlRelation("course","courseID","courseName"));

    selModel_g=new QItemSelectionModel(tabModel_g,this);
    connect(selModel_g,&QItemSelectionModel::currentChanged,
            this,&StuWindow::do_currentChanged);
    //当前行变化时发射currentRowChanged()信号
    connect(selModel_g,&QItemSelectionModel::currentRowChanged,
            this,&StuWindow::do_currentRowChanged);
    ui->tableView_g->setModel(tabModel_g);
    ui->tableView_g->setSelectionModel(selModel_g);
    ui->tableView_g->setItemDelegate(new QSqlRelationalDelegate(ui->tableView_g));
    tabModel_g->select();

    //--

    tabModel_c=new QSqlRelationalTableModel(this,DB);
    tabModel_c->setTable("course");
    tabModel_c->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tabModel_c->setSort(tabModel_c->fieldIndex("courseID"),Qt::AscendingOrder);
    if(!tabModel_c->select()){
        QMessageBox::critical(this, "错误信息",
                              "打开数据表错误,错误信息:\n"+tabModel_c->lastError().text());
                                  return;
    }

    tabModel_c->setHeaderData(tabModel_c->fieldIndex("courseID"),Qt::Horizontal,"课程号");
    tabModel_c->setHeaderData(tabModel_c->fieldIndex("courseName"),Qt::Horizontal,"课程名称");
    tabModel_c->setHeaderData(tabModel_c->fieldIndex("time"),Qt::Horizontal,"开课学期");
    tabModel_c->setHeaderData(tabModel_c->fieldIndex("hours"),Qt::Horizontal,"时间");
    tabModel_c->setHeaderData(tabModel_c->fieldIndex("credit"),Qt::Horizontal,"学分");
    tabModel_c->setHeaderData(tabModel_c->fieldIndex("cpno"),Qt::Horizontal,"先修课程");
    tabModel_c->setHeaderData(tabModel_c->fieldIndex("teacherID"),Qt::Horizontal,"任课老师");
    //tabModel_g->setRelation(tabModel_g->fieldIndex("userID"),QSqlRelation("student","userID","userName"));
    tabModel_c->setRelation(tabModel_c->fieldIndex("teacherID"),QSqlRelation("teacher","teacherID","teacherName"));


    selModel_c=new QItemSelectionModel(tabModel_c,this);
    connect(selModel_c,&QItemSelectionModel::currentChanged,
            this,&StuWindow::do_currentChanged);
    //当前行变化时发射currentRowChanged()信号
    connect(selModel_c,&QItemSelectionModel::currentRowChanged,
            this,&StuWindow::do_currentRowChanged);

    ui->tableView_c->setModel(tabModel_c);
    ui->tableView_c->setSelectionModel(selModel_c);
    ui->tableView_c->setItemDelegate(new QSqlRelationalDelegate(ui->tableView_c));


    tabModel_c->select();

    //-----
    tabModel_g_2=new QSqlRelationalTableModel(this,DB);
    tabModel_g_2->setTable("sc");
    tabModel_g_2->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tabModel_g_2->setSort(tabModel_g_2->fieldIndex("userID"),Qt::AscendingOrder);
    if(!tabModel_g_2->select()){
        QMessageBox::critical(this, "错误信息",
                              "打开数据表错误,错误信息:\n"+tabModel_g->lastError().text());
                                  return;
    }

    tabModel_g_2->setFilter("userID="+ID);
    tabModel_g_2->setHeaderData(tabModel_g_2->fieldIndex("userID"),Qt::Horizontal,"学生学号");
    tabModel_g_2->setHeaderData(tabModel_g_2->fieldIndex("courseid"),Qt::Horizontal,"科目");
    tabModel_g_2->setHeaderData(tabModel_g_2->fieldIndex("grade"),Qt::Horizontal,"成绩");
    //tabModel_g->setRelation(tabModel_g->fieldIndex("userID"),QSqlRelation("student","userID","userName"));
    tabModel_g_2->setRelation(tabModel_g_2->fieldIndex("courseid"),QSqlRelation("course","courseID","courseName"));
    selModel_g_2=new QItemSelectionModel(tabModel_g_2,this);
    connect(selModel_g_2,&QItemSelectionModel::currentChanged,
            this,&StuWindow::do_currentChanged);
    //当前行变化时发射currentRowChanged()信号
    connect(selModel_g_2,&QItemSelectionModel::currentRowChanged,
            this,&StuWindow::do_currentRowChanged);
    ui->tableView_g_2->setModel(tabModel_g_2);
    ui->tableView_g_2->setSelectionModel(selModel_g_2);
    ui->tableView_g_2->setItemDelegate(new QSqlRelationalDelegate(ui->tableView_g_2));
    ui->tableView_g_2->setColumnHidden(tabModel_g_2->fieldIndex("grade"),true);
    tabModel_g_2->select();

}





void StuWindow::showRecordCount()
{

}

void StuWindow::do_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
}

void StuWindow::do_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    dataMapper_i->setCurrentIndex(current.row());
}

void StuWindow::on_pushButton_add_clicked()
{
    int row=selModel_c->currentIndex().row();
    QSqlRecord rec=tabModel_c->record(row);
    int selid=rec.value("courseID").toInt();

    QSqlQuery addQuery;
    addQuery.prepare("insert into sc values(?,?,null);");
    addQuery.bindValue(0, ID);
    addQuery.bindValue(1, selid);
    qDebug()<<ID<<"--"<<selid;
    //addQuery.next();//要调用！！！

    if(addQuery.exec()){
            QMessageBox::information(this,"提示","课程加入成功");
    }
    else{
            QMessageBox::information(this,"提示","课程已加入，请勿重复添加");
    }

     tabModel_g_2->select();

}


void StuWindow::on_pushButton_exit_clicked()
{
     int row=selModel_c->currentIndex().row();
     QSqlRecord rec=tabModel_c->record(row);
     int selid=rec.value("courseID").toInt();

     QSqlQuery addQuery;
     addQuery.prepare("delete from sc where userID=? and courseid=?;");
     addQuery.bindValue(0, ID);
     addQuery.bindValue(1, selid);
     qDebug()<<ID<<"--"<<selid;
     //addQuery.next();//要调用！！！

     if(addQuery.exec()){
            QMessageBox::information(this,"提示","课程退出成功");
     }

     tabModel_g_2->select();

}

