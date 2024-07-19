#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include<QtSql/QSqlDatabase>
#include<QSqlTableModel>
#include<QItemSelectionModel>
#include<QDataWidgetMapper>
#include "tcomboboxdelegate.h"
#include<QtSql/QSqlRelationalTableModel>


namespace Ui {
class adminWindow;
}

class adminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit adminWindow(QWidget *parent = nullptr);
    ~adminWindow();

private:
    Ui::adminWindow *ui;
    QSqlDatabase DB;
    QSqlRelationalTableModel*tabModel;
    QItemSelectionModel*selModel;
    QDataWidgetMapper*dataMapper;

    QSqlDatabase DB_;
    QSqlRelationalTableModel*tabModel_;
    QItemSelectionModel*selModel_;
    QDataWidgetMapper*dataMapper_;

    QSqlDatabase DB_c;
    QSqlRelationalTableModel*tabModel_c;
    QItemSelectionModel*selModel_c;
    QDataWidgetMapper*dataMapper_c;

    QSqlDatabase DB_i;
    QSqlRelationalTableModel*tabModel_i;
    QItemSelectionModel*selModel_i;
    QDataWidgetMapper*dataMapper_i;



    //自定义代理
    TComboBoxDelegate delegateSex;
    TComboBoxDelegate delegateDepart;
    void openTable();
    void getFeildNames();
    void showRecordCount();
public:
    void do_currentChanged(const QModelIndex &current, const QModelIndex &previous);
    void do_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
private slots:
    void on_actOpenRecAppend_clicked();
    void on_actRecInsert_clicked();
    void on_actionactRecDelete_clicked();
    void on_actSubmit_clicked();
    void on_actRevert_clicked();
    void on_actQuit_clicked();
    void on_actPhoto_clicked();
    void on_actPhotoClear_clicked();
    void on_radioButtonAsc_clicked();
    void on_radioButtonDec_clicked();
    void on_radioButton_man_clicked();
    void on_radioButton_woman_clicked();
    void on_radioButtonAll_clicked();
    void on_actOpenRecAppend_3_clicked();
    void on_actRecInsert_3_clicked();
    void on_actionactRecDelete_3_clicked();
    void on_actPhoto_3_clicked();
    void on_actPhotoClear_3_clicked();
    void on_actSubmit_3_clicked();
    void on_actRevert_3_clicked();
    void on_actQuit_3_clicked();
    void on_actOpenRecAppend_4_clicked();
    void on_actRecInsert_4_clicked();
    void on_actionactRecDelete_4_clicked();
    void on_actSubmit_4_clicked();
    void on_actRevert_4_clicked();
    void on_actQuit_4_clicked();
    void on_actOpenRecAppend_5_clicked();
    void on_actionactRecDelete_5_clicked();
    void on_actSubmit_5_clicked();
    void on_actRevert_5_clicked();
    void on_actQuit_5_clicked();
};

#endif // ADMINWINDOW_H
