#ifndef NODES_PACKAGE_H
#define NODES_PACKAGE_H

#include "Include/Nodes/node.h"

namespace nodes {

class NodePackage : public Node
{
public:
    NodePackage();

    void setInputsNode(Node *const node) { m_inputsNode = node; }
    Node *inputsNode() { return m_inputsNode; }
    Node const *inputsNode() const { return m_inputsNode; }

    void setOutputsNode(Node *const node) { m_outputsNode = node; }
    Node *outputsNode() { return m_outputsNode; }
    Node const *outputsNode() const { return m_outputsNode; }

private:
    void showProperties() override;
    void handleEvent(Event const &event) override;
    bool open() override;

private:
    Node *m_inputsNode{};
    Node *m_outputsNode{};
};

} // namespace Nodes

#endif // NODES_PACKAGE_H
