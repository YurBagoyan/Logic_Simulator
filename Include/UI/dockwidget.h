#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QObject>
#include <QDockWidget>
#include <QAction>

class DockWidget : public QDockWidget
{
    Q_OBJECT
public:
    DockWidget(QWidget* parent = nullptr);

    void setAction(QAction* action);
private:
    void closeEvent(QCloseEvent* event);
    void showEvent(QShowEvent* event);


private:
    QAction* menuAction{nullptr};
};

#endif // DOCKWIDGET_H
