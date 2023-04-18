#include "Include/Elements/Values/outputbool.h"

namespace elements::values {

OutputBool::OutputBool()
{
    setMinInputs(1);
    setMaxInputs(1);
    setMinOutputs(0);
    setMaxOutputs(0);

    addInput(ValueType::Bool, "State", IOSocket::CanHoldInt | IOSocket::CanChangeName);
}

void OutputBool::toggle()
{
    m_currentValue = !m_currentValue;
}


void OutputBool::set(bool state)
{
    m_currentValue = state;
}

} // namespace elements::values
