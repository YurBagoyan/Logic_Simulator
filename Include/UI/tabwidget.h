#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QObject>
#include <QTabWidget>
#include <QTabBar>
#include <QLineEdit>
#include <QEvent>


class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    TabWidget(QWidget* parent = nullptr);
    void closeTab(const int index);

};

class TabBar : public QTabBar
{
    Q_OBJECT

public:
    TabBar(QWidget *parent = nullptr) : QTabBar(parent) { }

    void mouseDoubleClickEvent(QMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
};


#endif // TABWIDGET_H
