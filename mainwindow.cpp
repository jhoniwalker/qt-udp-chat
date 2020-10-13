#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QUdpSocket>
#include <QMessageBox>
#include <QHostInfo>
#include <QRandomGenerator>
#include "util.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("UDP SOCKET CHAT");
    //qDebug() << this->size();
    this->setFixedSize(430,381);

    myUdpSocket = new QUdpSocket(this);

    util myutil;
    QString localhostIP = myutil.getIpHost();
    if (localhostIP == ""){
        localhostIP = myutil.getWifiIP();
    }
    qDebug() << myutil.getWifiIP();



    //Genero un puerto de forma randómica
    quint16 randPort = QRandomGenerator::global()->generate();


    /*Realizo el bindeo de la ip y el puerto al socket.
    Así se prepara a la aplicación para trabajar como servidor.
    La aplicación está preparada para recibir datos.*/
    myUdpSocket->bind(QHostAddress(localhostIP), randPort);

    //se hace una conexión con el socket y la señal listo para leer
    connect(myUdpSocket, SIGNAL(readyRead()),this,SLOT(readSocket()));

    //seteo el editor de texto plano para que sea de solo lectura.
    ui->receivedMessages->setReadOnly(true);

    //Muestro datos del servidor para que los contactos se puedan conectar.
    ui->receivedMessages->appendPlainText("Provea a su contacto de la siguiente información");
    ui->receivedMessages->appendPlainText("Mi dirección IP es: "+ QHostAddress(localhostIP).toString());
    ui->receivedMessages->appendPlainText("Mi puerto es: "+QString("%1").arg(randPort));

    //Se realiza una conexión con el input de envio de mensaje y la señal de la tecla enter.
    connect(ui->message,SIGNAL(returnPressed()),this,SLOT(on_sendButton_clicked()));



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
        ui->receivedMessages->appendPlainText(QString(datagram) +"(ip -> "+sender.toString()+" : port -> "+QString("%1").arg(senderPort) + ")" );
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
