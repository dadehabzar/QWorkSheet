#include "storage.h"

Storage::Storage(QObject *parent) : QObject(parent)
{
}

/*todo:
void Storage::slotSaveAsXls(QString filename,QTabWidget *tabwidget,QMap<QString ,QTableWidget *> g_tablewidget_map)
{
    if(filename.count()==filename.lastIndexOf('.')+4) filename.remove(filename.lastIndexOf('.'),4);
    for (int i = 0; i < tabwidget->count(); ++i) {
        ExportExcelObject obj(filename, tabwidget->tabText(i), g_tablewidget_map[tabwidget->tabText(i)]);
        int columns = g_tablewidget_map[tabwidget->tabText(i)]->columnCount();
        for(int j=0; j<columns; j++){
            QString str, header = g_tablewidget_map[tabwidget->tabText(i)]->horizontalHeaderItem(j)->text();
            int c=0;
            QChar last_char='.';
            foreach(QChar ch, header){
                if(!ch.isLetterOrNumber()) {
                    ch = '_';
                    if(last_char != '_'){ str.append(ch); }else{ c--; }
                }else{
                    str.append(ch);
                }
                last_char=ch;
                if(++c>19) break;
            }
            obj.addField(j,str,"char(20)");
        }
        int retVal = obj.export2Excel();
        if(retVal){
            //todo:
        }
    }
}*/


void Storage::slotSaveAsCsv(QString filename, QString csv)
{
    QFile mfile(filename);
    if(!mfile.open(QFile::WriteOnly | QIODevice::Text )){
        qDebug() << "QWorkSheet::Storage::slotSaveAsCsv Error: could not open file ("<<filename<<") to write from sender ("<<sender()->objectName()<<")";
        return;
    }
    QTextStream outcsvtxt(&mfile);
    outcsvtxt << csv;
    mfile.flush();
    mfile.close();
}

void Storage::slotLoadFromCsv(QString filename)
{
    QFile mfile(filename);
    if(!mfile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "QWorkSheet::Storage::slotLoadFromCsv Error: could not open file ("<<filename<<") to write from sender ("<<sender()->objectName()<<")";
        return;
    }
    QTextStream incsvtxt(&mfile);
    QString header,sheet_str,file_str = incsvtxt.readAll();
    QStringList lines_list,sheet_list;
    lines_list = file_str.split('\n');
    //get header
    while(!lines_list.at(0).contains('[')) {
        header.append( lines_list.at(0) +"\n" );
        lines_list.removeFirst();
        if(lines_list.count()==0) break;
    }
    //get and split sheets
    while(lines_list.count()>0){
        sheet_str = lines_list.at(0) +"\n" ;
        lines_list.removeFirst();
        if(lines_list.count()==0) break;
        while(!lines_list.at(0).contains('[')) {
            sheet_str += lines_list.at(0) +"\n" ;
            lines_list.removeFirst();
            if(lines_list.count()==0) break;
        }
        sheet_list.append(sheet_str);
    }

    emit sigResLoadFromCsv(header,sheet_list);
}




