#include "Include/colors.h"
#include "qdebug.h"

#include <array>

std::array<QColor, static_cast<size_t>(Color::Count)> const colors{ {
    { 25, 25, 25, 255 },    // Color::ViewBackground
    { 29, 84, 92, 230 },    // Color::NameBackground
    { 219, 220, 221, 255 }, // Color::FontName
    { 169, 169, 169, 255 }, // Color::FontType
    { 105, 105, 105, 64 },  // Color::NodeHeader
    { 105, 105, 105, 128 }, // Color::NodeContent
    { 54, 54, 54, 255 },    // Color::SocketBorder
    { 255, 99, 71, 255 },   // Color::SocketInput
    { 135, 206, 235, 255 }, // Color::SocketOutput
    { 173, 255, 47, 255 },  // Color::SocketDrop
    { 255, 215, 0, 255 },   // Color::SocketHover
    { 244, 53, 64, 255 },   // Color::BoolSignalOff
    { 75, 173, 88, 255 },   // Color::BoolSignalOn
    { 62, 84, 174, 255 },   // Color::IntegerSignalOff
    { 0, 170, 238, 255 },   // Color::IntegerSignalOn
    { 111, 80, 96, 255 },   // Color::FloatSignalOff
//  { 232, 0, 99, 255 },    // Color::FloatSignalOn
    { 254, 144, 50, 255 },  // Color::FloatSignalOn
    { 165, 165, 165, 64 },  // Color::Link
    { 255, 255, 255, 255 }  // Color::Selected
} };

QColor get_color(Color const color)
{
    switch (color) {
        case Color::ViewBackground:     return QColor{ 25, 25, 25, 255 };
        case Color::NameBackground:     return QColor{ 29, 84, 92, 230 };
        case Color::FontName:           return QColor{ 219, 220, 221, 255 };
        case Color::FontType:           return QColor{ 169, 169, 169, 255 };
        case Color::NodeHeader:         return QColor{ 105, 105, 105, 64 };
        case Color::NodeContent:        return QColor{ 105, 105, 105, 128 };
        case Color::SocketBorder:       return QColor{ 54, 54, 54, 255 };
        case Color::SocketInput:        return QColor{ 255, 99, 71, 255 };
        case Color::SocketOutput:       return QColor{ 135, 206, 235, 255 };
        case Color::SocketDrop:         return QColor{ 173, 255, 47, 255 };
        case Color::SocketHover:        return QColor{ 0, 170, 238, 255 }; // 255, 215, 0, 255
        case Color::BoolSignalOff:      return QColor{ 244, 53, 64, 255 }; // 244, 53, 64, 255
        case Color::BoolSignalOn:       return QColor{ 66, 245, 66, 255 }; // 66, 245, 66, 255
        case Color::IntegerSignalOff:   return QColor{ 62, 84, 174, 255 };
        case Color::IntegerSignalOn:    return QColor{ 0, 170, 238, 255 };
        case Color::FloatSignalOff:     return QColor{ 111, 80, 96, 255 };
        case Color::FloatSignalOn:      return QColor{ 254, 144, 50, 255 };
        //case Color::FloatSignalOn:    return QColor{ 232, 0, 99, 255 };
        case Color::Link:               return QColor{ 165, 165, 165, 64 };
        case Color::Selected:           return QColor{ 0, 170, 238, 255 }; //

        //case Color::LinkLineColor:      return QColor{ 255, 215, 0, 255 };
        //case Color::colorSignalOff:     return QColor{ 254, 144, 50, 255 };
        //case Color::colorSignalOn:      return QColor{ 52, 217, 200, 255 };
        case Color::Count: break;
    }

    qDebug() << "Err: returned QColor{255, 255, 255, 255}: color.cpp";
    return QColor{255, 255, 255, 255};
}
