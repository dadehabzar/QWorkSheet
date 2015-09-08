#include "qworksheet.h"
#include "sheetvar.h"

bool g_isdata=false;
QWorkSheet::QWorkSheet(QWidget *parent): QDockWidget(parent)
{
    //qDebug () << "QWorkSheet::QWorkSheet" ;
    this->setObjectName("QWorkSheet"VERSIONWORKSHEET);
    this->setWindowTitle("WorkSheets");
    g_info_string = "Designed and Developed by www.QtGroup.ir";
    createStorageThread();
    createWidgets();
}

void QWorkSheet::slotSetInfoText(QString infotext)
{
    g_info_string = infotext;
}

void QWorkSheet::createWidgets()
{
    tabwidget = new QTabWidget(this);
    tabwidget->setTabPosition(QTabWidget::South);
    QGridLayout *maingridlayout = new QGridLayout;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabwidget);

    checkbox_autosave = new QCheckBox("Auto Save every");
    connect(checkbox_autosave,SIGNAL(stateChanged(int)),this,SLOT(slotChangeAutosaveState()));

    QToolButton *toolbtn_save = new QToolButton();
    toolbtn_save->setText("Save As");
    toolbtn_save->setIcon(QIcon(":/media/document-save-128.png"));
    toolbtn_save->setShortcut(tr("Ctrl+S"));
    toolbtn_save->setToolTip(tr("Save (*.csv) file ..."));
    toolbtn_save->setAutoRaise(true);
    connect(toolbtn_save,SIGNAL(clicked()),this,SLOT(slotToolbtnSaveAsClicked()));
    timer_autosave = new QTimer(this);
    connect(timer_autosave, SIGNAL(timeout()), this, SLOT(slotAutoSave()));

    QToolButton *toolbtn_open = new QToolButton();
    toolbtn_open->setText("Open");
    toolbtn_open->setIcon(QIcon(":/media/standardbutton-open-128.png"));
    toolbtn_open->setShortcut(tr("Ctrl+O"));
    toolbtn_open->setToolTip(tr("Open (*.csv) file ..."));
    toolbtn_open->setAutoRaise(true);
    connect(toolbtn_open,SIGNAL(clicked()),this,SLOT(slotToolbtnOpenClicked()));

    txtedit_save_filename = new QLineEdit();
    txtedit_save_filename->setEnabled(false);
    QLabel *label_time_unit = new QLabel("min. at");
    spinbox_autosave_time = new QSpinBox();
    spinbox_autosave_time->setMaximum(999);
    spinbox_autosave_time->setValue(15);
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(checkbox_autosave);
    hlayout->addWidget(spinbox_autosave_time);
    hlayout->addWidget(label_time_unit);
    hlayout->addWidget(txtedit_save_filename);
    hlayout->addWidget(toolbtn_save);
    hlayout->addWidget(toolbtn_open);

    QWidget *centralWidget = new QWidget(this);
    maingridlayout->addLayout(mainLayout,0,0);
    maingridlayout->addLayout(hlayout,1,0);
    centralWidget->setLayout(maingridlayout);

    this->setWidget(centralWidget);
}

void QWorkSheet::createStorageThread()
{
    QThread *thread = new QThread();
    Storage *mystorage = new Storage();
    mystorage->moveToThread(thread);
    connect( this, SIGNAL(destroyed()), thread, SLOT(quit()) );
    connect( this, SIGNAL(destroyed()), thread, SLOT(deleteLater()) );
    connect( this, SIGNAL(destroyed()), mystorage, SLOT(deleteLater()) );
    connect( this, SIGNAL(sigSaveAsCsv(QString,QString)),
             mystorage,SLOT(slotSaveAsCsv(QString,QString)) );
    //connect( this, SIGNAL(sigSaveAsXls(QString,QTabWidget*,QMap<QString,QTableWidget*>)), //todo:
    //         mystorage, SLOT(slotSaveAsXls(QString,QTabWidget*,QMap<QString,QTableWidget*>)) );
    connect( mystorage,SIGNAL(sigResLoadFromCsv(QString,QStringList)),this,SLOT(slotLoadTableWidgetFromTextStream(QString,QStringList)) );
    connect( this, SIGNAL(sigLoadFromCsv(QString)), mystorage, SLOT(slotLoadFromCsv(QString)) );
    thread->start();
    thread->setPriority(QThread::LowPriority);
}

