#include "general.h"

bool haveListsEqualSize(QString func_name, QList<int> list)
{
    int count = list.count();
    if(count<2){
        qDebug()<<"qworksheet::haveListsEqualSize Error: list count is "<<count<<" in "<<func_name;
        return false;
    }
    for(int i=0;i<count;i++){
        if(list.at(i)!=list.at(0)){
            qDebug()<<func_name<<"Error: unequal list count at index "<<i<<" in "<<func_name;
            return false;
        }
    }
    return true;
}

// qcolor to int
quint32 convertQColorToInt(QColor color888)
{
    return (0xFF << 24) +
           (color888.red() << 16) +
           (color888.green() << 8) +
            color888.blue();
}

// qcolor to qstring
QString convertQColorToQString(QColor color888)
{
    return QString("%1").
            arg(convertQColorToInt(color888), 6, 16, QLatin1Char('0')).
            toUpper().prepend("0x");
}

// int to qcolor
QColor convertIntToQColor(quint32 color888)
{
    QColor c;
    c = QRgb();
    c.setRed((color888 >>16) & 0xFF);
    c.setGreen((color888 >>8) & 0xFF);
    c.setBlue(color888 & 0xFF);
    c.setAlpha(0xFF);
    //qDebug()<<color888<<c.red()<<c.green()<<c.blue()<<((color888 >>16) & 0xFF)<<((color888 >>8) & 0xFF)<<(color888 & 0xFF);
    return c;
}
