#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMetaEnum>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    labelListen =new QLabel("监听状态");
    labelListen->setMinimumWidth(200);
    ui->statusbar->addWidget(labelListen);
    labelSocketstate =new QLabel("cocket状态");
    labelSocketstate->setMinimumWidth(200);
    ui->statusbar->addWidget(labelSocketstate);
    QString getip=getlocalip();
    setWindowTitle(windowTitle() + "本机ip" +getip);
    ui->comboBox->addItem(getip);
    tcpserver=new QTcpServer(this);
    connect(tcpserver,SIGNAL(newConnection()),this,SLOT(onNewConnection()));
    ui->actstart->setEnabled(true);
    ui->actstop->setEnabled(false);
}
MainWindow::~MainWindow()
{
    delete ui;
}
QString MainWindow::getlocalip(){
    QString localname=QHostInfo::localHostName();
    QHostInfo hostinfo=QHostInfo::fromName(localname);
    QList<QHostAddress>addrlist=hostinfo.addresses();
    QString localip="";
    if(!addrlist.isEmpty()){
        for(int i=0;i<addrlist.count();i++){
            QHostAddress addr=addrlist.at(i);
            if(addr.protocol()==QAbstractSocket::IPv4Protocol){
                localip=addr.toString();
                break;
            }
        }
    }
    return  localip;
}
void MainWindow::on_actstart_triggered()
{
    QString  ip=ui->comboBox->currentText();
    quint16 port=ui->spinBox->value();
    QHostAddress addr(ip);
    tcpserver->listen(addr,port);
    ui->plainTextEdit->appendPlainText("开始监听");
    ui->plainTextEdit->appendPlainText("服务器地址"+ tcpserver->serverAddress().toString());
    ui->plainTextEdit->appendPlainText("服务器端口" + QString::number(tcpserver->serverPort()));
    ui->actstart->setEnabled(false);
    ui->actstop->setEnabled(true);

}
void MainWindow::onNewConnection(){
    tcpSocket=tcpserver->nextPendingConnection();
    onConnected();
    connect(tcpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,
            SLOT(onStateChanged(QAbstractSocket::SocketState)) );
    onStateChanged(tcpSocket->state());
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(onDisconnected()));

}
void MainWindow::onConnected(){
    ui->plainTextEdit->appendPlainText("client connect");
    ui->plainTextEdit->appendPlainText("perr address"+tcpSocket->peerAddress().toString());
    ui->plainTextEdit->appendPlainText("peer port"+QString::number(tcpSocket->peerPort()));

}
void MainWindow::onDisconnected(){
    ui->plainTextEdit->appendPlainText("client disconnected");
    tcpSocket->deleteLater();
}
void MainWindow::onStateChanged(QAbstractSocket::SocketState socketState){
    QMetaEnum metaenum=QMetaEnum::fromType<QAbstractSocket::SocketState>();
    labelSocketstate->setText("socket状态"+QString(metaenum.valueToKey(socketState)));

}
void MainWindow::on_actstop_triggered()
{
    if(tcpserver->isListening())
    {
        tcpserver->close();
        ui->actstart->setEnabled(true);
        ui->actstop->setEnabled(false);
        labelListen->setText("监听状态 已停止监听");
    }
}
void MainWindow::on_pushButton_clicked()
{
    QString  msg=ui->lineEdit->text();
    ui->plainTextEdit->appendPlainText("[发送]"+msg);
    ui->lineEdit->clear();
    ui->lineEdit->setFocus();
    QByteArray str=msg.toUtf8();
    str.append("\n");
    tcpSocket->write(str);
}
void MainWindow::onReadyRead(){
    while(tcpSocket->canReadLine())
        ui->plainTextEdit->appendPlainText("[接收]"+tcpSocket->readLine());
}
void MainWindow::closeEvent(QCloseEvent *event){
    if(tcpserver->isListening())
        tcpserver->close();
    event->accept();
}
void MainWindow::on_actclear_triggered(){
    ui->plainTextEdit->clear();
}
