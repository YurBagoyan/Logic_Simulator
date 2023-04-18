#ifndef NODES_VALUES_OUTPUTBOOL_H
#define NODES_VALUES_OUTPUTBOOL_H

#include "Include/Nodes/node.h"

namespace nodes::values {

class OutputBool : public Node {
public:
    OutputBool();

private:
    void paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *widget) override;

    void refreshCentralWidget() override;
    void showProperties() override;
    void elementSet() override;

private:
    class OutputBoolWidget* m_widget{};
};

} // namespace nodes::values

#endif // NODES_VALUES_OUTPUTBOOL_H
