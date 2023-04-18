#include "Include/Nodes/Values/nodes_constbool.h"
#include "Include/Elements/Values/constbool.h"
#include "Include/UI/tablewidget.h"

#include <QCheckBox>

namespace nodes::values {

void ConstBool::showProperties()
{
    showCommonProperties();
    showIOProperties(IOSocketsType::Outputs);

    propertiesInsertTitle("Const Bool");

    int row = m_properties->rowCount();
    m_properties->insertRow(row);

    QTableWidgetItem *item{};

    item = new QTableWidgetItem{ "Value" };
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    m_properties->setItem(row, 0, item);

    auto const constBool = static_cast<elements::values::ConstBool *>(m_element);
    bool const current = constBool->currentValue();

    QCheckBox *const value = new QCheckBox;
    m_properties->setCellWidget(row, 1, value);
    value->setChecked(current);

    QObject::connect(value, &QCheckBox::stateChanged, [constBool](int state)
    {
        constBool->set(state == 2);

    });
}

} // namespace nodes::values
