#ifndef ELEMENTS_VALUES_RANDOMBOOL_H
#define ELEMENTS_VALUES_RANDOMBOOL_H

#include "Include/Elements/element.h"

namespace elements::values {

class RandomBool final : public Element {
public:
    static constexpr char const *const TYPE{ "Trigger/random_bool" };
    static constexpr string::hash_t const HASH{ string::hash(TYPE) };

    RandomBool();

    char const *type() const noexcept override { return TYPE; }
    string::hash_t hash() const noexcept override { return HASH; }

    void calculate() override;

private:
    bool m_state{};
};

} // namespace elements::values

#endif // ELEMENTS_VALUES_RANDOMBOOL_H
