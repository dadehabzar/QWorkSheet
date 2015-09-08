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
    void sigAddSheets(QStringList sheet_names, QStringList titles, QStringList x_axis_labels, QStringList y_axis_labels,
                       QList<int> real_display_precision, QList<bool> have_x_axis, QList<bool> have_y_axis,
                       QList<bool> reverse_x_axis, QList<bool> reverse_y_axis);
    void sigAddColumns(QString sheet_name, QStringList columns_name ,QList<QColor> columns_color);
    void sigAddRows(QString sheet_name, QString column_name, QStringList data_pack);

private slots:

    void on_pushButton_addSheets_clicked();
    void on_pushButton_addCoulms_clicked();
    void on_pushButton_addRows_clicked();
};

#endif // MAINWINDOW_H
