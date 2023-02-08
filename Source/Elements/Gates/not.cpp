#include "Include/Elements/Gates/not.h"

NotGate::NotGate(bool input)
    : Gate(GateType::NOT, input)
{
    calculate();
}

void NotGate::calculate()
{
    m_output = !m_input_1;
}

