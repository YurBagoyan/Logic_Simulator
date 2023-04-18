#include "Include/Elements/Values/bcd.h"

namespace elements::values {

BCD::BCD()
  : Element{}
{
    setMinInputs(1);
    setMaxInputs(1);
    setMinOutputs(4);
    setMaxOutputs(4);

    addInput(ValueType::Int, "Number", IOSocket::CanHoldInt);

    addOutput(ValueType::Bool, "A", IOSocket::CanHoldBool);
    addOutput(ValueType::Bool, "B", IOSocket::CanHoldBool);
    addOutput(ValueType::Bool, "C", IOSocket::CanHoldBool);
    addOutput(ValueType::Bool, "D", IOSocket::CanHoldBool);
}

void BCD::calculate()
{
    int32_t const VALUE{ std::get<int32_t>(m_inputs[0].value) };

    m_outputs[0].value = static_cast<bool>(VALUE & (1 << 0));
    m_outputs[1].value = static_cast<bool>(VALUE & (1 << 1));
    m_outputs[2].value = static_cast<bool>(VALUE & (1 << 2));
    m_outputs[3].value = static_cast<bool>(VALUE & (1 << 3));
}

} // namespace elements::math