bool QWorkSheet::isAvailableSheetName(QString sheet_name)
{
    for(int i=0; i<tabwidget->count(); i++)
        if(tabwidget->tabText(i)==sheet_name){ return true; }
    return false;
}

int QWorkSheet::getColumnIndex(QString sheet_name, QString column_name)
{
    //becarefull regarding sheet_name
    for(int i=0; i<g_tablewidget_map[sheet_name]->columnCount(); i++)
        if(g_tablewidget_map[sheet_name]->horizontalHeaderItem(i)->text()==column_name){
            return i;
        }
    return -1;
}

/// return last occupied row index
int QWorkSheet::getLastRowIndex(QString sheet_name, int column_index)
{
    //becarefull regarding sheet_name
    int row_count = g_tablewidget_map[sheet_name]->rowCount();
    for(int i=row_count-1; i>=0; i--){
        if(g_tablewidget_map[sheet_name]->item(i,column_index)){
            if(!g_tablewidget_map[sheet_name]->item(i,column_index)->text().isEmpty()){
                return i;
            }
        }else{
            //qDebug()<<"not found: row_index="<<i<<", column_index="<<column_index;
        }
    }
    return -1;
}

bool QWorkSheet::isAvailableColumnName(QString sheet_name, QString column_name)
{
    //becarefull regarding sheet_name
    if(getColumnIndex(sheet_name, column_name)>-1) return true;
    return false;
}

void QWorkSheet::slotAddSheets(QStringList sheet_names, QStringList titles, QStringList x_axis_labels,
                               QStringList y_axis_labels, QList<int> real_display_precision,
                               QList<bool> have_x_axis, QList<bool> have_y_axis,
                               QList<bool> reverse_x_axis, QList<bool> reverse_y_axis)
{
    foreach (QString var, sheet_names) {
        if(isAvailableSheetName(var)){
            qDebug () << "QWorkSheet::slotAddSheets Error: There is another sheet with the same name ("<<var<<") from sender ("<<sender()->objectName()<<")";
            return;
        }
    }
    QList<int> list;
    list<<sheet_names.count()<<titles.count()<<x_axis_labels.count()<<y_axis_labels.count()<<
          real_display_precision.count()<<have_x_axis.count()<<have_y_axis.count()<<
          reverse_x_axis.count()<<reverse_y_axis.count();
    if(!haveListsEqualSize("QWorkSheet::slotAddSheets ",list)) return;

    for (int i=0; i<sheet_names.count(); i++) {
        QString str = sheet_names.at(i);
        QTableWidget * my_tablewidget = new QTableWidget(this);        
        g_tablewidget_map[str] = my_tablewidget;
        SheetVar * my_sheetvar = new SheetVar();
        my_sheetvar->setSheetVar(str,titles.at(i),x_axis_labels.at(i),y_axis_labels.at(i),"",real_display_precision.at(i),
                                 have_x_axis.at(i),have_y_axis.at(i),reverse_x_axis.at(i),reverse_y_axis.at(i));
        g_sheetvar_map[str] = my_sheetvar;
        // QString qcolor2str="background-color: rgb(%1, %2, %3);";
        //qcolor2str=qcolor2str.arg(background.red()).arg(background.green()).arg(background.blue());
        //g_tablewidget_map[str]->setStyleSheet(qcolor2str);
        tabwidget->addTab(g_tablewidget_map[str],str);
    }
    g_isdata=true;
}

