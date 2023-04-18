#ifndef NODES_CONSTINT_H
#define NODES_CONSTINT_H

#include "Include/Nodes/node.h"

namespace nodes::values {

class ConstInt : public Node {
public:
    ConstInt();

private:
    void refreshCentralWidget() override;
    void showProperties() override;

private:
    QGraphicsSimpleTextItem *m_info{};
};

} // namespace nodes::values

#endif // NODES_CONSTINT_H

