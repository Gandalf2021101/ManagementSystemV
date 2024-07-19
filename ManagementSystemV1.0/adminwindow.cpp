#include "adminwindow.h"
#include "ui_adminwindow.h"
#include<QSqlError>
#include<QMessageBox>
#include<QSqlRecord>
#include<QSqlTableModel>
#include<QFile>
#include<QFileDialog>
#include<QtSql/QSqlRelationalDelegate>


adminWindow::adminWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::adminWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setStyleSheet("background-color: rgba(255,246,143,100);");
    ui->groupBox_2->setStyleSheet("background-color: white;");
    ui->groupBox_3->setStyleSheet("background-color: white;");
    ui->groupBox_4->setStyleSheet("background-color: white;");
    ui->groupBox_16->setStyleSheet("background-color: white;");
    ui->groupBox_17->setStyleSheet("background-color: white;");
    ui->groupBox_18->setStyleSheet("background-color: white;");
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);

    ui->tableView_3->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView_3->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_3->setAlternatingRowColors(true);

    ui->tableView_4->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView_4->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_4->setAlternatingRowColors(true);

    ui->tableView_5->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView_5->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_5->setAlternatingRowColors(true);


    ui->plainTextEdit->setStyleSheet("background-color: white;border-radius: 10px;border: 1px solid  white;");



    openTable();
}

adminWindow::~adminWindow()
{
    delete ui;
}

