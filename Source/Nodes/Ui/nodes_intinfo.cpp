#include "Include/Nodes/Ui/nodes_intinfo.h"
#include "Include/Elements/Ui/intinfo.h"
#include "Include/UI/tablewidget.h"

namespace nodes::Ui {

IntInfo::IntInfo()
{
    QFont font{};
    font.setFamily("Consolas");
    font.setPointSize(10);
    auto widget = new QGraphicsSimpleTextItem("0");
    widget->setFont(font);

    auto brush = widget->brush();
    brush.setColor(Qt::white);
    widget->setBrush(brush);

    setCentralWidget(widget);

    m_info = widget;
}

void IntInfo::refreshCentralWidget()
{
    if (!m_element) return;
    int32_t const value{ std::get<int32_t>(m_element->inputs()[0].value) };
    m_info->setText(QString::number(value));

    calculateBoundingRect();
}

void IntInfo::showProperties()
{
    showCommonProperties();
    showIOProperties(IOSocketsType::Inputs);
    showIOProperties(IOSocketsType::Outputs);
}

} // namespace nodes::Ui
