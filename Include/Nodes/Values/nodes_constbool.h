#ifndef NODES_CONSTBOOL_H
#define NODES_CONSTBOOL_H

#include "Include/Nodes/node.h"

namespace nodes::values {

class ConstBool : public Node {
public:
    void showProperties() override;
};

} // namespace nodes::values

#endif // NODES_CONSTBOOL_H
