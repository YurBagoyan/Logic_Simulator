#ifndef INTINFO_H
#define INTINFO_H

#include "Include/Nodes/node.h"

namespace nodes::Ui {

class IntInfo : public Node {
 public:
  IntInfo();

 private:
  void refreshCentralWidget() override;
  void showProperties() override;

 private:
  QGraphicsSimpleTextItem *m_info{};
};

} // namespace nodes::Ui

#endif // INTINFO_H
