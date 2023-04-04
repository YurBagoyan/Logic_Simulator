#include "Include/Elements/Logic/demultiplexer.h"

Demultiplexer::Demultiplexer()
  : Element{}
{
    setMinInputs(2);
    setMaxInputs(2);
    setMinOutputs(2);

    addInput(ValueType::Int, "Select", IOSocket::CanHoldInt);
    addInput(ValueType::Int, "Value", IOSocket::CanHoldInt);

    addOutput(ValueType::Int, "#1", IOSocket::CanHoldInt | IOSocket::CanChangeName);
    addOutput(ValueType::Int, "#2", IOSocket::CanHoldInt | IOSocket::CanChangeName);

    setDefaultNewOutputFlags(IOSocket::CanHoldInt | IOSocket::CanChangeName);
}

void Demultiplexer::calculate()
{
    int32_t const SELECT =std::get<int32_t>(m_inputs[0].value);
    int32_t const VALUE = std::get<int32_t>(m_inputs[1].value);
    int32_t const SIZE = static_cast<int32_t>(m_outputs.size()) - 1;
    int32_t const INDEX = std::clamp<int32_t>(SELECT, 0, SIZE);

    for (auto &&output : m_outputs)
        output.value = 0;

    m_outputs[static_cast<size_t>(INDEX)].value = VALUE;
}
