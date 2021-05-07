#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include <QIntValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(750,500);
    ui->spinBox->setMaximum(26);
    ui->spinBox->setMinimum(1);
    ui->pushButton->setDisabled(true);
    ui->textEdit->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    messages.append(RotFunction(true,ui->textEdit->toPlainText()));
    QTextStream TS(&this->messages);
    ui->textBrowser->setText(TS.readAll());

}




void MainWindow::on_pushButton_4_clicked()
{
    messages = "";
    QTextStream TS(&this->messages);
    ui->textBrowser->setText(TS.readAll());
    ui->spinBox->setDisabled(false);
    ui->pushButton_2->setDisabled(false);
    ui->pushButton_3->setDisabled(false);
}


void MainWindow::on_pushButton_2_clicked()
{
   ROT = ui->spinBox->value();
   ui->pushButton->setDisabled(false);
   ui->textEdit->setDisabled(false);
   ui->spinBox->setDisabled(true);
   ui->pushButton_2->setDisabled(true);
}

QString MainWindow::RotFunction(bool CipherOrDecipher, QString message){
    //you are ciphering
    if(CipherOrDecipher==true)
    {
        for(int i =0;i<message.length();i++){
            if(message[i] == '\n')
            {
                message[i]='\n';
            }
            else
            message[i] = (QChar)(message[i].unicode() + ROT);
        }
        message.append('\n');
    }
    // you are decrypting
    else
    {
        for(int i =0;i<message.length();i++){
            if(message[i]=='\n'){
                message[i]='\n';
            }
            else
            message[i] = (QChar)(message[i].unicode()-ROT);
        }
    }
    return message;
}

void MainWindow::on_pushButton_3_clicked()
{
    messages = RotFunction(false,messages);
    QTextStream TS(&this->messages);
    ui->textBrowser->setText(TS.readAll());
    ui->pushButton_3->setDisabled(true);
}

