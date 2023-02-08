#ifndef ORGATE_H
#define ORGATE_H

#include "Include/Elements/gate.h"

class OrGate : public Gate
{
public:
    OrGate(bool input_1 = false, bool input_2 = false);

    void calculate() override;

};

#endif // ORGATE_H