void adminWindow::openTable()
{
    //在这里完成各种初始化

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

    //一些组件初始化
    QSqlRelationalTableModel* tmptableModel=new QSqlRelationalTableModel(this,DB);
    tmptableModel->setTable("class");
    tmptableModel->select();
    for(int i=0;i<tmptableModel->rowCount();i++){
        QModelIndex index=tmptableModel->index(i,1);
        QString className=tmptableModel->data(index).toString();
        ui->dbspinBoxClass->addItem(className);
    }
    tmptableModel->setTable("major");
    tmptableModel->select();
    for(int i=0;i<tmptableModel->rowCount();i++){
        QModelIndex index=tmptableModel->index(i,tmptableModel->fieldIndex("majorName"));
        QString mName=tmptableModel->data(index).toString();
        ui->dbComboDep->addItem(mName);
        ui->dbComboDep_3->addItem(mName);
    }





    tabModel=new QSqlRelationalTableModel(this,DB);
    tabModel->setTable("student");
    tabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tabModel->setSort(tabModel->fieldIndex("userID"),Qt::AscendingOrder);

    if(!tabModel->select()){
        QMessageBox::critical(this, "错误信息",
                              "打开数据表错误,错误信息:\n"+tabModel->lastError().text());
                                  return;
    }

    tabModel->setHeaderData(tabModel->fieldIndex("userID"),Qt::Horizontal,"学号");
    tabModel->setHeaderData(tabModel->fieldIndex("userName"),Qt::Horizontal,"姓名");
    tabModel->setHeaderData(tabModel->fieldIndex("userSex"),Qt::Horizontal,"性别");
    tabModel->setHeaderData(tabModel->fieldIndex("markYear"),Qt::Horizontal,"入学日期");
    tabModel->setHeaderData(tabModel->fieldIndex("classID"),Qt::Horizontal,"班级");
    tabModel->setHeaderData(tabModel->fieldIndex("photo"),Qt::Horizontal,"照片");
    tabModel->setHeaderData(tabModel->fieldIndex("userAge"),Qt::Horizontal,"年龄");
    tabModel->setRelation(tabModel->fieldIndex("classID"),QSqlRelation("class","classID","className"));
    tabModel->setRelation(tabModel->fieldIndex("majorid"),QSqlRelation("major","majorID","majorName"));

    tabModel_=new QSqlRelationalTableModel(this,DB);
    tabModel_->setTable("teacher");
    tabModel_->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tabModel_->setSort(tabModel_->fieldIndex("teacherID"),Qt::AscendingOrder);
    if(!tabModel_->select()){
        QMessageBox::critical(this, "错误信息",
                              "打开数据表错误,错误信息:\n"+tabModel->lastError().text());
                                  return;
    }

    tabModel_->setHeaderData(tabModel_->fieldIndex("employeeID"),Qt::Horizontal,"职工号");
    tabModel_->setHeaderData(tabModel_->fieldIndex("teacherName"),Qt::Horizontal,"姓名");
    tabModel_->setHeaderData(tabModel_->fieldIndex("teachSex"),Qt::Horizontal,"性别");
    tabModel_->setHeaderData(tabModel_->fieldIndex("teacherAge"),Qt::Horizontal,"年龄");
    tabModel_->setHeaderData(tabModel_->fieldIndex("teachYear"),Qt::Horizontal,"入职日期");
    tabModel_->setHeaderData(tabModel_->fieldIndex("photo"),Qt::Horizontal,"照片");


    tabModel_c=new QSqlRelationalTableModel(this,DB);
    tabModel_c->setTable("course");
    tabModel_c->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tabModel_c->setSort(tabModel_c->fieldIndex("courseID"),Qt::AscendingOrder);
    if(!tabModel_c->select()){
        QMessageBox::critical(this, "错误信息",
                              "打开数据表错误,错误信息:\n"+tabModel->lastError().text());
                                  return;
    }

    tabModel_c->setHeaderData(tabModel_c->fieldIndex("courseID"),Qt::Horizontal,"课程号");
    tabModel_c->setHeaderData(tabModel_c->fieldIndex("courseName"),Qt::Horizontal,"课程名");
    tabModel_c->setHeaderData(tabModel_c->fieldIndex("time"),Qt::Horizontal,"开课时间");
    tabModel_c->setHeaderData(tabModel_c->fieldIndex("hours"),Qt::Horizontal,"课时");
    tabModel_c->setHeaderData(tabModel_c->fieldIndex("credit"),Qt::Horizontal,"学分");
    tabModel_c->setHeaderData(tabModel_c->fieldIndex("cpno"),Qt::Horizontal,"先修课程");

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




    showRecordCount();
    //设置selecmodel
    selModel=new QItemSelectionModel(tabModel,this);
    selModel_=new QItemSelectionModel(tabModel_,this);
    selModel_c=new QItemSelectionModel(tabModel_c,this);
    selModel_i=new QItemSelectionModel(tabModel_i,this);


    // 2.连接槽函数用引用，且不用参数和括号
    //当前行或列变化时发射currentChanged信号
    connect(selModel,&QItemSelectionModel::currentChanged,
            this,&adminWindow::do_currentChanged);
    //当前行变化时发射currentRowChanged()信号
    connect(selModel,&QItemSelectionModel::currentRowChanged,
            this,&adminWindow::do_currentRowChanged);

    connect(selModel_,&QItemSelectionModel::currentChanged,
            this,&adminWindow::do_currentChanged);
    //当前行变化时发射currentRowChanged()信号
    connect(selModel_,&QItemSelectionModel::currentRowChanged,
            this,&adminWindow::do_currentRowChanged);

    connect(selModel_i,&QItemSelectionModel::currentChanged,
            this,&adminWindow::do_currentChanged);
    //当前行变化时发射currentRowChanged()信号
    connect(selModel_i,&QItemSelectionModel::currentRowChanged,
            this,&adminWindow::do_currentRowChanged);



    // 3.设置modelview
    ui->tableView->setModel(tabModel);
    ui->tableView->setSelectionModel(selModel);
    ui->tableView->setColumnHidden(tabModel->fieldIndex("photo"),true);
    ui->tableView->setColumnHidden(tabModel->fieldIndex("teacherID"),true);
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));

    //ui->tableView->setColumnHidden(tabModel->fieldIndex("Memo"),true);


    ui->tableView_3->setModel(tabModel_);
    ui->tableView_3->setSelectionModel(selModel_);
    ui->tableView_3->setColumnHidden(tabModel_->fieldIndex("photo"),true);
    ui->tableView_3->setColumnHidden(tabModel_->fieldIndex("employID"),true);

    ui->tableView_4->setModel(tabModel_i);
    ui->tableView_4->setSelectionModel(selModel_i);


    ui->tableView_5->setModel(tabModel_c);
    ui->tableView_5->setSelectionModel(selModel_c);
    ui->tableView_5->setColumnHidden(tabModel_c->fieldIndex("teacherID"),true);




    // 4.设置自定义代理

    QStringList strlist1;
    QStringList strlist2;
    strlist1<<"男"<<"女";
    strlist2<<"理论力学与应用"<<"航天航空工程"<<"智慧交通"<<"软件工程"<<"集成电路"<<"智能科学与技术"<<"智能科学与技术";
    delegateSex.iniCombo(strlist1,false);
    delegateDepart.iniCombo(strlist2,false);
    ui->tableView->setItemDelegateForColumn(tabModel->fieldIndex("userSex"),&delegateSex);
    ui->tableView->setItemDelegateForColumn(tabModel->fieldIndex("teachSex"),&delegateSex);
    ui->tableView->setItemDelegateForColumn(tabModel->fieldIndex("major"),&delegateDepart);




    // 5.设置mapper
    dataMapper=new QDataWidgetMapper(this);
    dataMapper->setModel(tabModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    //dataMapper->addMapping(ui->dbComboDep,tabModel->fieldIndex("Department"));
    dataMapper->addMapping(ui->dbComboSex,tabModel->fieldIndex("userSex"));
    dataMapper->addMapping(ui->dbregdate,tabModel->fieldIndex("markYear"));
    dataMapper->addMapping(ui->dbEditName,tabModel->fieldIndex("userName"));
    dataMapper->addMapping(ui->dbSpinEmpNo,tabModel->fieldIndex("userID"));
    dataMapper->addMapping(ui->dbspinBoxClass,tabModel->fieldIndex("classID"));
    dataMapper->addMapping(ui->plainTextEdit_3,tabModel->fieldIndex("memo"));
    dataMapper->addMapping(ui->dbComboDep,tabModel->fieldIndex("majorid"));
    dataMapper->toFirst();


    dataMapper_=new QDataWidgetMapper(this);
    dataMapper_->setModel(tabModel_);
    dataMapper_->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    //dataMapper_->addMapping(ui->dbComboDep,tabModel->fieldIndex("Department"));
    dataMapper_->addMapping(ui->dbComboSex_3,tabModel_->fieldIndex("teachSex"));
    dataMapper_->addMapping(ui->dbregdate_3,tabModel_->fieldIndex("teachYear"));
    dataMapper_->addMapping(ui->dbEditName_3,tabModel_->fieldIndex("teacherName"));
    dataMapper_->addMapping(ui->dbSpinEmpNo_3,tabModel_->fieldIndex("employeeID"));
    dataMapper_->addMapping(ui->dbspinBoxClass_3,tabModel_->fieldIndex("roomID"));
    dataMapper_->addMapping(ui->plainTextEdit_2,tabModel_->fieldIndex("memo"));

    dataMapper_->toFirst();


    dataMapper_i=new QDataWidgetMapper(this);
    dataMapper_i->setModel(tabModel_i);
    dataMapper_i->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    //dataMapper_->addMapping(ui->dbComboDep,tabModel->fieldIndex("Department"));
    dataMapper_i->addMapping(ui->plainTextEdit,tabModel_i->fieldIndex("content"));
    dataMapper_i->toFirst();






    // 6.获取字段名称列表，填充groupBoxSort
    getFeildNames();

    //7.更新action和界面组件的使能状态
    tabModel->select();
    ui->actRecInsert->setEnabled(true);
    ui->actRecInsert->setEnabled(true);
    ui->actionactRecDelete->setEnabled(true);
    //ui->actPhoto->setEnabled(true);
    //ui->actPhotoClear->setEnabled(true);
    //ui->actSubmit->click();
    ui->actRevert->setEnabled(false);
    ui->actSubmit->setEnabled(false);
    ui->actQuit->setEnabled(true);

    ui->actRecInsert_3->setEnabled(true);
    ui->actRecInsert_3->setEnabled(true);
    ui->actionactRecDelete_3->setEnabled(true);
    ui->actRevert_3->setEnabled(true);
    ui->actSubmit_3->setEnabled(true);
    ui->actQuit_3->setEnabled(true);


    ui->actRecInsert_4->setEnabled(true);
    ui->actRecInsert_4->setEnabled(true);
    ui->actionactRecDelete_4->setEnabled(true);
    ui->actRevert_4->setEnabled(true);
    ui->actSubmit_4->setEnabled(true);
    ui->actQuit_4->setEnabled(true);


    ui->actionactRecDelete_5->setEnabled(true);
    ui->actRevert_5->setEnabled(true);
    ui->actSubmit_5->setEnabled(true);
    ui->actQuit_5->setEnabled(true);


}

