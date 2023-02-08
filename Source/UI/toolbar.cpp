#include "Include/UI/toolbar.h"


ToolBar::ToolBar(const QString &title, Ui::MainWindow* ui, QWidget* parent)
    : QToolBar(parent)
    , m_ui(ui)
{
    setWindowTitle(title);
    setMovable(false);


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

void ToolBar::setActions()
{
    selectMode = new QAction(this);
    selectMode->setIcon(QIcon(":Icons/Selection.ico"));
    //selectMode->setShortcut(Qt::CTRL | Qt::Key_H);
    connect(selectMode, &QAction::triggered, this, [this]() {
            m_ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag); });


    scrollHandMode = new QAction(this);
    scrollHandMode->setIcon(QIcon(":Icons/Hand.ico"));
    scrollHandMode->setShortcut(Qt::CTRL | Qt::Key_H);
    connect(scrollHandMode, &QAction::triggered, this, [this]() {
            m_ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag); });


    addTab = new QAction(this);
    addTab->setIcon(QIcon(":Icons/AddTab.ico"));
    connect(addTab, &QAction::triggered, this, &ToolBar::addNewTab);

    // Add actions in tool bar
    addAction(selectMode);
    addAction(scrollHandMode);
    addSeparator();
    addAction(addTab);

}

void ToolBar::addNewTab()
{
    m_ui->tabWidget->addTab(new GraphicsView, "New Tab");

}
