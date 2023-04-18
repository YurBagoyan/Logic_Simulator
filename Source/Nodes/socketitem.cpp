#include "Include/Nodes/socketitem.h"

#include <QCursor>
#include <QFontMetrics>
#include <QApplication>
#include <QMimeData>
#include <QWidget>
#include <QGraphicsWidget>
#include <QDrag>

#include "Include/Nodes/node.h"
#include "Include/UI/graphicsview.h"
#include "Include/Nodes/linkitem.h"
#include "Include/colors.h"
#include "Include/package.h"


SocketItem::SocketItem(Node* const node, const Type type)
    : QGraphicsItem{ node }
    , m_node{ node }
    , m_type{ type }
{
    m_font.setFamily("Consolas");
    m_font.setPointSize(10);
    setFlags(QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemIsFocusable |
             QGraphicsItem::ItemSendsScenePositionChanges);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);

    setZValue(1);

    if (type == Type::Output) {
        setCursor(Qt::OpenHandCursor);
    } else {
        setAcceptDrops(true);
    }

    m_colorSignalOn = get_color(Color::BoolSignalOn);
    m_colorSignalOff = get_color(Color::BoolSignalOff);
}

QRectF SocketItem::boundingRect() const
{
    return QRectF{ -(static_cast<qreal>(SIZE) / 2.), -(static_cast<qreal>(SIZE) / 2.), static_cast<qreal>(SIZE),
                 static_cast<qreal>(SIZE) };
}

void SocketItem::paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF const rect = boundingRect();

    QPen pen{ get_color(Color::SocketBorder) };
    pen.setWidth(2);

    QBrush brush;

    if (m_isHover)
        brush.setColor(get_color(Color::SocketHover));
    else if (m_isDrop)
        brush.setColor(get_color(Color::SocketDrop));
    else if (!m_isSignalOn)
        brush.setColor(m_colorSignalOff);
    else if (m_isSignalOn)
        brush.setColor(m_colorSignalOn);

//    else if (m_type == Type::Input)
//        brush.setColor(get_color(Color::SocketInput));
//    else if (m_type == Type::Output)
//        brush.setColor(get_color(Color::SocketOutput));

    brush.setStyle(Qt::SolidPattern);
    painter->setPen(pen);
    painter->setBrush(brush);
    if (m_type == Type::Output)
        painter->drawEllipse(rect);
    else
        painter->drawRect(rect);

    if (m_used) {
        painter->save();
        painter->setPen(Qt::NoPen);
        painter->setBrush(pen.color());

        if (m_type == Type::Output) {
            painter->drawEllipse(rect.adjusted(4, 4, -4, -4));
        } else {
            painter->drawRect(rect.adjusted(4, 4, -4, -4));
        }

        painter->restore();
    }

    if (!m_nameHidden) {
        pen.setColor(get_color(Color::FontName));
        painter->setPen(pen);
        painter->setFont(m_font);

        QFontMetrics const metrics{ m_font };
        int const FONT_HEIGHT = metrics.height();

        if (m_type == Type::Input)
            painter->drawText(static_cast<int>(rect.width()) - 4, (FONT_HEIGHT / 2) - metrics.strikeOutPos(), m_name);
        else
            painter->drawText(-metrics.horizontalAdvance(m_name) - SIZE + SIZE / 3, (FONT_HEIGHT / 2) - metrics.strikeOutPos(), m_name);
    }
}

void SocketItem::connect(SocketItem * const other)
{
    auto const linkItem = new LinkItem;
    linkItem->setColors(m_colorSignalOff, m_colorSignalOn);
    linkItem->setValueType(m_valueType);
    linkItem->setFrom(this);
    linkItem->setTo(other);

    m_links.push_back(linkItem);
    m_used = true;

    other->m_links.push_back(linkItem);
    other->m_used = true;
    other->m_isDrop = false;

    scene()->addItem(linkItem);
}

void SocketItem::disconnect(SocketItem * const other)
{
    auto const link = linkBetween(this, other);
    if (!link) return;

    auto const FROM_ID = elementId();
    auto const FROM_SOCKET_ID = socketId();
    auto const TO_ID = other->elementId();
    auto const TO_SOCKET_ID = other->socketId();

    auto const graphicsView = m_node->graphicsView();
    auto const package = graphicsView->package();

    package->disconnect(FROM_ID, FROM_SOCKET_ID, TO_ID, TO_SOCKET_ID);

    removeLink(link);
    other->removeLink(link);

    if (m_links.empty()) m_used = false;
    m_isHover = false;
    other->m_used = false;
    other->m_isHover = false;

    delete link;
}

void SocketItem::disconnectAll()
{
    if (m_type == Type::Input)
      disconnectAllInputs();
    else
      disconnectAllOutputs();
}

void SocketItem::disconnectAllInputs()
{
    auto links = m_links;
    for (auto &link : links) {
        auto const from = link->from();
        from->disconnect(this);
    }
}

void SocketItem::disconnectAllOutputs()
{
    auto links = m_links;
    for (auto &link : links)
        disconnect(link->to());
}

void SocketItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event);

    m_isHover = true;

//    for (auto const link : m_links)
//        link->setHover(m_isHover);
}

void SocketItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event);

    m_isHover = false;

    for (auto const link : m_links)
        link->setHover(m_isHover);
}

void SocketItem::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
    Q_UNUSED(event);

    if (m_used) {
        event->ignore();
        return;
    }

    auto const view = reinterpret_cast<GraphicsView *>(scene()->views()[0]);

    LinkItem *const linkItem{ view->dragLink() };
    if (!linkItem || m_valueType != linkItem->valueType()) {
        event->ignore();
        return;
    }

    linkItem->setTo(this);
    m_isDrop = true;
}

void SocketItem::dragLeaveEvent(QGraphicsSceneDragDropEvent* event)
{
    Q_UNUSED(event);

    m_isDrop = false;

    auto const view = reinterpret_cast<GraphicsView *>(scene()->views()[0]);

    auto const linkItem = view->dragLink();
    if (!linkItem) return;
    linkItem->setTo(nullptr);
}

void SocketItem::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
    Q_UNUSED(event);
}

void SocketItem::dropEvent(QGraphicsSceneDragDropEvent* event)
{
    Q_UNUSED(event);

    auto const graphicsView = reinterpret_cast<GraphicsView*>(scene()->views()[0]);

    auto const linkItem = graphicsView->dragLink();
    if (!linkItem) return;

    m_links.push_back(linkItem);
    linkItem->setTo(this);
    m_used = true;
    m_isDrop = false;

    graphicsView->acceptDragLink();

    auto const package = graphicsView->package();
    auto const from = linkItem->from();

    package->connect(from->elementId(), from->socketId(), m_elementId, m_socketId);

    setSignal(linkItem->isSignalOn());
}

void SocketItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);

    if (m_type == Type::Input)
        return;

    //setCursor(Qt::ClosedHandCursor);
    setCursor(Qt::ArrowCursor);
}

void SocketItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);

    if (m_type == Type::Input) return;

    if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton)).length() < QApplication::startDragDistance())
      return;

    QMimeData*  mime = new QMimeData;
    QDrag* drag = new QDrag(event->widget());
    drag->setMimeData(mime);

    auto linkItem = new LinkItem;
    linkItem->setColors(m_colorSignalOff, m_colorSignalOn);
    linkItem->setValueType(m_valueType);
    linkItem->setFrom(this);
    linkItem->setSignal(m_isSignalOn);

    scene()->addItem(linkItem);
    m_links.push_back(linkItem);

    auto const view = reinterpret_cast<GraphicsView *>(scene()->views()[0]);
    view->setDragLink(linkItem);

    Qt::DropAction const action{ drag->exec() };
    if (action == Qt::IgnoreAction) {
        removeLink(linkItem);
        scene()->removeItem(linkItem);
        view->cancelDragLink();
    } else {
        m_used = true;
    }

    setCursor(Qt::OpenHandCursor);
}

void SocketItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);

    if (m_type == Type::Input) return;

    setSelected(true);
    for (auto const link : m_links)
        link->setSelected(true);


    setCursor(Qt::OpenHandCursor);
}

QVariant SocketItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    if (change == QGraphicsItem::ItemScenePositionHasChanged) {
        for (LinkItem *const link : m_links)
            link->trackNodes();
    }
    return QGraphicsItem::itemChange(change, value);
}

void SocketItem::removeLink(LinkItem * const linkItem)
{
    auto const it = std::remove(std::begin(m_links), std::end(m_links), linkItem);
    m_links.erase(it, std::end(m_links));
}

LinkItem *SocketItem::linkBetween(SocketItem * const from, SocketItem * const to) const
{
    auto const it = std::find_if(std::begin(m_links), std::end(m_links), [from, to](LinkItem *const a_link) {
        return a_link->from() == from && a_link->to() == to;
    });

    if (it == std::end(m_links))
        return nullptr;

    return *it;
}

void SocketItem::setColors(QColor const signalOff, QColor const sigalOn)
{
    m_colorSignalOff = signalOff;
    m_colorSignalOn = sigalOn;
}

void SocketItem::setValueType(const ValueType type)
{
    m_valueType = type;

    switch (m_valueType) {
        case ValueType::Bool: setColors(get_color(Color::BoolSignalOff), get_color(Color::BoolSignalOn)); break;
        case ValueType::Float: setColors(get_color(Color::FloatSignalOff), get_color(Color::FloatSignalOn)); break;
        case ValueType::Int: setColors(get_color(Color::IntegerSignalOn), get_color(Color::IntegerSignalOn)); break;
    }
}

int SocketItem::nameWidth() const
{
    QFontMetrics const metrics{ m_font };
    return metrics.horizontalAdvance(m_name);
}

void SocketItem::setSignal(bool const signal)
{
    m_isSignalOn = signal;

    if (m_type == Type::Output) {
        for (LinkItem *const link : m_links) {
            link->setSignal(signal);
        }
    }
}
