#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QUdpSocket>
#include <QMessageBox>
#include <QHostInfo>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("UDP SOCKET CHAT");

    myUdpSocket = new QUdpSocket(this);

    //obtengo datos de mi host y me quedo con la dirección ip
    QList <QHostAddress> listDir = QHostInfo::fromName(QHostInfo::localHostName()).addresses();
       //for (int i = 0; i < list.size(); i++)
          //qDebug() << list.at(i).toString();
    qDebug() << listDir.at(1).toString();
    //-------------------------
    //Realizo en bindeo de la ip y el puerto del servidor.
    //Para que funcionen en direfentes máquinas deben tener diferentes puertos
    myUdpSocket->bind(QHostAddress("192.168.0.12"), 7755);

    //se hace una conección con el socket y la señal listo para leer
    connect(myUdpSocket, SIGNAL(readyRead()),this,SLOT(readSocket()));

    //seteo el editor de texto plano para que sea de solo lectura.
    ui->receivedMessages->setReadOnly(true);
    ui->receivedMessages->appendPlainText("Provea a su contacto de la siguiente información");
    ui->receivedMessages->appendPlainText("Mi dirección IP es: "+ QHostAddress("192.168.0.12").toString());
    ui->receivedMessages->appendPlainText("Mi puerto es: "+QString("%1").arg(7755));
    connect(ui->message,SIGNAL(returnPresse()),this,SLOT(on_sendButton_clicked()));



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
        datagram.resize(myUdpSocket->pendingDatagramSize());

        //direccion que envia los datos
        QHostAddress sender;

        //puerto vinculado a la dirección que envia los datos
        quint16 senderPort;

        //lectura de datos
        myUdpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        //se despliega la información en el texto plano
        ui->receivedMessages->appendPlainText(QString(datagram)+"(ip -> "+sender.toString()+" : port -> "+QString("%1").arg(senderPort) + ")" );
    }
}

void MainWindow::on_connectButton_clicked()
{
   //hago la conexion
   if(ui->ipAddress->text().isEmpty()){

         QMessageBox::critical(this, "Error", "Complete el campo ip");

      }else{
         //se gurdan las ip y los puertos
         ipAddress = QHostAddress(ui->ipAddress->text());
         port = ui->port->value();
         QMessageBox::information(this, "Chat", "Conectando...");
      }

}

void MainWindow::on_sendButton_clicked()
{
    myUdpSocket->writeDatagram(ui->message->text().toLatin1(), ipAddress, port);
    ui->message->clear();
}
