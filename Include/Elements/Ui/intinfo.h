#ifndef ELEMENTS_UI_INTINFO_H
#define ELEMENTS_UI_INTINFO_H

#include "Include/Elements/element.h"

namespace elements::Ui {

class IntInfo : public Element {
public:
    static constexpr char const *const TYPE{ "values/int_info" };
    static constexpr string::hash_t const HASH{ string::hash(TYPE) };

    IntInfo();

    char const *type() const noexcept override { return TYPE; }
    string::hash_t hash() const noexcept override { return HASH; }
};

} // namespace elements::Ui

#endif // ELEMENTS_UI_INTINFO_H
