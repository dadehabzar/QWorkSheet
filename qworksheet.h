// Copyright (c) 2012 www.QtGroup.ir
// Designer & Developer: Abolfazl Rostamzadeh <a.rostamzadeh@yahoo.com>
// Coder Assistance: Mohsen Khavary <mohsenkhavary@yahoo.com>
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///////////////////////////////////////////////////////////////////////////////

#ifndef QWORKSHEET_H
#define QWORKSHEET_H

#include <QtGui>
#include <QTableWidget>
#include <QTabWidget>
#include <QDockWidget>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QToolButton>
#include <QCheckBox>
#include <QTimer>
#include <QSpinBox>

#include "storage.h"
#include "sheetvar.h"

#define VERSIONWORKSHEET "0.70"

#if defined(QWORKSHEET_LIBRARY)
#  define QWORKSHEETSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QWORKSHEETSHARED_EXPORT Q_DECL_IMPORT
#endif

class QWORKSHEETSHARED_EXPORT QWorkSheet : public QDockWidget
{
    Q_OBJECT
public:
    explicit QWorkSheet(QWidget *parent = 0);
    QString g_info_string;
signals:
    void sigSaveAsXls(QString filename, QTabWidget *tabwidget,QMap<QString ,QTableWidget *> g_tablewidget_map);
    void sigSaveAsCsv(QString filename, QString csv);
    void sigLoadFromCsv(QString filename);
    //to plotter
    void sigRemovePlots(QStringList plot_list);
    void sigAddPlots(QStringList plot_names, QStringList titles, QStringList x_axis_labels, QStringList y_axis_labels,
                     QList<int> real_display_precision,
                     QList<bool> have_x_axis, QList<bool> have_y_axis, QList<bool> reverse_x_axis, QList<bool> reverse_y_axis);
    void sigAddData(QString plot_name, QString curve_name, QPolygonF xydata);
    void sigAddCurves(QString plot_name, QStringList curve_names, QList<QColor> curve_colors, bool symbol);
    void sigSetFooter(QString plot_name, QString text);
    void sigSetPlotCurvesVisible(QString plot_name, QStringList curve_names, bool state=true);
    //to nmr
    void sigCreateToolbarWidgets();
public slots:
    void slotSetInfoText(QString infotext);
    void slotAddSheets(QStringList sheet_names, QStringList titles, QStringList x_axis_labels, QStringList y_axis_labels,
                       QList<int> real_display_precision, QList<bool> have_x_axis, QList<bool> have_y_axis,
                       QList<bool> reverse_x_axis, QList<bool> reverse_y_axis);
    void slotAddColumns(QString sheet_name, QStringList columns_name , QList<QColor> columns_color);
    void slotClearColumn(QString sheet_name, QString column_name );
    void slotAddRows(QString sheet_name, QString column_name, QStringList data_pack);
    void slotLoadTableWidgetFromTextStream(QString header, QStringList sheets);
    void slotSetFooter(QString sheet_name, QString text);
    void slotRemoveColumns(QString sheet_name, QStringList column_names);
private slots:
    void slotChangeAutosaveState();
    void slotToolbtnSaveAsClicked();
    void slotToolbtnOpenClicked();
    void slotAutoSave();
    void slotChangeCheckBoxAutoSave(bool checked = false);
private:
    QTabWidget *tabwidget; //tabwidget for storing sheets
    QMap<QString ,QTableWidget *> g_tablewidget_map;
    QMap<QString ,SheetVar *> g_sheetvar_map;
    QCheckBox *checkbox_autosave;
    QLineEdit *txtedit_save_filename;
    QSpinBox *spinbox_autosave_time;
    QTimer *timer_autosave;
    bool isAvailableSheetName(QString sheet_name);
    bool isAvailableColumnName(QString sheet_name, QString column_name);
    int  getColumnIndex(QString sheet_name, QString column_name);
    int  getLastRowIndex(QString sheet_name, int column_index);
    void autoSetItemsColor(QTableWidgetItem *newItemx, int row_index, int column_index);
    QString getCsvAsString(QString header);
    void createStorageThread();
    void createWidgets();
};

#endif // QWORKSHEET_H
