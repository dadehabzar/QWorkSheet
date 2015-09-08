#ifndef STORAGE_H
#define STORAGE_H
#include <QTabWidget>
#include <QTableWidget>
#include <QMessageBox>
#include <QFile>
#include <QDebug>

#include "exportexcelobject.h"

class Storage : public QObject
{
    Q_OBJECT
public:
    Storage(QObject *parent=0);
public slots:
    //void slotSaveAsXls(QString filename, QTabWidget *tabwidget, QMap<QString, QTableWidget *> g_tablewidget_map);
    void slotSaveAsCsv(QString filename, QString csv);
    void slotLoadFromCsv(QString filename);
signals:
    void sigResLoadFromCsv(QString header, QStringList csv);
};

#endif // STORAGE_H
