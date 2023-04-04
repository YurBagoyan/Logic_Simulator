#ifndef DEMULTIPLEXER_H
#define DEMULTIPLEXER_H

#include "Include/Elements/element.h"

class Demultiplexer final : public Element {
public:
    static constexpr char const *const TYPE{ "logic/demultiplexer_int" };
    static constexpr string::hash_t const HASH{ string::hash(TYPE) };

    Demultiplexer();

    char const *type() const noexcept override { return TYPE; }
    string::hash_t hash() const noexcept override { return HASH; }

    void calculate() override;
};

#endif // DEMULTIPLEXER_H
