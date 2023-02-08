#include "Include/Elements/Gates/or.h"

OrGate::OrGate(bool input_1, bool input_2)
    : Gate(GateType::OR, input_1, input_2)
{
    calculate();
}

void OrGate::calculate()
{
    m_output = m_input_1 || m_input_2;
}

