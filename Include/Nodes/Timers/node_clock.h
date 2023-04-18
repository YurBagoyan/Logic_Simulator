#ifndef NODES_TIMERS_CLOCK_H
#define NODES_TIMERS_CLOCK_H

#include "Include/Nodes/node.h"

namespace nodes::timers {

class Clock : public Node {
    void showProperties() override;
};

} // namespace nodes::timers

#endif // NODES_TIMERS_CLOCK_H
