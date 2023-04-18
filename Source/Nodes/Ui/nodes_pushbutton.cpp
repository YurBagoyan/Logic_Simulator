#include "Include/Nodes/Ui/nodes_pushbutton.h"
#include "Include/Elements/Ui/pushbutton.h"
#include "Include/UI/tablewidget.h"
#include "Include/colors.h"
#include <QCheckBox>

namespace nodes::Ui {

class PushButton;

class PushButtonWidget : public QGraphicsItem {
public:
    QRectF boundingRect() const override { return m_boundingRect; }

    void mousePressEvent(QGraphicsSceneMouseEvent *a_event) override
    {
        (void)a_event;
        m_state = true;
        m_pushButton->set(m_state);
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *a_event) override
    {
        (void)a_event;
        m_state = false;
        m_pushButton->set(m_state);
    }

    void paint(QPainter *a_painter, QStyleOptionGraphicsItem const *a_option, QWidget *a_widget) override
    {
        (void)a_option;
        (void)a_widget;

        QBrush const BRUSH{ (m_state ? get_color(Color::BoolSignalOn) : get_color(Color::BoolSignalOff)) };
        QPen const PEN{ Qt::black };

        a_painter->setPen(PEN);
        a_painter->setBrush(BRUSH);
        a_painter->drawRect(m_boundingRect);
    }

    void setPushButton(elements::Ui::PushButton *const a_pushButton) { m_pushButton = a_pushButton; }

private:
    bool m_state{};
    QRectF m_boundingRect{ 0, 0, 80, 20 };
    elements::Ui::PushButton *m_pushButton{};
};

PushButton::PushButton()
{
    auto const widget = new PushButtonWidget;
    setCentralWidget(widget);
}

void PushButton::paint(QPainter *a_painter, QStyleOptionGraphicsItem const *a_option, QWidget *a_widget)
{
    (void)a_option;
    (void)a_widget;

    paintBorder(a_painter);
}

void PushButton::showProperties()
{
    showCommonProperties();
    showIOProperties(IOSocketsType::Outputs);

    propertiesInsertTitle("Push Button");

    int row = m_properties->rowCount();
    m_properties->insertRow(row);

    QTableWidgetItem *item{};

    item = new QTableWidgetItem{ "Value" };
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    m_properties->setItem(row, 0, item);

    auto const element = static_cast<elements::Ui::PushButton *>(m_element);
    bool const current = element->currentValue();

    QCheckBox *const value = new QCheckBox;
    m_properties->setCellWidget(row, 1, value);
    value->setChecked(current);

    QObject::connect(value, &QCheckBox::stateChanged, [element](int a_state) { element->set(a_state == 2); });
}

void PushButton::elementSet()
{
    auto const pushButtonWidget = static_cast<PushButtonWidget *>(m_centralWidget);
    pushButtonWidget->setPushButton(static_cast<elements::Ui::PushButton *>(m_element));
}

} // namespace nodes::Ui
