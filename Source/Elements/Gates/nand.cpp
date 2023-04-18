#include "Include/Elements/Gates/nand.h"
#include "qdebug.h"

namespace elements::gates {

Nand::Nand()
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

void Nand::calculate()
{
    bool result = true;
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
        if (!value) {
            result = false;
            break;
        }
    }    

    if(notConnected) {
        m_outputs[0].value = false;
        return;
    }

    m_outputs[0].value = !result;
}

}
