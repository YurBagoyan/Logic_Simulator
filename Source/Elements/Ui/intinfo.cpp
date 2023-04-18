#include "Include/Elements/Ui/intinfo.h"

namespace elements::Ui {

IntInfo::IntInfo()
{
    setMinInputs(1);
    setMaxInputs(1);
    setMinOutputs(0);
    setMaxOutputs(0);

    addInput(ValueType::Int, "Int", IOSocket::CanHoldInt | IOSocket::CanChangeName);
}

} // namespace elements::Ui
