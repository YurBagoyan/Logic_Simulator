#include "Include/Elements/Gates/xnor.h"

XnorGate::XnorGate(bool input_1, bool input_2)
    : Gate(GateType::XNOR, input_1, input_2)
{
    calculate();
}

void XnorGate::calculate()
{
    m_output = !(m_input_1 ^ m_input_2);
}
