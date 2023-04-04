#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H

#include "Include/Elements/element.h"

class Multiplexer final : public Element {
public:
    static constexpr char const *const TYPE{ "logic/multiplexer_int" };
    static constexpr string::hash_t const HASH{ string::hash(TYPE) };

    Multiplexer();

    char const *type() const noexcept override { return TYPE; }
    string::hash_t hash() const noexcept override { return HASH; }

    void calculate() override;
};


#endif // MULTIPLEXER_H
