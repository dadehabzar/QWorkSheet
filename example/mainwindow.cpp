#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../qworksheet.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

//QPointer<Testlib> libt;

    link = new QWorkSheet();
   //link->startshow();

    ui->setupUi(this);

    //libb->startshow();

    connect(this,SIGNAL(sigAddSheets(QStringList)),link,SLOT(slotAddSheets(QStringList)));
    connect(this,SIGNAL(sigAddColumns(QString, QStringList )),link,SLOT(slotAddColumns(QString , QStringList)));
    connect(this,SIGNAL(sigAddRows(QString , QString , QStringList  )),link,SLOT(slotAddRows(QString , QString , QStringList )));
    //link->show();

    ui->tabWidget->addTab(link,"Qworksheet");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::add()
{
    qDebug()<< "ok";
}

void MainWindow::on_pushButton_clicked()
{
    QStringList str;
    str << "1" << "2" << "3" << "4"<< "ali"<< "sadegh";
    emit sigAddSheets(str);
}

void MainWindow::on_pushButton_2_clicked()
{
    QStringList str;
//    QList<QColor> c;
    QList<int> i;
    i << 10 <<10 << 10 << 10 <<10;
//    c .append(Qt::red);
//    c .append(Qt::red);
//    c .append(Qt::red);
//    c .append(Qt::red);
//    c .append(Qt::red);
//    c .append(Qt::green);
//    c .append(Qt::yellow);
//c .append(Qt::yellow);
//c .append(Qt::yellow);
//c .append(Qt::yellow);
//c .append(Qt::yellow);

    str << "1"  << "2" << "3" << "4" << "5";
    emit sigAddColumns("3",str);
        emit sigAddColumns("2",str);
    emit sigAddColumns("1",str);
   // QStringList str2;
    //str2 << "11"  << "22" << "33" << "4c4" << "55" <<"ggugyuyt" << "ghjghjhg";
    //emit sigAddColumns("3",str2,c);
}

void MainWindow::on_pushButton_3_clicked()
{


    QApplication::processEvents();
    QList<QString> str;
    str << "1r " <<"2r" << "7r" << "4r"  <<"5r" << "5555";
//    emit  sigAddRows("1","Mohsen",str);
//    emit  sigAddRows("1","1c",str);
    emit  sigAddRows("3","1",str);
    emit  sigAddRows("3","2",str);
     QList<QString> str2;
     str2<< "test";
     emit  sigAddRows("3","4",str2);
     emit  sigAddRows("3","5",str2);
     emit  sigAddRows("3","3",str2);
emit  sigAddRows("2","2",str);

   // QList<QString> str2;
    //str2 << "1 " <<"2" << "7" << "4" <<"5dtgdfgdfgdfgdfgdfgdfgdfgdfg" ;
   // emit sigAddRows("3",str2);


}

void MainWindow::on_pushButton_4_clicked()
{

}



void MainWindow::on_checkBox_stateChanged(int arg1)
{

}



void MainWindow::on_pushButton_5_clicked()
{
   // QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath (),
    //tr("Excel Files (*.bin)"));
    QString fileName ="";
    QStringList str;
    str << "92"  ;
    emit sigAddColumns("3",str);
    //link->test_binary(fileName);

}