void adminWindow::getFeildNames()
{
    QSqlRecord Rec=tabModel->record();
    for(int i=0;i<Rec.count();i++){
                                  ui->comboBox->addItem(Rec.fieldName(i));
    }

    QSqlRecord Rec_=tabModel_->record();
    for(int i=0;i<Rec_.count();i++){
                                  ui->comboBox_3->addItem(Rec_.fieldName(i));
    }


}

void adminWindow::showRecordCount()
{
    ui->statusbar->showMessage(QString::asprintf("记录条数：%1").arg(tabModel->rowCount()));
}

void adminWindow::do_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
    // 有未更新到数据库的修改时可以用这两个东西，所以这个与selmodel关联？
    ui->actSubmit->setEnabled(tabModel->isDirty());
    ui->actRevert->setEnabled(tabModel->isDirty());

    ui->actSubmit_3->setEnabled(tabModel_->isDirty());
    ui->actRevert_3->setEnabled(tabModel_->isDirty());

    ui->actSubmit_5->setEnabled(tabModel_i->isDirty());
    ui->actRevert_5->setEnabled(tabModel_i->isDirty());
    //ui->actSubmit_4->setEnabled(tabModel_->isDirty());
    //ui->actRevert_4->setEnabled(tabModel_->isDirty());

    //ui->actSubmit_5->setEnabled(tabModel_->isDirty());
    //ui->actRevert_5->setEnabled(tabModel_->isDirty());

}

