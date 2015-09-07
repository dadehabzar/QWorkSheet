#include "sheetvar.h"

SheetVar::SheetVar()
{
    sheet_name = title = x_axis_label = y_axis_label = footer = "";
    real_display_precision = 0;
    have_x_axis = have_y_axis = true;
    reverse_x_axis = reverse_y_axis = false;
    curve_names.clear();
    curve_colors.clear();
}

void SheetVar::setSheetVar(QString sheet_name_, QString title_, QString x_axis_label_, QString y_axis_label_, QString footer_,
                      int real_display_precision_, bool have_x_axis_, bool have_y_axis_, bool reverse_x_axis_, bool reverse_y_axis_)
{
    sheet_name = sheet_name_;
    title = title_;
    footer = footer_;
    x_axis_label = x_axis_label_;
    y_axis_label = y_axis_label_;
    real_display_precision = real_display_precision_;
    have_x_axis = have_x_axis_;
    have_y_axis = have_y_axis_;
    reverse_x_axis = reverse_x_axis_;
    reverse_y_axis = reverse_y_axis_;
}

