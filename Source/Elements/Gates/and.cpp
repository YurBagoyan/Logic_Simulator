#include "Include/Elements/Gates/and.h"

AndGate::AndGate(bool input_1, bool input_2)
    : Gate(GateType::AND, input_1, input_2)
{
    calculate();
}

void AndGate::calculate()
{
    m_output = m_input_1 && m_input_2;
}


