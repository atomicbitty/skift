#include <libgraphic/Painter.h>
#include <libsystem/system/Logger.h>
#include <libwidget/widgets/RoundedPanel.h>

void rounded_panel_paint(RoundedPanel *widget, Painter *painter, Rectangle rectangle)
{
    __unused(rectangle);

    painter_fill_rounded_rectangle(painter, widget_get_bound(widget), widget->radius, widget_get_color(widget, THEME_MIDDLEGROUND));
}

static const WidgetClass rounded_panel_class = {
    .name = "RoundedPanel",
    .paint = (WidgetPaintCallback)rounded_panel_paint,
};

Widget *rounded_panel_create(Widget *parent, int radius)
{
    RoundedPanel *rounded_panel = __create(RoundedPanel);

    rounded_panel->radius = radius;

    widget_initialize(WIDGET(rounded_panel), &rounded_panel_class, parent);

    return WIDGET(rounded_panel);
}
