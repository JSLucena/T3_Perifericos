#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QtDebug>
#include <QTimer>
#include <time.h>
#include <QMessageBox>
typedef struct
{
    char cadastrado;
    char comando;
    char nome[20];
    char cargo[20];
    char matricula[10];
    char hora_entrada[10];
    char data_entrada[10];
    char hora_saida[10];
    char data_saida[10];
    int hora;
    int min;
    int seg;
    int dia;
    int mes;
    int ano;

} estrutura;
estrutura usuario;


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


void MainWindow::timer_teste()
{
    //qDebug("Entra nesta função a cada 1s");
    if(usuario.cadastrado == 's')
    {
        ui->cadastrado->setText("Usuario cadastrado");
    }
    else
    {
        ui->cadastrado->setText("Usuario não cadastrado");
    }
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
     qDebug() <<"receive"<<   serial->read((char*)&usuario,sizeof(estrutura));
       // ui->lineEdit->setText(usuario.nome);
       // sprintf(teste,"nome: %s\ncargo: %s\nmatricula: %s");

    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QString msg;
    char buffer[30];
    int bytes = 0;


    if(serial->isOpen())// se porta aberta
    {



         //EXEMPLO PARA ENVIAR A ESTRUTURA NO QT

        msg = ui->lineEdit->text();
        char c_str2[msg.length()];
        strcpy(usuario.nome,msg.toLatin1()); //ou direto => strcpy(usuario.nome,msg.toLatin1());
        msg = ui->CargoEdit->text();
        strcpy(usuario.cargo,msg.toLatin1());
        msg = ui->MatriculaEdit->text();
        strcpy(usuario.matricula,msg.toLatin1());
       usuario.comando='w';
        qDebug(usuario.nome);
        qDebug(usuario.cargo);
        qDebug(usuario.matricula);
         qDebug() <<"Trasnmit" <<serial->write((char *)&usuario,sizeof(estrutura)); //EXEMPLO PARA ENVIAR A ESTRUTURA NO QT
        serial->waitForBytesWritten(500);
    }
}

void MainWindow::on_conecta_clicked()
{
    serial->setPortName("COM3");
    serial->setBaudRate(9600);
    serial->setDataBits(static_cast<QSerialPort::DataBits>(8));
    serial->setParity(static_cast<QSerialPort::Parity>(0));
    serial->setStopBits(static_cast<QSerialPort::StopBits>(1));
    serial->setFlowControl(static_cast<QSerialPort::FlowControl>(0));
    if (serial->open(QIODevice::ReadWrite))
    {
        qDebug("Porta Aberta");
        ui->label->setText("Status da Porta : ABERTA");
        timer->start(1000);



        usuario.comando ='h';
        time(&segundos);
        localTime = localtime(&segundos);
        usuario.hora = localTime->tm_hour;
        usuario.min = localTime->tm_min;
        usuario.seg = localTime->tm_sec;
        usuario.dia = localTime->tm_mday;
        usuario.mes = localTime->tm_mon+1;
        usuario.ano = localTime->tm_year % 100; //escreve h
        qDebug() <<"Trasnmit" << serial->write((char *)&usuario,sizeof(usuario));
        serial->waitForBytesWritten(500);

    }
    else
    {
        qDebug("Não Abriu");
        ui->label->setText("Status da Porta : FECHADA");
    }
}

void MainWindow::on_leCartao_clicked()
{
    if(serial->isOpen())
    {
        usuario.comando ='l';
        qDebug() <<"Trasnmit" << serial->write((char *)&usuario,sizeof(usuario));
        serial->waitForBytesWritten(500);

   //     if(serial->bytesAvailable()>=sizeof(estrutura))
  //      {
    //        serial->read((char*)&usuario,sizeof(estrutura));
   //     }
    }
    if(usuario.cadastrado == 's')
    {
        ui->HoraEntrada->setText(usuario.hora_entrada);
        ui->HoraSaida->setText(usuario.hora_saida);
        ui->DataEntrada->setText(usuario.data_entrada);
        ui->DataSaida->setText(usuario.data_saida);
        ui->lineEdit->setText(usuario.nome);
        ui->CargoEdit->setText(usuario.cargo);
        ui->MatriculaEdit->setText(usuario.matricula);

    }
}

void MainWindow::on_apagaCartao_clicked()
{
    if(serial->isOpen())
    {
        usuario.comando = 'a';
        usuario.cadastrado = 'n';
        strcpy(usuario.nome ,"");
        strcpy(usuario.cargo ,"");
        strcpy(usuario.matricula ,"");
        strcpy(usuario.hora_entrada ,"");
        strcpy(usuario.hora_saida ,"");
        strcpy(usuario.data_entrada ,"");
        strcpy(usuario.data_saida ,"");
        usuario.hora = 0;
        usuario.min = 0;
        usuario.seg = 0;
        usuario.dia = 0;
        usuario.mes = 0;
        usuario.ano = 0;
        serial->write((char *)&usuario,sizeof(usuario));
        serial->waitForBytesWritten(500);

    }
}

void MainWindow::on_cadastrar_clicked()
{
    QString msg;
    msg = ui->lineEdit->text();
    char c_str2[msg.length()];
    strcpy(usuario.nome,msg.toLatin1()); //ou direto => strcpy(usuario.nome,msg.toLatin1());
    msg = ui->CargoEdit->text();
    strcpy(usuario.cargo,msg.toLatin1());
    msg = ui->MatriculaEdit->text();
    strcpy(usuario.matricula,msg.toLatin1());
    usuario.cadastrado = 's';
    usuario.comando = 'w';
    serial->write((char *)&usuario,sizeof(usuario));
    serial->waitForBytesWritten(500);
}

void MainWindow::on_ativa_clicked()
{
    QMessageBox msgBox;
       QPushButton *entrar = msgBox.addButton(tr("Entrar"), QMessageBox::ActionRole);
       QPushButton *sair = msgBox.addButton(tr("Sair"), QMessageBox::ActionRole);

       msgBox.setWindowTitle("Entrando ou Saindo?");

       msgBox.exec();

       time(&segundos);
       localTime = localtime(&segundos);

       char str[100],str2[100];
       sprintf(str,"%02d:%02d:%02d", localTime->tm_hour,localTime->tm_min, localTime->tm_sec);



       sprintf(str2,"%02d/%02d/%02d", localTime->tm_mday, localTime->tm_mon, localTime->tm_year%100);

       usuario.comando = 'm';

       if(msgBox.clickedButton() == entrar)
       {
           strcpy(usuario.hora_entrada,str);
           strcpy(usuario.data_entrada,str2);
           ui->HoraEntrada->setText(usuario.hora_entrada);
           ui->DataEntrada->setText(usuario.data_entrada);

           qDebug() << "entrando";



       }
       else if(msgBox.clickedButton() == sair)
       {
           qDebug() << "saindo";
           strcpy(usuario.hora_saida,str);
           strcpy(usuario.data_saida,str2);
            ui->HoraSaida->setText(usuario.hora_saida);
            ui->DataSaida->setText(usuario.data_saida);

       }
       serial->write((char *)&usuario,sizeof(usuario));
       serial->waitForBytesWritten(500);
}
