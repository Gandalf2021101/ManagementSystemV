
#include <QMainWindow>
#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlRelationalTableModel>
#include<QItemSelectionModel>
#include<QDataWidgetMapper>

namespace Ui {
class TeaWindow;
}

class TeaWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TeaWindow(QWidget *parent = nullptr);
    ~TeaWindow();
    void do_currentChanged(const QModelIndex &current, const QModelIndex &previous);
    void do_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);

private slots:
    void on_pushButtonpwd_clicked();

    void on_pushButton_submit_clicked();

    void on_pushButton_revert_clicked();

private:
    Ui::TeaWindow *ui;
    QSqlDatabase DB;
    QSqlRelationalTableModel*tabModel_g;
    QItemSelectionModel*selModel_g;
    QDataWidgetMapper*dataMapper_g;

    QSqlRelationalTableModel*tabModel_c;
    QItemSelectionModel*selModel_c;
    QDataWidgetMapper*dataMapper_c;

    QSqlRelationalTableModel*tabModel_s;
    QItemSelectionModel*selModel_s;
    QDataWidgetMapper*dataMapper_s;

    void openTable();
    void getID();
};


