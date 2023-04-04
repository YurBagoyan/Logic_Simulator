#pragma once
#ifndef SOCKETITEM_H
#define SOCKETITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QVector>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsSceneMouseEvent>

#include "Include/Elements/element.h"


class Node;
class LinkItem;

class SocketItem : public QGraphicsItem
{
public:
    enum class Type { Input, Output };
    constexpr static int SIZE = 16;

    SocketItem(Node* const node, const Type type);

    bool isInput() const { return m_type == Type::Input; }
    bool isOutput() const { return m_type == Type::Output; }

    QRectF boundingRect() const override;
    void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget) override;

    void showName() { m_nameHidden = false; }
    void hideName() { m_nameHidden = true; }

    void connect(SocketItem *const other);
    void disconnect(SocketItem *const other);
    void disconnectAll();
    void disconnectAllInputs();
    void disconnectAllOutputs();

    void markAsUsed() { m_used = true; }

public: /// Geters and Seters
    void setName(QString const &name) { m_name = name; }
    void setValueType(ValueType const type);
    void setElementId(size_t const elementId) { m_elementId = elementId; }
    void setSocketId(uint8_t const socketId) { m_socketId = socketId; }
    void setColors(QColor const signalOff, QColor const sigalOn);
    void setSignal(bool const signal);
    ValueType valueType() const { return m_valueType; }
    void setHover(bool const hover) { m_isHover = hover; }

    QString name() const { return m_name; }
    size_t elementId() const { return m_elementId; }
    uint8_t socketId() const { return m_socketId; }
    int nameWidth() const;

protected: /// Events
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent* event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
    void dropEvent(QGraphicsSceneDragDropEvent* event) override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, QVariant const& value) override;

private:
    void removeLink(LinkItem *const linkItem);
    LinkItem* linkBetween(SocketItem *const from, SocketItem *const to) const;

private:
    QString m_name;
    QFont m_font;
    ValueType m_valueType;

    Node *const m_node;
    Type m_type;

    size_t m_elementId;
    uint8_t m_socketId;

    QColor m_colorSignalOn{};
    QColor m_colorSignalOff{};
    bool m_isSignalOn{false};

    bool m_isHover{false};
    bool m_isDrop{false};
    bool m_used{false};
    bool m_nameHidden{false};

    QVector<LinkItem *> m_links{};
};

#endif // SOCKETITEM_H
