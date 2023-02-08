#include "Include/Elements/Gates/xor.h"

XorGate::XorGate(bool input_1, bool input_2)
    : Gate(GateType::XOR, input_1, input_2)
{
    calculate();
}

void XorGate::calculate()
{
    m_output = m_input_1 ^ m_input_2;
}

