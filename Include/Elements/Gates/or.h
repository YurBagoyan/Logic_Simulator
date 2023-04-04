#ifndef ORGATE_H
#define ORGATE_H

#include "Include/Elements/element.h"

class Or : public Element
{
public:
    Or();

    static constexpr char const *const TYPE{ "gates/or" };
    static constexpr string::hash_t const HASH{ string::hash(TYPE) };

    char const *type() const noexcept override { return TYPE; }
    string::hash_t hash() const noexcept override { return HASH; }

    void calculate() override;
};

#endif // ORGATE_H
