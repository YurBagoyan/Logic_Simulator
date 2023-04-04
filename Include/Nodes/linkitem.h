#ifndef LINKITEM_H
#define LINKITEM_H

#include <QGraphicsPathItem>
#include <QPainter>

#include "Include/Elements/element.h"

class QGraphicsDropShadowEffect;
class SocketItem;

constexpr int LINK_TYPE{ QGraphicsItem::UserType + 2 };

class LinkItem : public QGraphicsPathItem
{
public:
    LinkItem(QGraphicsItem* parent = nullptr);

    int type() const override { return LINK_TYPE; }

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget) override;
    void advance(int phase) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

    void trackNodes();

public: /// Getters and setters
    void setFrom(SocketItem *const from);
    void setTo(SocketItem *const to);
    void setTo(QPointF const to);
    void setHover(bool const hover);
    void setColors(QColor const signalOff, QColor const signalOn);
    void setSignal(bool const signal);
    void setValueType(ValueType const type) { m_valueType = type; }

    ValueType valueType() const { return m_valueType; }
    bool isSnapped() const { return m_isSnapped; }
    bool isSignalOn() const { return m_isSignalOn; }

    SocketItem *from() const { return m_from; }
    SocketItem *to() const { return m_to; }

private:
    SocketItem* m_from{};
    SocketItem* m_to{};

    QRectF m_boundingRect{};
    QPainterPath m_path{};
    QPainterPath m_shape{};

    QColor m_colorSignalOn{};
    QColor m_colorSignalOff{};

    QPointF m_toPoint{};

    qreal m_dashOffset{};
    ValueType m_valueType{};
    bool m_isHover{};
    bool m_isSnapped{};
    bool m_isSignalOn{};
};

#endif // LINKITEM_H
