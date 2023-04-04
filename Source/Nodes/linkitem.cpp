#include "Include/Nodes/linkitem.h"
#include "Include/Nodes/socketitem.h"
#include "Include/colors.h"

LinkItem::LinkItem(QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
{
    setFlags(ItemSendsGeometryChanges | ItemIsFocusable | ItemIsSelectable);
    setZValue(-1);
    setAcceptHoverEvents(true);
}

QRectF LinkItem::boundingRect() const
{
    return m_shape.boundingRect();
}

QPainterPath LinkItem::shape() const
{
    return m_shape;
}

void LinkItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    (void)option;
    (void)widget;

    QColor  signalColor{ (m_isSignalOn ? m_colorSignalOn : m_colorSignalOff) };
    signalColor = get_color(Color::LinkLineColor);
    QColor const notActive{ (isSelected() ? get_color(Color::Selected) : signalColor) };
    QColor const hover{ get_color(Color::SocketHover) };

    QPen pen{ (m_isHover ? hover : notActive) };
    pen.setStyle((m_to ? Qt::SolidLine : Qt::DashDotLine));
    pen.setWidth(4);

    if (m_valueType != ValueType::Bool) {
        QPen dash = pen;
        QColor hover2 = signalColor;
        hover2.setAlpha(85);
        dash.setColor(hover2);
        dash.setStyle(Qt::DotLine);
        dash.setWidth(6);
        dash.setDashOffset(m_dashOffset);
        painter->setPen(dash);
        painter->drawPath(m_path);
    }

    painter->setPen(pen);
    painter->drawPath(m_path);
}

void LinkItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    (void)event;
    setHover(true);
}

void LinkItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    (void)event;
    setHover(false);
}

void LinkItem::advance(int phase)
{
    if (!phase) return;

    if (m_valueType != ValueType::Bool) m_dashOffset -= 0.1;

    update();
}

void LinkItem::setFrom(SocketItem *const from)
{
    m_from = from;

    QPointF const position{ mapFromScene(m_from->scenePos()) };
    setPos(position);

    trackNodes();
}

void LinkItem::setTo(SocketItem *const to)
{
    m_to = to;

    m_isSnapped = m_to != nullptr;

    trackNodes();
}

void LinkItem::setTo(QPointF const to)
{
    m_toPoint = mapFromScene(to);

    trackNodes();
}

void LinkItem::setHover(bool const hover)
{
    m_isHover = hover;
    if (m_from) m_from->setHover(m_isHover);
    if (m_to) m_to->setHover(m_isHover);
}

void LinkItem::setColors(QColor const signalOff, QColor const signalOn)
{
    m_colorSignalOff = signalOff;
    m_colorSignalOn = signalOn;
}

void LinkItem::setSignal(bool const signal)
{
    m_isSignalOn = signal;

    if (m_to) m_to->setSignal(signal);
}

void LinkItem::trackNodes()
{
    prepareGeometryChange();

    QPointF const linkItemPos{ m_from->scenePos() };
    setPos(linkItemPos);

    QPointF const toPoint{ (m_to ? mapFromScene(m_to->scenePos()) : m_toPoint) };
    m_toPoint = toPoint;

    double x = toPoint.x() < 0. ? toPoint.x() : 0.;
    double y = toPoint.y() < 0. ? toPoint.y() : 0.;
    double w = fabs(toPoint.x());
    double h = fabs(toPoint.y());

    m_boundingRect.setX(x);
    m_boundingRect.setY(y);
    m_boundingRect.setWidth(w);
    m_boundingRect.setHeight(h);

    QPointF c1{}, c2{};

    double dist{ fabs(m_toPoint.x()) * 0.5 };

    c1.setX(dist);

    c2.setX(m_toPoint.x() - dist);
    c2.setY(m_toPoint.y());

    m_path = QPainterPath{};
    m_path.cubicTo(c1, c2, m_toPoint);

    QPainterPathStroker stroker{};
    stroker.setWidth(15);
    m_shape = stroker.createStroke(m_path);
}
