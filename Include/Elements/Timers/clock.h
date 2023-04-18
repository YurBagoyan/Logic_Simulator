#ifndef ELEMENTS_TIMERS_CLOCK_H
#define ELEMENTS_TIMERS_CLOCK_H

#include "Include/Elements/element.h"

namespace elements::timers {

class Clock : public Element
{
public:
    Clock();

    static constexpr char const *const TYPE{ "timers/clock" };
    static constexpr string::hash_t const HASH{ string::hash(TYPE) };

    char const *type() const noexcept override { return TYPE; }
    string::hash_t hash() const noexcept override { return HASH; }

    void reset() override { m_time = duration_t{ 0.0 }; }
    void update(duration_t const &a_delta) override;

    void setDuration(duration_t a_duration) { m_duration = a_duration; }

    duration_t duration() const { return m_duration; }

private:
    duration_t m_time{};
    duration_t m_duration{ 500 };
};

}


#endif // ELEMENTS_TIMERS_CLOCK_H
