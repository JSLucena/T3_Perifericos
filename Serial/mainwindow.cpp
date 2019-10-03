#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QtDebug>
#include <QTimer>
#include <time.h>

typedef struct
{
char nome[20];
char cargo[20];
char matricula[5];
char hora_entrada[10];
char data_entrada[10];
char hora_saida[10];
char data_saida[10];

} estrutura;
estrutura usuario;

typedef struct
{
   int hora;
   int min;
   int seg;
   int dia;
   int mes;
   int ano;

}estrutura2;
estrutura2 horaData;

QTimer *timer = new QTimer(); //cria novo timer

struct tm *localTime;
time_t segundos;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort(this);
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData); // interrupcao
    connect(timer, SIGNAL(timeout()),SLOT(timer_teste()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    serial->setPortName("COM3");
    serial->setBaudRate(115200);
    serial->setDataBits(static_cast<QSerialPort::DataBits>(8));
    serial->setParity(static_cast<QSerialPort::Parity>(0));
    serial->setStopBits(static_cast<QSerialPort::StopBits>(1));
    serial->setFlowControl(static_cast<QSerialPort::FlowControl>(0));
    if (serial->open(QIODevice::ReadWrite))
    {
        qDebug("Porta Aberta");
        ui->label->setText("Status da Porta : ABERTA");
        timer->start(1000);
    }
    else
    {
        qDebug("Não Abriu");
        ui->label->setText("Status da Porta : FECHADA");
    }
}

void MainWindow::timer_teste()
{
    qDebug("Entra nesta função a cada 1s");
}
void MainWindow::readData()
{
    char buffer[30];
    int x=0;
    char teste[60];
 //   const QByteArray data = serial->readAll();
//   qDebug(data);
//    ui->lineEdit->setText(data);
    // ou
    /*
    while(x<1)
        x=serial->bytesAvailable();
    serial->read(buffer,2);
    */

    if(serial->bytesAvailable()>=sizeof(estrutura))
    {
        serial->read((char*)&usuario,sizeof(estrutura));
       // ui->lineEdit->setText(usuario.nome);
       // sprintf(teste,"nome: %s\ncargo: %s\nmatricula: %s");
        ui->testeTXT->setText(usuario.nome);
        ui->testeTXT2->setText(usuario.cargo);
        ui->testeTXT3->setText(usuario.matricula);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QString msg;
    char buffer[30];
    int bytes = 0;
    buffer[0]='h';

    time(&segundos);
    localTime = localtime(&segundos);

    if(serial->isOpen())// se porta aberta
    {
        serial->write(buffer,1); //escreve h
        horaData.hora = localTime->tm_hour;
        horaData.min = localTime->tm_min;
        horaData.seg = localTime->tm_sec;
        horaData.dia = localTime->tm_mday;
        horaData.mes = localTime->tm_mon+1;
        horaData.ano = localTime->tm_year % 100;
        serial->waitForBytesWritten(500);
        serial->write((char *)&horaData,sizeof(horaData)); //EXEMPLO PARA ENVIAR A ESTRUTURA NO QT
        serial->waitForBytesWritten(500);

        msg = ui->lineEdit->text();


        char c_str2[msg.length()];
        strcpy(usuario.nome,msg.toLatin1()); //ou direto => strcpy(usuario.nome,msg.toLatin1());
        msg = ui->CargoEdit->text();
        strcpy(usuario.cargo,msg.toLatin1());
        msg = ui->CargoEdit->text();
        strcpy(usuario.matricula,msg.toLatin1());



        buffer[0]='w';
        serial->write(buffer,1);
        serial->waitForBytesWritten(500);
        serial->write((char *)&usuario,sizeof(estrutura)); //EXEMPLO PARA ENVIAR A ESTRUTURA NO QT
        serial->waitForBytesWritten(500);
    }
}
