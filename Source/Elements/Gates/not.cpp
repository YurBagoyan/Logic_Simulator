#include "Include/Elements/Gates/not.h"

Not::Not()
    : Element {}
{
    setMinInputs(1);
    setMaxInputs(1);
    setMinOutputs(1);
    setMaxOutputs(1);

    addInput(ValueType::Bool, "#1", IOSocket::CanHoldBool | IOSocket::CanChangeName);

    addOutput(ValueType::Bool, "State", IOSocket::CanHoldBool | IOSocket::CanChangeName);
}

void Not::calculate()
{
    m_outputs[0].value = !std::get<bool>(m_inputs[0].value);
}


