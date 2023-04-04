#ifndef NORGATE_H
#define NORGATE_H

#include "Include/Elements/element.h"

class Nor : public Element
{
public:
    Nor();

    static constexpr char const *const TYPE{ "gates/nor" };
    static constexpr string::hash_t const HASH{ string::hash(TYPE) };

    char const *type() const noexcept override { return TYPE; }
    string::hash_t hash() const noexcept override { return HASH; }

    void calculate() override;

};

#endif // NORGATE_H
