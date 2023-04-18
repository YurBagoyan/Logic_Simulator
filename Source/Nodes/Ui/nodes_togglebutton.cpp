#include "Include/Nodes/Ui/nodes_togglebutton.h"
#include "Include/Elements/Ui/togglebutton.h"
#include "Include/colors.h"
#include "Include/UI/tablewidget.h"

#include <QCheckBox>

namespace nodes::Ui {

class ToggleButton;

class ToggleButtonWidget : public QGraphicsItem
{
public:
    QRectF boundingRect() const override { return m_boundingRect; }

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override { (void)event; }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override
    {
        (void)event;
        m_state = !m_state;
        m_toggleButton->set(m_state);
    }

    void paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *widget) override
    {
        (void)option;
        (void)widget;

        auto innerRect = m_boundingRect.adjusted(5.0, 10.0, -5.0, -20.0).translated(0.0, 5.0);
        QBrush brush{ (m_state ? get_color(Color::BoolSignalOn) : get_color(Color::BoolSignalOff)) };
        QPen pen{ Qt::black };
        painter->setPen(pen);
        painter->setBrush(brush);
        painter->drawRect(innerRect);

        QSizeF const toggleSize{ 30.0, m_boundingRect.height() };
        qreal const toggleX{ (m_state ? m_boundingRect.right() - toggleSize.width() : m_boundingRect.left()) };
        QRectF const toggleRect{ QPointF{ toggleX, m_boundingRect.top() }, toggleSize };

        //    brush.setColor(QColor{ 120,  83,  74, 255 });
        brush.setColor(Qt::lightGray);
        pen.setColor(Qt::black);
        painter->setPen(pen);
        painter->setBrush(brush);
        painter->drawRect(toggleRect);
    }

    void setToggleButton(elements::Ui::ToggleButton *const toggleButton) { m_toggleButton = toggleButton; }

private:
    bool m_state{};
    QRectF m_boundingRect{ 0, 0, 80, 20 };
    elements::Ui::ToggleButton *m_toggleButton{};
};

ToggleButton::ToggleButton()
{
    auto const widget = new ToggleButtonWidget;
    setCentralWidget(widget);
}

void ToggleButton::paint(QPainter *a_painter, QStyleOptionGraphicsItem const *a_option, QWidget *a_widget)
{
    (void)a_option;
    (void)a_widget;

    paintBorder(a_painter);
}

void ToggleButton::showProperties()
{
    showCommonProperties();
    showIOProperties(IOSocketsType::Outputs);

    propertiesInsertTitle("Toggle Button");

    int row = m_properties->rowCount();
    m_properties->insertRow(row);

    QTableWidgetItem *item{};

    item = new QTableWidgetItem{ "Value" };
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    m_properties->setItem(row, 0, item);

    auto const element = static_cast<elements::Ui::ToggleButton *>(m_element);
    bool const current = element->currentValue();

    QCheckBox *const value = new QCheckBox;
    m_properties->setCellWidget(row, 1, value);
    value->setChecked(current);

    QObject::connect(value, &QCheckBox::stateChanged, [element](int a_state) { element->set(a_state == 2); });
}

void ToggleButton::elementSet()
{
    auto const widget = static_cast<ToggleButtonWidget *>(m_centralWidget);
    widget->setToggleButton(static_cast<elements::Ui::ToggleButton *>(m_element));
}

} // namespace nodes::ui
