#ifndef XORGATE_H
#define XORGATE_H

#include "Include/Elements/gate.h"

class XorGate : public Gate
{
public:
    XorGate(bool input_1 = false, bool input_2 = false);

    void calculate() override;

};

#endif // XORGATE_H
