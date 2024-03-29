#include "Include/Elements/Gates/and.h"

namespace elements::gates {

And::And()
    : Element{}
{
    setMinInputs(2);
    setMinOutputs(1);
    setMaxOutputs(1);

    addInput(ValueType::Bool, "#1", IOSocket::CanHoldBool | IOSocket::CanChangeName);
    addInput(ValueType::Bool, "#2", IOSocket::CanHoldBool | IOSocket::CanChangeName);

    addOutput(ValueType::Bool, "Output", IOSocket::CanHoldBool | IOSocket::CanChangeName);

    setDefaultNewInputFlags(IOSocket::CanHoldBool | IOSocket::CanChangeName);
}

void And::calculate()
{
    bool result = true ;
    for (auto &input : m_inputs) {
        bool const value = std::get<bool>(input.value);
        if (!value) {
            result = false;
            break;
        }
    }

    m_outputs[0].value = result;
}

}

