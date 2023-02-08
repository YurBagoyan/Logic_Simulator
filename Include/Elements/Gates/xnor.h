#ifndef XNORGATE_H
#define XNORGATE_H

#include "Include/Elements/gate.h"

class XnorGate : public Gate
{
public:
    XnorGate(bool input_1 = false, bool input_2 = false);

    void calculate() override;

};;

#endif // XNORGATE_H
