#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include "Include/Elements/element.h"

namespace elements::Ui {

class ToggleButton final : public Element
{
public:
    static constexpr char const *const TYPE{ "ui/toggle_button" };
    static constexpr string::hash_t const HASH{ string::hash(TYPE) };

    ToggleButton();

    char const *type() const noexcept override { return TYPE; }
    string::hash_t hash() const noexcept override { return HASH; }

    void toggle();
    void set(bool a_state);

    bool currentValue() const { return m_currentValue; }

private:
    bool m_currentValue{};
};

} // namespace elements::Ui

#endif // TOGGLEBUTTON_H
