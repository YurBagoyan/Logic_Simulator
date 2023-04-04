#ifndef NANDGATE_H
#define NANDGATE_H


#include "Include/Elements/element.h"

class Nand : public Element
{
public:
    Nand();

    static constexpr char const *const TYPE{ "gates/nand" };
    static constexpr string::hash_t const HASH{ string::hash(TYPE) };

    char const *type() const noexcept override { return TYPE; }
    string::hash_t hash() const noexcept override { return HASH; }

    void calculate() override;

};

#endif // NANDGATE_H
