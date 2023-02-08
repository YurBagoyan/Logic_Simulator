#include "Include/UI/dockwidget.h"

DockWidget::DockWidget(QWidget* parent)
    : QDockWidget(parent)
{

}

void DockWidget::setAction(QAction* action)
{
    menuAction = action;
}

void DockWidget::closeEvent(QCloseEvent* event)
{
    Q_UNUSED(event);
    menuAction->setChecked(false);
}
