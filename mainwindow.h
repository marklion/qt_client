#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Login_but_clicked();

    void on_exit_but_clicked();
    void on_Networkreply();
    void on_LoginReply();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager oQAM;
    QNetworkReply *pxReply;
    QStringList m_port_list;
};

#endif // MAINWINDOW_H
