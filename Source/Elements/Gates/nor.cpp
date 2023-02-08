#include "Include/Elements/Gates/nor.h"

NorGate::NorGate(bool input_1, bool input_2)
    : Gate(GateType::NOR, input_1, input_2)
{
    calculate();
}

void NorGate::calculate()
{
    m_output = m_input_1 || m_input_2;
}

