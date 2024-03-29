#ifndef XORGATE_H
#define XORGATE_H

#include "Include/Elements/element.h"

namespace elements::gates {

class Xor : public Element
{
public:
    Xor();

    static constexpr char const *const TYPE{ "gates/xor" };
    static constexpr string::hash_t const HASH{ string::hash(TYPE) };

    char const *type() const noexcept override { return TYPE; }
    string::hash_t hash() const noexcept override { return HASH; }

    void calculate() override;

};

} // namespace elements::gates

#endif // XORGATE_H
