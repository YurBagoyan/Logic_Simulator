#include "Include/UI/dockwidget.h"

DockWidget::DockWidget(QWidget* parent)
    : QDockWidget(parent)
{

}

void DockWidget::setAction(QAction* action)
{
    menuAction = action;
}

void DockWidget::closeEvent(QCloseEvent*)
{
    menuAction->setChecked(false);
}

void DockWidget::showEvent(QShowEvent*)
{
    menuAction->setChecked(true);
}
