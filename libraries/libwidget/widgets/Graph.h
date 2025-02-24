#pragma once

#include <libsystem/utils/RingBuffer.h>
#include <libwidget/Widget.h>

class Graph : public Widget
{
private:
    Color _color;
    float *_data;
    size_t _data_size;
    size_t _current;

public:
    Graph(Widget *parent, size_t data_size, Color data_color);

    ~Graph();

    void paint(Painter &painter, Rectangle rectangle);

    Vec2i size();

    void record(float data);
};
