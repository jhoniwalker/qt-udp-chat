#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QUdpSocket>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("UDP SOCKET CHAT");

    myUdpSocket = new QUdpSocket(this);
    //myUdpSocket->bind(localhost, localport);

    //se hace una conección con el socket y la señal listo para leer
    //connect(udosocket, SIGNAL(readyRead()),this,SLOT(readSocket()));

    //seteo el editor de texto plano para que sea de solo lectura.
    ui->receivedMessages->setReadOnly(true);
    connect(ui->lineEdit,SIGNAL(returnPresse()),this,SLOT(on_sendButton_clicked()))



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readSocket()
{
    while (myUdpSocket->hasPendingDatagrams()){

        //crear buffer
        QByteArray datagram;

        //se redimensiona el buffr con el tamaño de los datagrama entrantes
        datagram.resize(myUdpsocket->pendingDatagramSize());

        //direccion que envia los datos
        QHostAddress sender;

        //puerto vinculado a la dirección que envia los datos
        quintl6 senderPort;

        //lectura de datos
        myUdpsocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        //se despliega la información en el texto plano

        ui->receivedMessages->appendPlainText(QString(datagram)+"(ip -> "+sender.toString()+" : port -> "+QString("%1").arg(senderPort) + ")" )
    }
}

void MainWindow::on_connectButton_clicked()
{

}

void MainWindow::on_sendButton_clicked()
{

}
