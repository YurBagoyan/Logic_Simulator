#include "Include/Elements/Timers/clock.h"

namespace elements::timers {

Clock::Clock()
{
    setMinInputs(0);
    setMaxInputs(0);
    setMinOutputs(1);
    setMaxOutputs(1);

    addOutput(ValueType::Bool, "Output", IOSocket::CanHoldBool);
}

void Clock::update(duration_t const &a_delta)
{
    m_time += a_delta;
    if (m_time >= m_duration) {
        bool const VALUE = !std::get<bool>(m_outputs[0].value);
        m_outputs[0].value = VALUE;
        reset();
    }
}

} // namespace elements::timers