void adminWindow::do_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    // 行切换时的状态控制
    ui->actionactRecDelete->setEnabled(current.isValid());
    ui->actPhoto->setEnabled(current.isValid());
    ui->actPhotoClear->setEnabled(current.isValid());
    if(!current.isValid()){
                                  //没有任何选中（或索引无效），则不显示任何照片,并结束
                                  ui->dbLabPhoto->clear();
                                  return;
    }
    dataMapper->setCurrentIndex(current.row());
    int row=current.row();
    QSqlRecord recd=tabModel->record(row);
    QByteArray data=recd.value("photo").toByteArray();
    QPixmap pic;
    pic.loadFromData(data);
    ui->dbLabPhoto->setPixmap(pic.scaledToWidth(ui->dbLabPhoto->width()));


    ui->actionactRecDelete_3->setEnabled(current.isValid());
    ui->actPhoto_3->setEnabled(current.isValid());
    ui->actPhotoClear_3->setEnabled(current.isValid());
    if(!current.isValid()){
                                  //没有任何选中（或索引无效），则不显示任何照片,并结束
                                  ui->dbLabPhoto->clear();
                                  return;
    }
    dataMapper_->setCurrentIndex(current.row());
    int row_3=current.row();
    QSqlRecord recd_3=tabModel_->record(row_3);
    QByteArray data_3=recd_3.value("photo").toByteArray();
    QPixmap pic_3;
    pic_3.loadFromData(data_3);
    ui->dbLabPhoto_3->setPixmap(pic_3.scaledToWidth(ui->dbLabPhoto_3->width()));


    dataMapper_i->setCurrentIndex(current.row());

}


void adminWindow::on_actOpenRecAppend_clicked()
{
    QSqlRecord rec=tabModel->record();
    rec.setValue(tabModel->fieldIndex("userID"),300000000+tabModel->rowCount());
    rec.setValue(tabModel->fieldIndex("userSex"),"男");
                                                 tabModel->insertRecord(tabModel->rowCount(),rec);
                 selModel->clearCurrentIndex();
                 QModelIndex curIndex=tabModel->index(tabModel->rowCount()-1,1);
                 selModel->setCurrentIndex(curIndex,QItemSelectionModel::Select);
                 showRecordCount();
}


