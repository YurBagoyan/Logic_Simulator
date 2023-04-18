#include "Include/Nodes/Values/nodes_constint.h"
#include "Include/Elements/Values/constint.h"

#include <QSpinBox>
#include "Include/UI/tablewidget.h"

namespace nodes::values {

ConstInt::ConstInt()
{
    QFont font{};
    font.setFamily("Consolas");
    font.setPointSize(10);
    auto widget = new QGraphicsSimpleTextItem(QString::number(0));
    widget->setFont(font);

    auto brush = widget->brush();
    brush.setColor(Qt::white);
    widget->setBrush(brush);

    setCentralWidget(widget);

    m_info = widget;
}

void ConstInt::refreshCentralWidget()
{
    if (!m_element) return;
    int32_t const VALUE{ std::get<int32_t>(m_element->outputs()[0].value) };
    m_info->setText(QString::number(VALUE));

    calculateBoundingRect();
}

void ConstInt::showProperties()
{
    showCommonProperties();
    showIOProperties(IOSocketsType::Outputs);

    propertiesInsertTitle("Const Int");

    int row = m_properties->rowCount();
    m_properties->insertRow(row);

    QTableWidgetItem *item{};

    item = new QTableWidgetItem{ "Value" };
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    m_properties->setItem(row, 0, item);

    auto const CONST_INT = static_cast<elements::values::ConstInt*>(m_element);
    int const CURRENT = CONST_INT->currentValue();

    QSpinBox *const value = new QSpinBox;
    value->setRange(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max());
    value->setValue(static_cast<int>(CURRENT));
    m_properties->setCellWidget(row, 1, value);

    QObject::connect(value, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                     [CONST_INT](int a_value) { CONST_INT->set(a_value); });
}

} // namespace nodes::values
