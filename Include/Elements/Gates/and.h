#ifndef AND_H
#define AND_H

#include "Include/Elements/gate.h"

class AndGate : public Gate
{
public:
    AndGate(bool input_1 = false, bool input_2 = false);

    void calculate() override;
};

#endif // AND_H
