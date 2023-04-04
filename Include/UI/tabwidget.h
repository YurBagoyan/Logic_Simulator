#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QObject>
#include <QTabWidget>
#include <QTabBar>
#include <QLineEdit>
#include "qevent.h"


class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    TabWidget(QWidget* parent = nullptr);
    void closeTab(const int index);
    void addNewTab();
};

class TabBar : public QTabBar
{
    Q_OBJECT

public:
    TabBar(QWidget *parent = nullptr) : QTabBar(parent) { }

private:
    void mouseDoubleClickEvent(QMouseEvent *event);
};


#endif // TABWIDGET_H
