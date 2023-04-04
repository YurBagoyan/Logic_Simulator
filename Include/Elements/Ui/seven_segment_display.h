#ifndef SEVENSEGMENTDISPLAY_H
#define SEVENSEGMENTDISPLAY_H

#include "Include/Elements/element.h"

class SevenSegmentDisplay final : public Element {
public:
    static constexpr char const *const TYPE{ "ui/seven_segment_display" };
    static constexpr string::hash_t const HASH{ string::hash(TYPE) };

    SevenSegmentDisplay();

    char const *type() const noexcept override { return TYPE; }
    string::hash_t hash() const noexcept override { return HASH; }
};

#endif // SEVENSEGMENTDISPLAY_H
