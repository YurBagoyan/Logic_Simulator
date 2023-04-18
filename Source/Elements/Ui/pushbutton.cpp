#include "Include/Elements/Ui/pushbutton.h"

namespace elements::Ui {

PushButton::PushButton()
{
    setMinInputs(0);
    setMaxInputs(0);
    setMinOutputs(1);
    setMaxOutputs(1);

    addOutput(ValueType::Bool, "State", IOSocket::CanHoldBool | IOSocket::CanChangeName);
}

void PushButton::toggle()
{
    m_currentValue = !m_currentValue;
    m_outputs[0].value = m_currentValue;
}

void PushButton::set(bool a_state)
{
    m_currentValue = a_state;
    m_outputs[0].value = m_currentValue;
}

} // namespace elements::Ui
