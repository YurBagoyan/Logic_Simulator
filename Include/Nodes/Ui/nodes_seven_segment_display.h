#ifndef NODES_UI_SEVENSEGMENTDISPLAY_H
#define NODES_UI_SEVENSEGMENTDISPLAY_H

#include "Include/Nodes/node.h"

namespace nodes::Ui {

class SevenSegmentDisplay : public Node {
public:
    SevenSegmentDisplay();

private:
    void refreshCentralWidget() override;
    void showProperties() override;

private:
    class SevenSegmentDisplayWidget *m_widget{};
};

}

#endif // NODES_UI_SEVENSEGMENTDISPLAY_H
