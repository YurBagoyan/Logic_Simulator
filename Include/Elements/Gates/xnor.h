#ifndef XNORGATE_H
#define XNORGATE_H

#include "Include/Elements/element.h"

class Xnor : public Element
{
public:
    Xnor();

    static constexpr char const *const TYPE{ "gates/xnor" };
    static constexpr string::hash_t const HASH{ string::hash(TYPE) };

    char const *type() const noexcept override { return TYPE; }
    string::hash_t hash() const noexcept override { return HASH; }

    void calculate() override;

};

#endif // XNORGATE_H
