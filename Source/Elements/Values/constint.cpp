#include "Include/Elements/Values/constint.h"

namespace elements::values {

ConstInt::ConstInt()
{
    setMinInputs(0);
    setMaxInputs(0);
    setMinOutputs(1);
    setMaxOutputs(1);

    addOutput(ValueType::Int, "Value", IOSocket::CanHoldInt | IOSocket::CanChangeName);
}

void ConstInt::set(int32_t a_value)
{
    m_currentValue = a_value;
    m_outputs[0].value = m_currentValue;
}

} // namespace elements::values

