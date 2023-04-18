#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "ui_mainwindow.h"
#include "Include/UI/graphicsview.h"

#include <QObject>
#include <QToolBar>
#include <QAction>

class ToolBar : public QToolBar
{
    Q_OBJECT
public:
    ToolBar(const QString &title, Ui::MainWindow* ui, QWidget* parent = nullptr);

    void setAction(QAction* action);

private:
    void setActions();
    void addNewTab();

    void closeEvent(QCloseEvent* event);
    void showEvent(QShowEvent* event);

private:
    Ui::MainWindow* m_ui;
    GraphicsView* m_graphicsView;

    QAction* menuAction{nullptr};

    QAction* selectMode;
    QAction* scrollHandMode;

    QAction* addTab;
};

#endif // TOOLBAR_H
