#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*连接服务器获取游戏服务器数据*/
    QNetworkRequest oReq(QUrl("http://192.168.64.140/info/"));
    pxReply = oQAM.get(oReq);
    connect(pxReply, SIGNAL(finished()), this, SLOT(on_Networkreply()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Login_but_clicked()
{
    /*读取用户输入*/
    QString szUsername = ui->username->text();
    QString szPassword = ui->password->text();

    /*组织json数据*/
    /*换算密码hash值*/
    QCryptographicHash hash_creator(QCryptographicHash::Md5);
    /*设置源数据，生成hash数据*/
    hash_creator.addData(szPassword.toUtf8());
    szPassword = QString(hash_creator.result().toHex()); //szPassword变成密码hash值了

    QJsonObject oJson;
    oJson["Username"] = szUsername;
    oJson["Password"] = szPassword;

    QJsonDocument oJsonDoc(oJson);
    QString szOut = oJsonDoc.toJson();

    /*post发送登陆请求*/
    QNetworkRequest oReq(QUrl("http://192.168.64.140/login/"));
    oReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    oReq.setHeader(QNetworkRequest::ContentLengthHeader, szOut.size());

    pxReply = oQAM.post(oReq, szOut.toUtf8());
    connect(pxReply, SIGNAL(finished()), this, SLOT(on_LoginReply()));
/*
    启动游戏进程，传递游戏服务器IP和端口号
    QStringList szArg;

    设置ip和端口号到szArg
    szArg.push_back("192.168.64.140");
    szArg.push_back(m_port_list[ ui->comboBox->currentIndex()]);

    QString szExePath = "client.exe";
    QProcess oP;
    oP.setProgram(szExePath);
    oP.setArguments(szArg);
    oP.startDetached();
    qApp->exit();
    */
}

void MainWindow::on_exit_but_clicked()
{
    QDesktopServices::openUrl(QUrl("http://192.168.64.140/reg.html"));
}

void MainWindow::on_Networkreply()
{
    QByteArray oInput = pxReply->readAll();
    QJsonDocument oJsonDoc = QJsonDocument::fromJson(oInput);
    QJsonArray oJsonArr = oJsonDoc.array();

    for (int i = 0; i < oJsonArr.size(); i++)
    {
        /*将读取到的服务器名，显示为下来菜单项*/
        ui->comboBox->addItem(oJsonArr[i].toObject()["Name"].toString(), i);
        /*将读到的端口号，存成向量*/
        m_port_list.push_back( oJsonArr[i].toObject()["Port"].toString());
    }
}

void MainWindow::on_LoginReply()
{
    /*接收登陆请求的回复*/
    QByteArray oInput = pxReply->readAll();
    QJsonDocument oJsonDoc = QJsonDocument::fromJson(oInput);
    /*从json对象中取出登陆结果*/
    QString szRes = oJsonDoc.object()["result"].toString();

    if ("yes" == szRes)
    {
        //启动游戏进程，传递游戏服务器IP和端口号
        QStringList szArg;

        //设置ip和端口号到szArg
        szArg.push_back("192.168.64.140");
        szArg.push_back(m_port_list[ ui->comboBox->currentIndex()]);

        QString szExePath = "client.exe";
        QProcess oP;
        oP.setProgram(szExePath);
        oP.setArguments(szArg);
        oP.startDetached();
        qApp->exit();
    }
    else
    {
        /*弹出登陆失败窗口*/
        QMessageBox::information(this, "登陆失败", "用户名或密码错误");
    }

}
