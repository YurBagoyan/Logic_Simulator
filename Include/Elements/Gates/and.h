#ifndef AND_H
#define AND_H

#include "Include/Elements/element.h"

class And : public Element
{
public:
    And();

    static constexpr char const *const TYPE{ "gates/and" };
    static constexpr string::hash_t const HASH{ string::hash(TYPE) };

    char const *type() const noexcept override { return TYPE; }
    string::hash_t hash() const noexcept override { return HASH; }

    void calculate() override;
};

#endif // AND_H
