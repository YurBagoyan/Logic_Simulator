#include "Include/Elements/Logic/multiplexer.h"

Multiplexer::Multiplexer()
  : Element{}
{
    setMinInputs(3);
    setMinOutputs(1);
    setMaxOutputs(1);

    addInput(ValueType::Int, "Select", IOSocket::CanHoldInt);
    addInput(ValueType::Int, "#1", IOSocket::CanHoldInt | IOSocket::CanChangeName);
    addInput(ValueType::Int, "#2", IOSocket::CanHoldInt | IOSocket::CanChangeName);

    addOutput(ValueType::Int, "Value", IOSocket::CanHoldInt);

    setDefaultNewInputFlags(IOSocket::CanHoldInt | IOSocket::CanChangeName);
}

void Multiplexer::calculate()
{
    int32_t const SELECT = std::get<int32_t>(m_inputs[0].value);
    int32_t const SIZE = static_cast<int32_t>(m_inputs.size()) - 2;
    int32_t const INDEX = std::clamp<int32_t>(SELECT, 0, SIZE);
    int32_t const VALUE = std::get<int32_t>(m_inputs[static_cast<size_t>(INDEX) + 1].value);

    m_outputs[0].value = VALUE;
}
