#ifndef TDIALOGLOGIN_H
#define TDIALOGLOGIN_H

#include <QDialog>

#include<QtSql/QSqlDatabase>


namespace Ui {
class TDialogLogin;
}

class TDialogLogin : public QDialog
{
    Q_OBJECT

public:
    explicit TDialogLogin(QWidget *parent = nullptr);
    ~TDialogLogin();

private slots:
    void on_login_button_clicked();

private:
    Ui::TDialogLogin *ui;
    QSqlDatabase DB;
};

#endif // TDIALOGLOGIN_H