void adminWindow::on_actRecInsert_clicked()
{
                 QSqlRecord rec=tabModel->record();
                 rec.setValue(tabModel->fieldIndex("userID"),300000000+tabModel->rowCount());
                 rec.setValue(tabModel->fieldIndex("userSex"),"男");
                                                              QModelIndex curIndex=selModel->currentIndex();
                              tabModel->insertRecord(curIndex.row(),rec);
                              selModel->clearCurrentIndex();
                              curIndex=tabModel->index(curIndex.row()+1,0);
                              selModel->setCurrentIndex(curIndex,QItemSelectionModel::Select);
                              rec=tabModel->record(curIndex.row());
                              showRecordCount();
}


void adminWindow::on_actionactRecDelete_clicked()
{    QModelIndex index=selModel->currentIndex();
     qDebug()<<index.row();
     tabModel->removeRow(index.row());
     showRecordCount();
     ui->actSubmit->setEnabled(true);
}


void adminWindow::on_actSubmit_clicked()
{
    bool flag=tabModel->submitAll();
    if(!flag){
        QMessageBox::information(this,"消息","数据保存错误，错误信息：\t"+tabModel->lastError().text());
    }
    else{
        ui->actSubmit->setEnabled(false);
        ui->actRevert->setEnabled(false);
    }
    showRecordCount();
}


void adminWindow::on_actRevert_clicked()
{
    tabModel->revertAll();
    ui->actRevert->setEnabled(false);
    ui->actSubmit->setEnabled(false);
    showRecordCount();
}


void adminWindow::on_actQuit_clicked()
{
    close();
}


void adminWindow::on_actPhoto_clicked()
{
    QString aFile=QFileDialog::getOpenFileName(this,"设置照片","","照片(*.jpg)");
    if(aFile.isEmpty()){
        return;
    }
    QFile *file=new QFile(aFile);
    file->open(QIODevice::ReadOnly);
    QByteArray data=file->readAll();
    file->close();

    //得到data是为了修改数据库中数据，至于显示组件上的修改，用路径赋予pic加载即可
    int rowRec=selModel->currentIndex().row();
    QPixmap pic;
    pic.load(aFile);//load和loadfromData的区别？
    ui->dbLabPhoto->setPixmap(pic.scaledToWidth(ui->dbLabPhoto->width())); // !!!!!

    QSqlRecord rec=tabModel->record(rowRec);
    rec.setValue("photo",data);
    bool a=tabModel->setRecord(rowRec,rec);// !!!!!!!
    printf("------%d",a);

}






void adminWindow::on_actPhotoClear_clicked()
{
    int recRow=selModel->currentIndex().row();
    QSqlRecord rec=tabModel->record(recRow);
    rec.setNull("photo");
    //还需要将其加入tabmodel中,才真正的修改
    tabModel->setRecord(selModel->currentIndex().row(),rec);
    ui->dbLabPhoto->clear();
}


void adminWindow::on_radioButtonAsc_clicked()
{
    tabModel->setSort(ui->comboBox->currentIndex(),Qt::AscendingOrder);
    tabModel->select();
}


void adminWindow::on_radioButtonDec_clicked()
{
    tabModel->setSort(ui->comboBox->currentIndex(),Qt::DescendingOrder);
    tabModel->select();
}


void adminWindow::on_radioButton_man_clicked()
{
        tabModel->setFilter(" userSex='男' ");
}


void adminWindow::on_radioButton_woman_clicked()
{
        tabModel->setFilter(" userSex='女'");
}


void adminWindow::on_radioButtonAll_clicked()
{
        tabModel->setFilter("");
}


void adminWindow::on_actOpenRecAppend_3_clicked()
{

}


void adminWindow::on_actRecInsert_3_clicked()
{

}


void adminWindow::on_actionactRecDelete_3_clicked()
{

}


void adminWindow::on_actPhoto_3_clicked()
{

}


void adminWindow::on_actPhotoClear_3_clicked()
{

}


void adminWindow::on_actSubmit_3_clicked()
{

}


void adminWindow::on_actRevert_3_clicked()
{

}


