#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../qworksheet.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    link = new QWorkSheet();
    connect(this,SIGNAL(sigAddSheets(QStringList,QStringList,QStringList,QStringList,QList<int>, QList<bool> , QList<bool> ,
                                     QList<bool> , QList<bool> )),link,
            SLOT(slotAddSheets(QStringList,QStringList,QStringList,QStringList,QList<int>, QList<bool> , QList<bool> ,
                               QList<bool> , QList<bool> )));
    connect(this,SIGNAL(sigAddColumns(QString , QStringList  ,QList<QColor> )),link,SLOT(slotAddColumns(QString , QStringList  ,QList<QColor> )));
    connect(this,SIGNAL(sigAddRows(QString , QString , QStringList  )),link,SLOT(slotAddRows(QString , QString , QStringList )));

    ui->tabWidget->addTab(link,"Qworksheet");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_addSheets_clicked()
{
    QStringList sheet_names;
    sheet_names<< "n1" << "n2" << "n3" << "n4"<< "n5"<< "n6";
    QStringList titles;
    titles<< "t1" << "t2" << "t3" << "t4"<< "t5"<< "t6";
    QStringList x_axis_labels;
    x_axis_labels<< "x1" << "" << "" << ""<< ""<< "";
    QStringList y_axis_labels;
    y_axis_labels<< "y1" << "y2" << "y3" << "y4"<< "y5"<< "y6";
    QList<int> real_display_precision ;
    real_display_precision<< 1 << 1 << 1 << 1<< 1<< 1;
    QList<bool> have_x_axis;
    have_x_axis << true << false << false << false << false << false;
    QList<bool> have_y_axis;
    have_y_axis << true << true << true << true << true << true;
    QList<bool> reverse_x_axis;
    reverse_x_axis << false << false << false << false << false << false;
    QList<bool> reverse_y_axis;
    reverse_y_axis << false << false << false << false << false << false;

    emit sigAddSheets(sheet_names,titles,x_axis_labels,y_axis_labels,real_display_precision,have_x_axis
                      ,have_y_axis,reverse_x_axis,reverse_y_axis);
}

void MainWindow::on_pushButton_addCoulms_clicked()
{
    QStringList str;
    QList<int> i;
    i << 10 <<10 << 10 << 10 <<10 << 12;
    QList<QColor> c;
    c .append(Qt::red);
    c .append(Qt::red);
    c .append(Qt::red);
    c .append(Qt::red);
    c .append(Qt::green);
    c .append(Qt::yellow);

    str << "c1"  << "c2" << "c3" << "c4" << "c5" << "c6";
    emit sigAddColumns("n1",str,c);
    emit sigAddColumns("n2",str,c);
    emit sigAddColumns("n3",str,c);
}

void MainWindow::on_pushButton_addRows_clicked()
{
    QApplication::processEvents();
    QList<QString> str;
    str << "1r " <<"2r" << "7r" << "4r"  <<"5r" << "6r";
    emit  sigAddRows("n3","c1",str);
    emit  sigAddRows("n3","c2",str);
    QList<QString> str2;
    str2<< "test";
    emit  sigAddRows("n3","c4",str2);
    emit  sigAddRows("n3","c5",str2);
    emit  sigAddRows("n3","c3",str2);
    emit  sigAddRows("n2","c6",str);
}

