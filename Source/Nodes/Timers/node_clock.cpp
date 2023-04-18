#include "Include/Nodes/Timers/node_clock.h"
#include "Include/Elements/Timers/clock.h"
#include "Include/UI/tablewidget.h"

namespace nodes::timers {

void Clock::showProperties()
{
    showCommonProperties();
    showIOProperties(IOSocketsType::Outputs);

    propertiesInsertTitle("Clock");

    int currentIndex = m_properties->rowCount();
    m_properties->insertRow(currentIndex);

    QTableWidgetItem *item{};
    item = new QTableWidgetItem{ "Rate" };
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    m_properties->setItem(currentIndex, 0, item);

    auto const clock = static_cast<elements::timers::Clock *>(m_element);

    QSpinBox *rateValue = new QSpinBox;
    rateValue->setRange(10, 100000);
    rateValue->setValue(static_cast<int>(clock->duration().count()));
    rateValue->setSuffix("ms");
    m_properties->setCellWidget(currentIndex, 1, rateValue);

    QObject::connect(rateValue, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](int value) {
        auto const clockElement = static_cast<elements::timers::Clock *>(m_element);
        clockElement->setDuration(std::chrono::milliseconds(value));
    });
}

} // namespace nodes::timers
