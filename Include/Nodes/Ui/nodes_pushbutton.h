#ifndef NODES_UI_PUSHBUTTON_H
#define NODES_UI_PUSHBUTTON_H

#include "Include/Nodes/node.h"

namespace nodes::Ui {

class PushButton : public Node {
public:
    PushButton();

private:
    void paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *widget) override;

    void showProperties() override;
    void elementSet() override;
};

} // namespace nodes::Ui

#endif // NODES_UI_PUSHBUTTON_H
