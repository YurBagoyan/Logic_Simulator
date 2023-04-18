#include "Include/Elements/Gates/not.h"
#include "qdebug.h"

namespace elements::gates {

Not::Not()
    : Element {}
{
    setMinInputs(1);
    setMaxInputs(1);
    setMinOutputs(1);
    setMaxOutputs(1);

    addInput(ValueType::Bool, "#1", IOSocket::CanHoldBool | IOSocket::CanChangeName);

    addOutput(ValueType::Bool, "Output", IOSocket::CanHoldBool | IOSocket::CanChangeName);
}

void Not::calculate()
{
    if(m_inputs[0].id != 0) {
        m_outputs[0].value = !std::get<bool>(m_inputs[0].value);
    } else {
        m_outputs[0].value = false;
    }
}

} // namespace elements::gates


