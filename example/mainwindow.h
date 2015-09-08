#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../qworksheet.h"
#include "QTabWidget"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QWorkSheet *link;


private:
    Ui::MainWindow *ui;

signals:
    void sigAddSheets(QStringList sheet_names);
    void sigAddColumns(QString sheet_name, QStringList columns_name );
    void sigAddRows(QString sheet_name, QString column_name, QStringList data_pack);
public slots:
    void add();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    //void on_checkBox_clicked();
    void on_checkBox_stateChanged(int arg1);
   // void on_MainWindow_iconSizeChanged(const QSize &iconSize);
    void on_pushButton_5_clicked();
};

#endif // MAINWINDOW_H
