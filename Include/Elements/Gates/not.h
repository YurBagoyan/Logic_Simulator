#ifndef NOTGATE_H
#define NOTGATE_H

#include "Include/Elements/element.h"

class Not : public Element
{
public:
    Not();

    static constexpr char const *const TYPE{ "gates/not" };
    static constexpr string::hash_t const HASH{ string::hash(TYPE) };

    char const *type() const noexcept override { return TYPE; }
    string::hash_t hash() const noexcept override { return HASH; }

    void calculate() override;

};

#endif // NOTGATE_H
