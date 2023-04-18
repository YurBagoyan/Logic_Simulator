#include "Include/Elements/Values/randombool.h"
#include <random>

namespace {
    std::random_device g_random{};
    std::mt19937 g_generator{ g_random() };
    std::bernoulli_distribution g_distrib(0.5);
} // namespace

namespace elements::values {

RandomBool::RandomBool()
  : Element{}
{
    setMinInputs(1);
    setMaxInputs(1);
    setMinOutputs(1);
    setMaxOutputs(1);

    addInput(ValueType::Bool, "Input", IOSocket::CanHoldBool);

    addOutput(ValueType::Bool, "Output", IOSocket::CanHoldBool);
}

void RandomBool::calculate()
{
    bool const STATE{ std::get<bool>(m_inputs[0].value) };

    if (STATE != m_state) {
        bool const VALUE{ g_distrib(g_generator) };
        m_outputs[0].value = VALUE;
        m_state = STATE;
    }
}

} // namespace elements::values
