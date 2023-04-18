#ifndef ELEMENTS_UI_BCD_TO_SEVEN_SEGMENTD_ISPLAY_H
#define ELEMENTS_UI_BCD_TO_SEVEN_SEGMENTD_ISPLAY_H

#include "Include/Elements/element.h"

namespace elements::Ui {

class BCDToSevenSegmentDisplay final : public Element {
public:
    static constexpr char const *const TYPE{ "ui/bcd_to_seven_segment_display" };
    static constexpr string::hash_t const HASH{ string::hash(TYPE) };

    BCDToSevenSegmentDisplay();

    char const *type() const noexcept override { return TYPE; }
    string::hash_t hash() const noexcept override { return HASH; }

    void calculate() override;

private:
    void setOutputs(bool const A, bool const B, bool const C, bool const D, bool const E, bool const F,
                    bool const G);
};

}

#endif // ELEMENTS_UI_BCD_TO_SEVEN_SEGMENTD_ISPLAY_H
