#ifndef BCD_H
#define BCD_H

#include "Include/Elements/element.h"
namespace elements::values  {

class BCD : public Element {
public:
    static constexpr char const *const TYPE{ "ui/bcd" };
    static constexpr string::hash_t const HASH{ string::hash(TYPE) };

    BCD();

    char const *type() const noexcept override { return TYPE; }
    string::hash_t hash() const noexcept override { return HASH; }

    void calculate() override;
};

} // namespace elements::math

#endif // BCD_H
