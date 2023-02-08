#ifndef NORGATE_H
#define NORGATE_H

#include "Include/Elements/gate.h"

class NorGate : public Gate
{
public:
    NorGate(bool input_1 = false, bool input_2 = false);

    void calculate() override;

};

#endif // NORGATE_H
