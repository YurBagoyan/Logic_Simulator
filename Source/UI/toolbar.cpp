#include "Include/UI/toolbar.h"


ToolBar::ToolBar(const QString &title, Ui::MainWindow* ui, QWidget* parent)
    : QToolBar(parent)
    , m_ui(ui)
{
    setWindowTitle(title);
    setMovable(false);

    m_graphicsView = reinterpret_cast<GraphicsView*>(ui->tabWidget->currentWidget());

    setStyleSheet(  "QToolBar {"
                    "   background-color: #404142;"
                    "   border-bottom-style:  dotted;"
                    "   border-bottom: solid;"
                    "   border-width: 1px;"
                    "   border-color: #2e2f30;"
                    "}"

                    "QToolButton {"
                    "   background-color: #404142;"
                    "   padding: 3px;"
                    "   margin-left: 2px;"
                    "}"

                    "QToolButton:hover {"
                    "   background-color: #1d545c;"
                    "}" );

    setActions();
}

void ToolBar::setAction(QAction* action)
{
    menuAction = action;
}

void ToolBar::setActions()
{
    selectMode = new QAction(this);
    selectMode->setIcon(QIcon(":Docs/InterfaceIcons/Selection.ico"));
    selectMode->setShortcut(Qt::Key_V);
    connect(selectMode, &QAction::triggered, this, [this]() {
            auto graphicsView = reinterpret_cast<GraphicsView*>(m_ui->tabWidget->currentWidget());
            graphicsView->setDragMode(QGraphicsView::RubberBandDrag); });


    scrollHandMode = new QAction(this);
    scrollHandMode->setIcon(QIcon(":Docs/InterfaceIcons/Hand.ico"));
    scrollHandMode->setShortcut(Qt::Key_H);
    connect(scrollHandMode, &QAction::triggered, this, [this]() {
            auto graphicsView = reinterpret_cast<GraphicsView*>(m_ui->tabWidget->currentWidget());
            graphicsView->setDragMode(QGraphicsView::ScrollHandDrag); });


    addTab = new QAction(this);
    addTab->setIcon(QIcon(":Docs/InterfaceIcons/AddTab.ico"));
    connect(addTab, &QAction::triggered, this, &ToolBar::addNewTab);

    // Add actions in tool bar
    addAction(selectMode);
    addAction(scrollHandMode);
    addSeparator();
    addAction(addTab);

}

void ToolBar::addNewTab()
{
    m_ui->tabWidget->addNewTab();
}

void ToolBar::closeEvent(QCloseEvent*)
{
    menuAction->setChecked(false);
}

void ToolBar::showEvent(QShowEvent *)
{
    menuAction->setChecked(true);
}


