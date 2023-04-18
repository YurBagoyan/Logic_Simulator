#include "Include/Elements/Gates/nor.h"

namespace elements::gates {

Nor::Nor()
    : Element {}
{
    setMinInputs(2);
    setMinOutputs(1);
    setMaxOutputs(1);

    addInput(ValueType::Bool, "#1", IOSocket::CanHoldBool | IOSocket::CanChangeName);
    addInput(ValueType::Bool, "#2", IOSocket::CanHoldBool | IOSocket::CanChangeName);

    addOutput(ValueType::Bool, "Output", IOSocket::CanHoldBool | IOSocket::CanChangeName);

    setDefaultNewInputFlags(IOSocket::CanHoldBool | IOSocket::CanChangeName);
}

void Nor::calculate()
{
    bool result = false;    
    bool notConnected = true;

    for (auto &input : m_inputs) {
        int id = input.id;
        if(id != 0) {
            notConnected = false;
            break;
        }
    }

    for (auto &input : m_inputs) {
        bool const value = std::get<bool>(input.value);
        result |= value;
        if (value) break;
    }

    if(notConnected) {
        m_outputs[0].value = false;
        return;
    }

    m_outputs[0].value = !result;
}

} // namespace elements::gates
