#include "Include/Elements/Ui/bcdtosevensegmentdisplay.h"

namespace elements::Ui {

BCDToSevenSegmentDisplay::BCDToSevenSegmentDisplay()
{
    setMinInputs(4);
    setMaxInputs(4);
    setMinOutputs(8);
    setMaxOutputs(8);

    addInput(ValueType::Bool, "A", IOSocket::CanHoldBool);
    addInput(ValueType::Bool, "B", IOSocket::CanHoldBool);
    addInput(ValueType::Bool, "C", IOSocket::CanHoldBool);
    addInput(ValueType::Bool, "D", IOSocket::CanHoldBool);

    addOutput(ValueType::Bool, "A", IOSocket::CanHoldBool);
    addOutput(ValueType::Bool, "B", IOSocket::CanHoldBool);
    addOutput(ValueType::Bool, "C", IOSocket::CanHoldBool);
    addOutput(ValueType::Bool, "D", IOSocket::CanHoldBool);
    addOutput(ValueType::Bool, "E", IOSocket::CanHoldBool);
    addOutput(ValueType::Bool, "F", IOSocket::CanHoldBool);
    addOutput(ValueType::Bool, "G", IOSocket::CanHoldBool);
}

void BCDToSevenSegmentDisplay::calculate()
{
    int32_t const A{ static_cast<int32_t>(std::get<bool>(m_inputs[0].value)) };
    int32_t const B{ static_cast<int32_t>(std::get<bool>(m_inputs[1].value)) };
    int32_t const C{ static_cast<int32_t>(std::get<bool>(m_inputs[2].value)) };
    int32_t const D{ static_cast<int32_t>(std::get<bool>(m_inputs[3].value)) };

    int32_t const VALUE{ (D << 3) | (C << 2) | (B << 1) | A };

    switch (VALUE) {
        case 0: setOutputs(true, true, true, true, true, true, false); break;
        case 1: setOutputs(false, true, true, false, false, false, false); break;
        case 2: setOutputs(true, true, false, true, true, false, true); break;
        case 3: setOutputs(true, true, true, true, false, false, true); break;
        case 4: setOutputs(false, true, true, false, false, true, true); break;
        case 5: setOutputs(true, false, true, true, false, true, true); break;
        case 6: setOutputs(true, false, true, true, true, true, true); break;
        case 7: setOutputs(true, true, true, false, false, false, false); break;
        case 8: setOutputs(true, true, true, true, true, true, true); break;
        case 9: setOutputs(true, true, true, true, false, true, true); break;
        default: setOutputs(false, false, false, false, false, false, false); break;
  }
}

void BCDToSevenSegmentDisplay::setOutputs(bool const A, bool const B, bool const C, bool const D,
                                          bool const E, bool const F, bool const G)
{
    m_outputs[0].value = A;
    m_outputs[1].value = B;
    m_outputs[2].value = C;
    m_outputs[3].value = D;
    m_outputs[4].value = E;
    m_outputs[5].value = F;
    m_outputs[6].value = G;
}

} // namespace elements::Ui
