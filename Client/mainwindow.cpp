
#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this , &MainWindow::slotReadyRead);
    connect(socket,&QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    nextBlockSize = 0;


}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_ButtonConnect_clicked()
{
     socket->connectToHost("178.172.151.205", 2323);
    if(socket->waitForConnected(3000))
     {
         ui->textBrowser->setText( "Connection Successfully ^_^ ");
     }
     else
    {
          ui->textBrowser->setText(" Connection Error  -_-");
    }

}

void MainWindow::SendToServer(QString str)
{
    Data.clear();
    QString NickName;
    NickName = ui->LineName->text();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out<<quint16(0)<<QTime::currentTime()<<+"["+NickName+"] " + str;
    out.device()->seek(0);
    out<<quint16(Data.size()- sizeof(quint16));
    socket->write(Data);
    ui->lineEdit->clear();
}

void MainWindow::slotReadyRead()
{
    QDataStream in (socket);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status()==QDataStream::Ok)
    {
       // QString str;
       // in>>str;
       // ui->textBrowser->append(str);
        for(;;)
       {
           if(nextBlockSize == 0)
            {
                if(socket->bytesAvailable()<2)
               {
                   break;

               }
                in>>nextBlockSize;

            }

            if(socket->bytesAvailable()<nextBlockSize)
            {
                break;
            }
            QString str;

            QTime time;
            in>>time>>str;
            nextBlockSize = 0;
            ui->textBrowser->append(time.toString()+" "+ str);
       }

    }
    else
    {
        ui->textBrowser->append("read  error");
    }
}


void MainWindow::on_ButtonSend_clicked()
{

    SendToServer(ui->lineEdit->text());


}


void MainWindow::on_lineEdit_returnPressed()
{
    SendToServer(ui->lineEdit->text());
}