void QWorkSheet::slotAddColumns(QString sheet_name, QStringList columns_name, QList<QColor> columns_color )
{
    if(!isAvailableSheetName(sheet_name)){
        qDebug () << "QWorkSheet::slotAddColumns Error: There is no sheet ("<<sheet_name<<") from sender ("<<sender()->objectName()<<")";
        return;
    }
    foreach (QString var, columns_name){
        if(isAvailableColumnName(sheet_name,var)){
            qDebug () << "QWorkSheet::slotAddSheets Error: There is another column with the same name ("<<var<<") in sheet ("<<sheet_name<<") from sender ("<<sender()->objectName()<<")";
            return;
        }
    }
    QList<int> list;
    list<<columns_name.count()<<columns_color.count();
    if(!haveListsEqualSize("QWorkSheet::slotAddColumns ",list)) return;
    //getting headears
    QStringList headerlabels;
    for (int i = 0; i < g_tablewidget_map[sheet_name]->columnCount(); ++i) {
        headerlabels.append(g_tablewidget_map[sheet_name]->horizontalHeaderItem(i)->text());

    }
    //
    int columns_count=g_tablewidget_map[sheet_name]->columnCount()+columns_name.count();
    g_tablewidget_map.setSharable(false);

    g_tablewidget_map[sheet_name]->setColumnCount(columns_count);

    if(g_tablewidget_map[sheet_name]->rowCount()<=0) g_tablewidget_map[sheet_name]->setRowCount(1);
    g_tablewidget_map[sheet_name]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //
    headerlabels.append(columns_name);
    //
    g_sheetvar_map[sheet_name]->curve_names = headerlabels;
    g_sheetvar_map[sheet_name]->curve_colors.clear();
    g_sheetvar_map[sheet_name]->curve_colors.append(columns_color);
    g_tablewidget_map[sheet_name]->setHorizontalHeaderLabels(headerlabels);
    g_isdata = true;
    //
}

void QWorkSheet::slotClearColumn(QString sheet_name, QString column_name)
{
    if(!isAvailableSheetName(sheet_name)){
        qDebug () << "QWorkSheet::slotClearColumn Error: There is no sheet ("<<sheet_name<<") from sender ("<<sender()->objectName()<<")";
        return;
    }
    int column_index = getColumnIndex(sheet_name,column_name);
    if(column_index<0){
        qDebug () << "QWorkSheet::slotClearColumn Error: There is no column ("<<column_name<<") in sheet ("<<sheet_name<<") from sender ("<<sender()->objectName()<<")";
        return;
    }
    int row_index = getLastRowIndex(sheet_name,column_index);
    for(int i=row_index; i>=0; i--){
        g_tablewidget_map[sheet_name]->item(i,column_index)->setText("");
    }
}

void QWorkSheet::slotAddRows(QString sheet_name, QString column_name, QStringList data_pack)
{
    if(!isAvailableSheetName(sheet_name)){
        qDebug () << "QWorkSheet::slotAddRows Error: There is no sheet ("<<sheet_name<<") from sender ("<<sender()->objectName()<<")";
        return;
    }
    int column_index = getColumnIndex(sheet_name,column_name);
    if(column_index<0){
        qDebug () << "QWorkSheet::slotAddRows Error: There is no column ("<<column_name<<") in sheet ("<<sheet_name<<") from sender ("<<sender()->objectName()<<")";
        return;
    }
    int row_index = getLastRowIndex(sheet_name,column_index);
    //qDebug()<<"column_index="<<column_index<<", row_index="<<row_index;
    int row_count = g_tablewidget_map[sheet_name]->rowCount();
    foreach(QString var, data_pack){
        row_index++;
        if(row_count-1<=row_index) {
            g_tablewidget_map[sheet_name]->setRowCount(++row_count);
        }
        g_tablewidget_map[sheet_name]->setItem(row_index,column_index,new QTableWidgetItem(var));
        autoSetItemsColor(g_tablewidget_map[sheet_name]->item(row_index,column_index),row_index,column_index);
    }
    //g_tablewidget_map[sheet_name]->selectRow(row_index);
    g_isdata=true;
}

