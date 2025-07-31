#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include<QHostInfo>
#include <QLabel>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent *event);
private:
    QString  getlocalip();
private slots:
    void onNewConnection();
    void onConnected();
    void onDisconnected();
    void onStateChanged(QAbstractSocket::SocketState socketState);
    void onReadyRead();
    void on_actstart_triggered();
    void on_actstop_triggered();
    void on_actclear_triggered();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QLabel  *labelListen;
    QLabel *labelSocketstate;
    QTcpServer *tcpserver;
    QTcpSocket *tcpSocket;
};
#endif // MAINWINDOW_H
