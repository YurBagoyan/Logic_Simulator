#include "Include/Elements/Ui/togglebutton.h"

namespace elements::Ui {

ToggleButton::ToggleButton()
{
    setMinInputs(0);
    setMaxInputs(0);
    setMinOutputs(1);
    setMaxOutputs(1);

    addOutput(ValueType::Bool, "Output", IOSocket::CanHoldBool | IOSocket::CanChangeName);
}

void ToggleButton::toggle()
{
    m_currentValue = !m_currentValue;
    m_outputs[0].value = m_currentValue;
}

void ToggleButton::set(bool state)
{
    m_currentValue = state;
    m_outputs[0].value = m_currentValue;
}

} // namespace elements::Ui