void QWorkSheet::slotLoadTableWidgetFromTextStream(QString header, QStringList sheets)
{
    QString error_str;
    if(sheets.isEmpty()) {
        error_str += tr("sheets is empty!\n") ;
        invalid_format:
        error_str = tr("Invalid or Corrupted CSV worksheet data file!\n") +error_str;
        QMessageBox  _qmbmsgbox;
        _qmbmsgbox.warning(0,g_info_string,error_str,QMessageBox::Yes);
        return;
    }
    g_tablewidget_map.clear();
    g_sheetvar_map.clear();
    tabwidget->clear();
    //plot_name+title+x_axis_label+y_axis_label+real_display_precision+have_x_axis+have_y_axis+reverse_x_axis+reverse_y_axis
    QStringList sheet_list;
    bool isNMR = false;
    QString first_plot_name;
    for(int i=0;i<sheets.count();i++){
        QString sheet_spec = sheets.at(i);
        sheets[i].remove(0, sheets[i].indexOf('\n')+1);//remove first line
        sheet_spec.remove(sheet_spec.indexOf('\n'), sheet_spec.count() - sheet_spec.indexOf('\n'));//keep first line
        sheet_spec.remove(0, sheet_spec.indexOf('[')+1);
        sheet_spec.remove(sheet_spec.indexOf(']'), sheet_spec.count() - sheet_spec.indexOf(']'));
        //within %%%
        QStringList same_sheet, titles, x_axis_labels, y_axis_labels, sheet_parts;
        QList<int> real_display_precision;
        QList<bool> have_x_axis, have_y_axis, reverse_x_axis, reverse_y_axis;
        QString footer;
        sheet_parts = sheet_spec.split("%%%");
        if (sheet_parts.count()!=10) {
            error_str += tr("parts count == ") + QString::number(sheet_parts.count()) + '\n';
            goto invalid_format;
        }
        sheet_list.append(sheet_parts.at(0));
        same_sheet.append(sheet_parts.at(0));
        if(i==0) first_plot_name = sheet_parts.at(0);
        if(!isNMR) isNMR = sheet_parts.at(0).contains("NMR",Qt::CaseSensitive);
        titles.append(sheet_parts.at(1));
        x_axis_labels.append(sheet_parts.at(2));
        y_axis_labels.append(sheet_parts.at(3));
        footer = sheet_parts.at(4);
        real_display_precision.append(sheet_parts.at(5).toInt());
        have_x_axis.append(sheet_parts.at(6).toInt()?true:false);
        have_y_axis.append(sheet_parts.at(7).toInt()?true:false);
        reverse_x_axis.append(sheet_parts.at(8).toInt()?true:false);
        reverse_y_axis.append(sheet_parts.at(9).toInt()?true:false);
        //qworksheet
        slotAddSheets(same_sheet,titles, x_axis_labels, y_axis_labels, real_display_precision, have_x_axis, have_y_axis, reverse_x_axis, reverse_y_axis);
        //qplotter
        emit sigRemovePlots(same_sheet);
        emit sigAddPlots(same_sheet, titles, x_axis_labels, y_axis_labels, real_display_precision,
                         have_x_axis, have_y_axis, reverse_x_axis, reverse_y_axis);
        emit sigSetFooter(same_sheet.at(0),footer);
        //
        sheet_spec = sheets.at(i);
        sheets[i].remove(0, sheets[i].indexOf('\n')+1);//remove first line
        sheet_spec.remove( sheet_spec.indexOf('\n'), sheet_spec.count() - sheet_spec.indexOf('\n') );//keep first line
        sheet_spec.remove(0, sheet_spec.indexOf('{')+1);
        sheet_spec.remove(sheet_spec.indexOf('}'), sheet_spec.count() - sheet_spec.indexOf('}'));
        QStringList curve_colors_st = sheet_spec.split(';');
        QList<QColor> curve_colors;
        for(int j=0; j<curve_colors_st.count(); j++){
            bool ok;
            QColor c = convertIntToQColor(curve_colors_st.at(j).toUInt(&ok,16));
            curve_colors.append( c );
        }
        //
        sheet_spec = sheets.at(i);
        sheets[i].remove(0, sheets[i].indexOf('\n')+1);//remove first line
        sheet_spec.remove( sheet_spec.indexOf('\n'), sheet_spec.count() - sheet_spec.indexOf('\n') );//keep first line
        QStringList columns_names = sheet_spec.split(',');
        //
        slotAddColumns(sheet_list.at(i),columns_names,curve_colors);
        QStringList curve_names = columns_names;
        curve_names.removeFirst();
        curve_colors.removeFirst();
        emit sigAddCurves(sheet_parts.at(0),curve_names,curve_colors,false);
        //
        QStringList data_lines = sheets.at(i).split('\n',QString::SkipEmptyParts);
        QPolygonF xydata;        
        for(int k=0; k < columns_names.count(); k++) {
            QStringList columns_data;
            bool not_empty_column;
            for(int j=0; j<data_lines.count(); j++) {
                QStringList data = data_lines.at(j).split(',');
                columns_data.append(data.at(k));
                if(k==0) xydata.append(QPointF(data.at(k).toDouble(&not_empty_column),0));
                  else xydata[j].setY(data.at(k).toDouble(&not_empty_column));
            }
            slotAddRows( sheet_list.at(i), columns_names.at(k), columns_data );
            if((k!=0) && not_empty_column) emit sigAddData( sheet_list.at(i), columns_names.at(k), xydata );
        }
        //
        if(i==sheets.count()-1) {
            QStringList curve_names;
            emit sigSetPlotCurvesVisible(first_plot_name, curve_names);
        }
    }
    //qnmr
    if(isNMR) emit sigCreateToolbarWidgets();
}

