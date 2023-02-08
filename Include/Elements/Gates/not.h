#ifndef NOTGATE_H
#define NOTGATE_H

#include "Include/Elements/gate.h"

class NotGate : public Gate
{
public:
    NotGate(bool input = false);

    void calculate() override;

};

#endif // NOTGATE_H
