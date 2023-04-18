#ifndef ELEMENTS_VALUES_OUTPUTBOOL_H
#define ELEMENTS_VALUES_OUTPUTBOOL_H


#include "Include/Elements/element.h"

namespace elements::values {

class OutputBool final : public Element {
public:
    static constexpr char const *const TYPE{ "values/output_widget" };
    static constexpr string::hash_t const HASH{ string::hash(TYPE) };

    OutputBool();

    char const *type() const noexcept override { return TYPE; }
    string::hash_t hash() const noexcept override { return HASH; }

    void toggle();
    void set(bool state);

    bool currentValue() const { return m_currentValue; }

private:
    bool m_currentValue{};
};

} // namespace elements::values

#endif // ELEMENTS_VALUES_OUTPUTBOOL_H
