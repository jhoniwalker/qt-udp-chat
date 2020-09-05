#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHostAddress>
#include <QUdpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void readSocket();

private slots:
    void on_connectButton_clicked();

    void on_sendButton_clicked();

private:
    Ui::MainWindow *ui;
    //defino variable myUdpSocket como puntero de tipo QUdpSocket
    QUdpSocket *myUdpSocket;
    QHostAddress ipAddress;
    quint16 port;
};
#endif // MAINWINDOW_H
