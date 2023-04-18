#include "Include/Elements/Ui/seven_segment_display.h"

namespace elements::Ui {

SevenSegmentDisplay::SevenSegmentDisplay()
{
    setMinInputs(8);
    setMaxInputs(8);
    setMinOutputs(0);
    setMaxOutputs(0);

    addInput(ValueType::Bool, "A", IOSocket::CanHoldBool);
    addInput(ValueType::Bool, "B", IOSocket::CanHoldBool);
    addInput(ValueType::Bool, "C", IOSocket::CanHoldBool);
    addInput(ValueType::Bool, "D", IOSocket::CanHoldBool);
    addInput(ValueType::Bool, "E", IOSocket::CanHoldBool);
    addInput(ValueType::Bool, "F", IOSocket::CanHoldBool);
    addInput(ValueType::Bool, "G", IOSocket::CanHoldBool);
    addInput(ValueType::Bool, "DP", IOSocket::CanHoldBool);
}

} // namespace elements::ui
