#include "Include/Elements/Gates/nand.h"

NandGate::NandGate(bool input_1, bool input_2)
    : Gate(GateType::NAND, input_1, input_2)
{
    calculate();
}

void NandGate::calculate()
{
    m_output = !(m_input_1 && m_input_2);
}
