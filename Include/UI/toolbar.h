#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "ui_mainwindow.h"

#include <QObject>
#include <QToolBar>
#include <QAction>

class ToolBar : public QToolBar
{
    Q_OBJECT
public:
    ToolBar(const QString &title,  Ui::MainWindow* ui, QWidget* parent = nullptr);

private:
    void setActions();
    void addNewTab();

private:
    Ui::MainWindow* m_ui;

    QAction* selectMode;
    QAction* scrollHandMode;

    QAction* addTab;
};

#endif // TOOLBAR_H
