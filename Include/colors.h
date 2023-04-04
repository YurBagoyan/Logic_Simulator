#ifndef COLORS_H
#define COLORS_H

#include <QColor>

enum class Color {
    ViewBackground,
    NameBackground,
    FontName,
    FontType,
    NodeHeader,
    NodeContent,
    SocketBorder,
    SocketInput,
    SocketOutput,
    SocketDrop,
    SocketHover,
    BoolSignalOff,
    BoolSignalOn,
    IntegerSignalOff,
    IntegerSignalOn,
    FloatSignalOff,
    FloatSignalOn,
    Link,
    Selected,

    LinkLineColor,
    colorSignalOff,
    colorSignalOn,

    Count
};

QColor get_color(Color const color);

#endif // COLORS_H
