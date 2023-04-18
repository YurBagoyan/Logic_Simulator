#ifndef NODES_TOGGLE_BUTTON_H
#define NODES_TOGGLE_BUTTON_H

#include "Include/Nodes/node.h"

namespace nodes::Ui {

class ToggleButton : public Node {
public:
    ToggleButton();

private:
    void paint(QPainter *a_painter, QStyleOptionGraphicsItem const *a_option, QWidget *a_widget) override;

    void showProperties() override;
    void elementSet() override;
};


} // namespace nodes::Ui

#endif // NODES_TOGGLE_BUTTON_H
