#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void readData();
    void timer_teste();
    void on_pushButton_2_clicked();

    void on_conecta_clicked();

    void on_leCartao_clicked();

    void on_apagaCartao_clicked();

    void on_cadastrar_clicked();

    void on_ativa_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
};

#endif // MAINWINDOW_H