void QWorkSheet::slotSetFooter(QString sheet_name, QString text)
{
    if(!isAvailableSheetName(sheet_name)){
        qDebug () << "QWorkSheet::slotSetFooter Error: There is no sheet ("<<sheet_name<<") from sender ("<<sender()->objectName()<<")";
        return;
    }
    g_sheetvar_map[sheet_name]->footer = text;
}

//empty column_names to remove all column_names
void QWorkSheet::slotRemoveColumns(QString sheet_name, QStringList column_names)
{
    if(!isAvailableSheetName(sheet_name)){
        qDebug () << "QWorkSheet::slotRemoveColumns Error: There is no sheet ("<<sheet_name<<") from sender ("<<sender()->objectName()<<")";
        return;
    }
    if(column_names.count()==0){
        for(int i=0; i<g_tablewidget_map[sheet_name]->columnCount(); i++)
            column_names.append(g_tablewidget_map[sheet_name]->horizontalHeaderItem(i)->text());
    }
    foreach(QString column_name, column_names){
        int column_index = getColumnIndex(sheet_name,column_name);
        if(column_index<0){
            qDebug () << "QWorkSheet::slotRemoveColumns Error: There is no column ("<<column_name<<") in sheet ("<<sheet_name<<") from sender ("<<sender()->objectName()<<")";
            return;
        }
        int row_index = getLastRowIndex(sheet_name,column_index);
        for(int i=row_index; i>=0; i--){
            g_tablewidget_map[sheet_name]->item(i,column_index)->setText("");
        }
        g_tablewidget_map[sheet_name]->removeColumn(column_index);
    }
}

void QWorkSheet::autoSetItemsColor(QTableWidgetItem *newItemx,int row_index, int column_index)
{
    int _isg = column_index;
    int _irw = 0;
    if(row_index % 2) _irw=10;
    while(_isg>7) _isg-=7;
    switch(_isg){
    case 0:
    {
        if(newItemx) newItemx->setBackgroundColor(QColor(200-_irw,200-_irw,255));
        //if(newItemy) newItemy->setBackgroundColor(QColor(127,127,255));
        break;
    }
    case 1:
    {
        //if(newItemx) newItemx->setBackgroundColor(QColor(200,200,225));
        if(newItemx) newItemx->setBackgroundColor(QColor(200-_irw,225-_irw,200));
        break;
    }
    case 2:
    {
        //if(newItemx) newItemx->setBackgroundColor(QColor(200-_irw,200-_irw,200));
        if(newItemx) newItemx->setBackgroundColor(QColor(235-_irw,225-_irw,225));
        break;
    }
    case 3:
    {
        //if(newItemx) newItemx->setBackgroundColor(QColor(200,255,200));
        if(newItemx) newItemx->setBackgroundColor(QColor(127-_irw,210-_irw,127));
        break;
    }
    case 4:
    {
        if(newItemx) newItemx->setBackgroundColor(QColor(127-_irw,127-_irw,225));
        //if(newItemy) newItemy->setBackgroundColor(QColor(127,225,127));
        break;
    }
    case 5:
    {
        //if(newItemx) newItemx->setBackgroundColor(QColor(200,200,200));
        if(newItemx) newItemx->setBackgroundColor(QColor(127-_irw,200-_irw,127));
        break;
    }
    case 6:
    {
        if(newItemx) newItemx->setBackgroundColor(QColor(127-_irw,127-_irw,200));
        //if(newItemy) newItemy->setBackgroundColor(QColor(255,127,127));
        break;
    }
    case 7:
    {
        //if(newItemx) newItemx->setBackgroundColor(QColor(200,200,200));
        if(newItemx) newItemx->setBackgroundColor(QColor(200-_irw,127-_irw,127));
        break;
    }
    }
    //if(newItemz) newItemz->setBackgroundColor(QColor(0,0,0));
}

