#ifndef CONSTINT_H
#define CONSTINT_H

#include "Include/Elements/element.h"

namespace elements::values {

class ConstInt : public Element {
public:
    static constexpr char const *const TYPE{ "values/const_int" };
    static constexpr string::hash_t const HASH{ string::hash(TYPE) };

    ConstInt();

    char const *type() const noexcept override { return TYPE; }
    string::hash_t hash() const noexcept override { return HASH; }

//    void serialize(Json &a_json) override;
//    void deserialize(Json const &a_json) override;

    void set(int32_t a_value);

    int32_t currentValue() const { return m_currentValue; }

private:
    int32_t m_currentValue{};
};

} // namespace elements::values

#endif // CONSTINT_H
