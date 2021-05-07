#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include <QIntValidator>
#include <QClipboard>

QClipboard *clipboard = QGuiApplication::clipboard();


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(750,500);
    ui->spinBox->setMaximum(26);
    ui->spinBox->setMinimum(1);
    ui->pushButton->setDisabled(true);
    ui->pushButton_3->setDisabled(true);
    ui->textEdit->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 *on_pushButton_clicked() controls the copy button
 */


void MainWindow::on_pushButton_clicked()
{
    clipboard->setText(RotFunction(true,ui->textEdit->toPlainText()));
    messages.append(clipboard->text());
    ui->textBrowser->setText(messages);
    ui->textEdit->setText("");
    ui->pushButton_3->setDisabled(false);

}


/*
 * on_pushButton_4_clicked() controls the clear clipboard button
 */

void MainWindow::on_pushButton_4_clicked()
{
    messages = "";
    QTextStream TS(&this->messages);
    ui->textBrowser->setText(TS.readAll());
    ui->textEdit->setText(TS.readAll());
    ui->textEdit->setDisabled(true);
    ui->spinBox->setDisabled(false);
    ui->pushButton_2->setDisabled(false);
    ui->pushButton_3->setDisabled(true);
    ui->pushButton->setDisabled(true);
}
/*
 * on_pushButton_2_clicked() controls setRot button.
 */

void MainWindow::on_pushButton_2_clicked()
{
   ROT = ui->spinBox->value();
   ui->pushButton->setDisabled(false);
   ui->textEdit->setDisabled(false);
   ui->spinBox->setDisabled(true);
   ui->pushButton_2->setDisabled(true);
}

/*
 * RotFunction(1 = cipher 0 = decipher, message)
 */

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
/*
 * on_pushButton_3_clicked() is the decrypt button.
 */
void MainWindow::on_pushButton_3_clicked()
{
    messages = RotFunction(false,messages);
    ui->textBrowser->setText(messages);
    ui->pushButton_3->setDisabled(true);
    ui->pushButton->setDisabled(true);
}
