#ifndef SHEETVAR_H
#define SHEETVAR_H

#include "general.h"

class SheetVar
{
public:
    SheetVar();
    QString sheet_name, title, x_axis_label, y_axis_label, footer;
    QStringList curve_names;
    QList<QColor> curve_colors;
    int real_display_precision;
    bool have_x_axis, have_y_axis, reverse_x_axis, reverse_y_axis;
    void setSheetVar( QString sheet_name_, QString title_, QString x_axis_label_, QString y_axis_label_, QString footer_,
                      int real_display_precision_,
                      bool have_x_axis_, bool have_y_axis_,
                      bool reverse_x_axis_, bool reverse_y_axis_ );
};

#endif // SHEETVAR_H