void QWorkSheet::slotAutoSave() //
{
    g_isdata=false;
    QString filename = txtedit_save_filename->text();
    if(filename!=""){
        if(filename.contains(".xls",Qt::CaseInsensitive)){
            for (int i = 0; i < tabwidget->count(); ++i)
            {
                int rows = g_tablewidget_map[tabwidget->tabText(i)]->rowCount();
                if(rows>65534){
                    if(((QTimer *)sender()) != timer_autosave){
                        QMessageBox  _qmbmsgbox;
                        _qmbmsgbox.warning(0,g_info_string,
                                           tr("Overflow rows (>65534) in columns of sheet ")+tabwidget->tabText(i)+
                                           tr(".\nPlease use *.csv or *.wst files for full data storage."),
                                           QMessageBox::Yes);
                    }else{
                        qDebug()<<g_info_string<<" QWorkSheet::slotAutoSave():: "
                               <<tr("Overflow rows (>65534) in columns of sheet ")<<tabwidget->tabText(i)
                              <<tr(". Please use *.csv or *.wst files for full data storage.");
                    }
                    goto savecsv;
                }
            }
            emit sigSaveAsXls(filename,tabwidget,g_tablewidget_map);
            return;
        }
savecsv:
        QString header;
        header += g_info_string + "\n";
        header += "Recorded by " + this->objectName()+ QDateTime::currentDateTime().toString(" @ yyyy.MM.dd  hh:mm:ss \r\n");

        QString csv = getCsvAsString(header);

        if(filename.contains(".wst",Qt::CaseInsensitive)){
            //QByteArray bin = convertCsvStringToBinary(csv);;
            //emit sigSaveAsBin(filename, bin);
            return;
        }

        //consider selection rule for furture extentions
        emit sigSaveAsCsv(filename, csv);
    }
}

void QWorkSheet::slotChangeAutosaveState()
{
    if(checkbox_autosave->isChecked()){
        if(txtedit_save_filename->text().isEmpty()){
            slotToolbtnSaveAsClicked();
        }
        if(!txtedit_save_filename->text().isEmpty()){
            timer_autosave->start(60000*spinbox_autosave_time->value());
        } else {
            QTimer::singleShot(100,this,SLOT(slotChangeCheckBoxAutoSave()));
        }
    }
    else{
        timer_autosave->stop();
    }
}

void QWorkSheet::slotChangeCheckBoxAutoSave(bool checked)
{
    checkbox_autosave->setChecked(checked);
}

void QWorkSheet::slotToolbtnSaveAsClicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), txtedit_save_filename->text(),
                                                    tr("Data files (*.csv)"));
    if (filename.isEmpty())
        return;
    int lastdot = filename.lastIndexOf('.');
    if(lastdot > -1){
        if( filename.count() - lastdot == 4 ) filename.remove(lastdot,4);
    }
    filename.append(".csv");

    txtedit_save_filename->setText(filename);
    slotAutoSave();
}

