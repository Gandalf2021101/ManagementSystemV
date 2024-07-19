#include <QMainWindow>
#include<QtSql/QSqlDatabase>
//#include<QSqlTableModel>
#include<QItemSelectionModel>
#include<QDataWidgetMapper>
#include "tcomboboxdelegate.h"
#include<QtSql/QSqlRelationalTableModel>


namespace Ui {
class StuWindow;
}

class StuWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StuWindow(QWidget *parent = nullptr);
    ~StuWindow();
    void do_currentChanged(const QModelIndex &current, const QModelIndex &previous);
    void do_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_exit_clicked();

private:
    QSqlDatabase DB;
    Ui::StuWindow *ui;
    QSqlRelationalTableModel*tabModel_i;
    QItemSelectionModel*selModel_i;
    QDataWidgetMapper*dataMapper_i;

    QSqlRelationalTableModel*tabModel_g;
    QItemSelectionModel*selModel_g;
    QDataWidgetMapper*dataMapper_g;

    QSqlRelationalTableModel*tabModel_g_2;
    QItemSelectionModel*selModel_g_2;
    QDataWidgetMapper*dataMapper_g_2;

    QSqlRelationalTableModel*tabModel_c;
    QItemSelectionModel*selModel_c;
    QDataWidgetMapper*dataMapper_c;
    //自定义代理
    TComboBoxDelegate delegateSex;
    TComboBoxDelegate delegateDepart;
    void openTable();
    void showRecordCount();
};


