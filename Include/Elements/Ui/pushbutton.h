#ifndef ELEMENTS_UI_PUSHBUTTON_H
#define ELEMENTS_UI_PUSHBUTTON_H

#include "Include/Elements/element.h"

namespace elements::Ui {

class PushButton final : public Element {
public:
    static constexpr char const *const TYPE{ "ui/push_button" };
    static constexpr string::hash_t const HASH{ string::hash(TYPE) };

    PushButton();

    char const *type() const noexcept override { return TYPE; }
    string::hash_t hash() const noexcept override { return HASH; }

    void toggle();
    void set(bool a_state);

    bool currentValue() const { return m_currentValue; }

private:
    bool m_currentValue{};
};

} // namespace elements::Ui

#endif // ELEMENTS_UI_PUSHBUTTON_H