void QWorkSheet::slotToolbtnOpenClicked()
{
    if(g_isdata){
        QMessageBox msgBox;
        int rect= msgBox.warning(this,"Warning","Do you want to save data?",QMessageBox::Save |QMessageBox::Discard | QMessageBox::Cancel);
        if(rect==QMessageBox::Discard){
            QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), txtedit_save_filename->text(),
                                                            tr("Data files (*.csv)"));
            if (filename.isEmpty())
                return;
            emit sigLoadFromCsv(filename);
        }
        else if(rect==QMessageBox::Save )
            slotToolbtnSaveAsClicked();

    }
}

QString QWorkSheet::getCsvAsString(QString header)
{
    QString str, temp, cama, maxcama, all;
    SheetVar *sv;
    QTableWidget *tw;
    int i;
    for ( i = 0; i < tabwidget->count(); i++) {
        sv = g_sheetvar_map[tabwidget->tabText(i)];
        tw = g_tablewidget_map[tabwidget->tabText(i)];
        int columns = tw->columnCount();
        cama.clear();
        cama.fill(',',columns-1);
        if(maxcama.count()<cama.count()) maxcama=cama;        
        temp =  "[" +
                sv->sheet_name.remove(',') + "%%%" +
                sv->title.remove(',') + "%%%" +
                sv->x_axis_label.remove(',') + "%%%" +
                sv->y_axis_label.remove(',') + "%%%" +
                sv->footer.remove(',') + "%%%" +
                QString::number(sv->real_display_precision) +"%%%" +
                QString::number(sv->have_x_axis) + "%%%" +
                QString::number(sv->have_y_axis) + "%%%" +
                QString::number(sv->reverse_x_axis) + "%%%" +
                QString::number(sv->reverse_y_axis) +
                "]" +
                cama +
                "\n" ;
        str = "";
        for(int i=0; i<sv->curve_colors.count(); i++){
            str += convertQColorToQString(sv->curve_colors.at(i));
            if(i < sv->curve_colors.count()-1) str +=';';
        }
        temp += "{" + str + "}"+ cama+"\n";//color
        str="";
        for (int j = 0; j < columns; ++j) {
            str += tw->horizontalHeaderItem(j)->text().replace("\r\n"," ").replace("\n"," ");
            if (j<columns-1) str += ",";
        }
        temp += str + "\n";
        str = "";
        int rows = tw->rowCount();
        for (int k = 0; k < rows-1; ++k) {
            for (int m = 0; m < columns; m++ ) {
                if(tw->item(k,m)){
                    str +=  tw->item(k,m)->text();
                    if (m<columns-1) str += ",";
                }else{
                    break;
                }
            }
            str += "\n";
        }
        temp += str ;
        all += temp;
    }

    header.replace("\r\n","\n").replace("\n",maxcama + "\n");
    return header + all;
}


//    if(g_columns_count!=row_name.count()){
//        for (int i = 0; i < row_name.count(); ++i) {
//            g_tablewidget_map[sheet_name]->setItem((g_row_counter)-1,i,new QTableWidgetItem(row_name[i]));
//            g_tablewidget_map[sheet_name]->item((g_row_counter)-1,i)->setBackgroundColor(g_color_list.at(i));

//        }
//        for (int j = row_name.count()-1; j < g_columns_count-1; j++) {
//            g_tablewidget_map[sheet_name]->setItem((g_row_counter)-1,j+1,new QTableWidgetItem(" "));
//            g_tablewidget_map[sheet_name]->item((g_row_counter)-1,j+1)->setBackgroundColor(g_color_list.at(j+1));
//        }
//    }
//    else{

//        for (int i = 0; i < row_name.count(); ++i) {

//            g_tablewidget_map[sheet_name]->setItem((g_row_counter)-1,i,new QTableWidgetItem(row_name[i]));
//            g_tablewidget_map[sheet_name]->item((g_row_counter)-1,i)->setBackgroundColor(g_color_list.at(i));

//        }
//    }

//    g_row_counter++;


//    if(color.isEmpty())return;
//    for (int j = 0; j < color.count(); ++j) {

//        g_color_list.append(color.at(j));
//    }


//    for (int i = 0; i < color.count(); ++i) {
//        g_tablewidget_map[sheet_name]->setItem(0,i,new QTableWidgetItem(" "));
//        g_tablewidget_map[sheet_name]->item(0,i)->setBackgroundColor(color.at(i));
//    }
