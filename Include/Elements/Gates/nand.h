#ifndef NANDGATE_H
#define NANDGATE_H

#include "Include/Elements/gate.h"

class NandGate : public Gate
{
public:
    NandGate(bool input_1 = false, bool input_2 = false);

    void calculate() override;

};
#endif // NANDGATE_H