void adminWindow::on_actQuit_3_clicked()
{
        close();
}


void adminWindow::on_actOpenRecAppend_4_clicked()
{
        QSqlRecord rec=tabModel_c->record();
        rec.setValue(tabModel_c->fieldIndex("courseID"),1+tabModel_c->rowCount());
        //rec.setValue(tabModel_c->fieldIndex("teachSex"),"男");///此处22.13
                                                      tabModel_c->insertRecord(tabModel_c->rowCount(),rec);
                     selModel_c->clearCurrentIndex();
                     QModelIndex curIndex=tabModel_c->index(tabModel_c->rowCount()-1,1);
                     selModel_c->setCurrentIndex(curIndex,QItemSelectionModel::Select);
                     showRecordCount();
}


void adminWindow::on_actRecInsert_4_clicked()
{
                     QSqlRecord rec=tabModel_c->record();
                     rec.setValue(tabModel_c->fieldIndex("courseID"),1+tabModel->rowCount());
                     //rec.setValue(tabModel_c->fieldIndex("userSex"),"男");
                     QModelIndex curIndex=selModel_c->currentIndex();
                                  tabModel_c->insertRecord(curIndex.row(),rec);
                                  selModel_c->clearCurrentIndex();
                                  curIndex=tabModel_c->index(curIndex.row()+1,0);
                                  selModel_c->setCurrentIndex(curIndex,QItemSelectionModel::Select);
                                  rec=tabModel_c->record(curIndex.row());
                                  showRecordCount();
}


void adminWindow::on_actionactRecDelete_4_clicked()
{
                                  QModelIndex index=selModel_c->currentIndex();
                                  qDebug()<<index.row();
                                  tabModel_c->removeRow(index.row());
                                  showRecordCount();
                                  ui->actSubmit_4->setEnabled(true);
}


void adminWindow::on_actSubmit_4_clicked()
{
    bool flag=tabModel_c->submitAll();
    if(!flag){
        QMessageBox::information(this,"消息","数据保存错误，错误信息：\t"+tabModel_c->lastError().text());
    }
    else{
        ui->actSubmit_4->setEnabled(false);
        ui->actRevert_4->setEnabled(false);
    }
    showRecordCount();
}


void adminWindow::on_actRevert_4_clicked()
{
    tabModel_c->revertAll();
    ui->actRevert_4->setEnabled(false);
    ui->actSubmit_4->setEnabled(false);
    showRecordCount();
}


void adminWindow::on_actQuit_4_clicked()
{
    close();
}


void adminWindow::on_actOpenRecAppend_5_clicked()
{
    QSqlRecord rec=tabModel_i->record();
    rec.setValue(tabModel_i->fieldIndex("informID"),1+tabModel_i->rowCount());
    //rec.setValue(tabModel_c->fieldIndex("teachSex"),"男");///此处22.13
    tabModel_i->insertRecord(tabModel_i->rowCount(),rec);
    selModel_i->clearCurrentIndex();
    QModelIndex curIndex=tabModel_i->index(tabModel_i->rowCount()-1,1);
    selModel_i->setCurrentIndex(curIndex,QItemSelectionModel::Select);
    showRecordCount();
}


void adminWindow::on_actionactRecDelete_5_clicked()
{
    QModelIndex index=selModel_i->currentIndex();
    qDebug()<<index.row();
    tabModel_i->removeRow(index.row());
    showRecordCount();
    ui->actSubmit_5->setEnabled(true);
}


void adminWindow::on_actSubmit_5_clicked()
{
    bool flag=tabModel_i->submitAll();
    if(!flag){
        QMessageBox::information(this,"消息","数据保存错误，错误信息：\t"+tabModel_i->lastError().text());
    }
    else{
        ui->actSubmit_5->setEnabled(false);
        ui->actRevert_5->setEnabled(false);
    }
    showRecordCount();
}


void adminWindow::on_actRevert_5_clicked()
{
    tabModel_i->revertAll();
    ui->actRevert_5->setEnabled(false);
    ui->actSubmit_5->setEnabled(false);
    showRecordCount();
}


void adminWindow::on_actQuit_5_clicked()
{
    close();
}

