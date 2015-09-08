#ifndef GENERAL_H
#define GENERAL_H

#include <QList>
#include <QColor>
#include <QString>
#include <QDebug>

bool haveListsEqualSize(QString func_name, QList<int> list);
quint32 convertQColorToInt(QColor color888);
QString convertQColorToQString(QColor color888);
QColor convertIntToQColor(quint32 color888);

#endif // GENERAL_H
