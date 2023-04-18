#include "Include/Nodes/Values/nodes_outputbool.h"
#include "Include/Elements/Values/outputbool.h"
#include "Include/UI/tablewidget.h"
#include "Include/colors.h"

#include <QCheckBox>

namespace nodes::values {

class OutputBool;

class OutputBoolWidget : public QGraphicsItem {
public:

    OutputBoolWidget()
      : QGraphicsItem{}
    {

    }

    QRectF boundingRect() const override { return m_boundingRect; }

//    void mousePressEvent(QGraphicsSceneMouseEvent *event) override
//    {
//        (void)event;
//        m_state = true;
//        m_pushButton->set(m_state);
//    }

//    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override
//    {
//        (void)event;
//        m_state = false;
//        m_outputBool->set(m_state);
//    }

    void paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *widget) override
    {
        (void)option;
        (void)widget;

        QBrush const BRUSH{ (m_state ? get_color(Color::BoolSignalOn) : get_color(Color::BoolSignalOff)) };
        QPen const PEN{ Qt::black };

        painter->setPen(PEN);
        painter->setBrush(BRUSH);
        painter->drawRect(m_boundingRect);
    }


    void setOutputBool(elements::values::OutputBool *const outputBool) { m_outputBool = outputBool; }
    void setState(bool const value) { m_state = value; }

private:
    bool m_state{};
    QRectF m_boundingRect{ 0, 0, 80, 20 };
    elements::values::OutputBool *m_outputBool{};

signals:
    void stateChanged(bool const state);
};


OutputBool::OutputBool()
{
    auto const widget = new OutputBoolWidget;
    setCentralWidget(widget);

    m_widget = widget;
}

void OutputBool::paint(QPainter *a_painter, QStyleOptionGraphicsItem const *a_option, QWidget *a_widget)
{
    (void)a_option;
    (void)a_widget;

    paintBorder(a_painter);
}

void OutputBool::refreshCentralWidget()
{
    if (!m_element) return;

    bool const value = std::get<bool>(m_element->inputs()[0].value);
    m_widget->setState(value);
}

void OutputBool::showProperties()
{
    showCommonProperties();
    showIOProperties(IOSocketsType::Outputs);

    propertiesInsertTitle("Output (Bool)");

    int row = m_properties->rowCount();
    m_properties->insertRow(row);

    QTableWidgetItem *item{};

    item = new QTableWidgetItem{ "Value" };
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    m_properties->setItem(row, 0, item);

    auto const element = static_cast<elements::values::OutputBool *>(m_element);
    bool const current = element->currentValue();

    QTableWidgetItem *const value = new QTableWidgetItem { "Bool"} ;
    value->setFlags(item->flags() & ~Qt::ItemIsEditable);
    m_properties->setItem(row, 1, value);

}

void OutputBool::elementSet()
{
    auto const outputBoolWidget = static_cast<OutputBoolWidget *>(m_centralWidget);
    outputBoolWidget->setOutputBool(static_cast<elements::values::OutputBool *>(m_element));
}

} // namespace nodes::values
