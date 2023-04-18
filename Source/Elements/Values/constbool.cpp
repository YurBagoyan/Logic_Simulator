#include "Include/Elements/Values/constbool.h"

namespace elements::values {

ConstBool::ConstBool()
{
    setMinInputs(0);
    setMaxInputs(0);
    setMinOutputs(1);
    setMaxOutputs(1);

    addOutput(ValueType::Bool, "Output", IOSocket::CanHoldBool | IOSocket::CanChangeName);
}

void ConstBool::toggle()
{
    m_currentValue = !m_currentValue;
    m_outputs[0].value = m_currentValue;
}

void ConstBool::set(bool a_state)
{
    m_currentValue = a_state;
    m_outputs[0].value = m_currentValue;
}


} // namespace elements::values
