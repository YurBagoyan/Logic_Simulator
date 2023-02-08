#ifndef EXPANDERWIDGET_H
#define EXPANDERWIDGET_H

#include <QFrame>
#include <QGridLayout>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>
#include <QTreeWidget>

class ExpanderWidget : public QWidget {
    Q_OBJECT

public:
    explicit ExpanderWidget(QWidget *parent = 0, const QString & title = "", const int animationDuration = 300);
    void setContentLayout(QLayout & contentLayout);

private:
    QGridLayout mainLayout;
    QToolButton toggleButton;
    QFrame headerLine;
    QParallelAnimationGroup toggleAnimation;
    QScrollArea contentArea;
    int animationDuration{300};

public:
    void setTitle(const QString title) {
        toggleButton.setText(title);
    }

};

#endif // EXPANDERWIDGET_H
