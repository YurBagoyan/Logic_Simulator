#ifndef CONSTBOOL_H
#define CONSTBOOL_H

#include "Include/Elements/element.h"

namespace elements::values {


class ConstBool : public Element {
public:
    static constexpr char const *const TYPE{ "values/const_bool" };
    static constexpr string::hash_t const HASH{ string::hash(TYPE) };

    ConstBool();

    char const *type() const noexcept override { return TYPE; }
    string::hash_t hash() const noexcept override { return HASH; }

//  void serialize(Json &a_json) override;
//  void deserialize(Json const &a_json) override;

    void toggle();
    void set(bool a_state);

    bool currentValue() const { return m_currentValue; }

private:
    bool m_currentValue{};
};

} // namespace elements::values

#endif // CONSTBOOL_H
