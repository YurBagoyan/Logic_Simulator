#include "Include/mainwindow.h"

#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QPushButton>
#include <QFile>
#include <QHeaderView>

#include <QGraphicsScene>
#include <QGridLayout>
#include <QKeyEvent>
#include <QToolBar>
#include <QAction>


#include "Include/Nodes/node.h"
#include "Include/Elements/Gates/all.h"
#include "Include/Elements/Timers/clock.h"
#include "Include/Elements/Logic/demultiplexer.h"
#include "Include/Elements/Logic/multiplexer.h"

Element* getElement(const QString name)
{

    if(name == "AND")
        return new And();
    else if(name == "NAND")
        return new Nand();
    else if(name == "NOR")
        return new Nor();
    else if(name == "NOT")
        return new Not();
    else if(name == "OR")
        return new Or;
    else if(name == "XNOR")
        return new Xnor;
    else if(name == "XOR")
        return new Xor;
    else if(name == "CLOCK")
        return new Clock();
    else if(name == "MULTIPLEXER")
        return new Multiplexer();
    else if(name == "DEMULTIPLEXER")
        return new Demultiplexer();

    return nullptr;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    dialogWindow = new FirstDialogWindow();
    dialogWindow->exec();

    if(dialogWindow->isRejected()) {
        return;
    }

    m_projectName = dialogWindow->getProjectName();
    m_projectPath = dialogWindow->getProjectPath();

    // Setup mainwindow
    ui->setupUi(this);
    setWindowTitle(windowTitle() + " - " + m_projectName + "[*]");  // * for unsaved project
    ui->tabWidget->closeTab(ui->tabWidget->count() - 1);
    setupGraphicsView();

    // Setup dock widgets
    ui->libraryDockWidget->setAction(ui->actionLibrary);
    ui->proportiesDockWidget->setAction((ui->actionProperties));
    ui->usedElementsDockWidget_2->setAction(ui->actionUsed_Elements);

    // Setup tool bar
    toolBar = new ToolBar("Tool Bar", ui, m_graphicsView, this);
    this->addToolBar(toolBar);
    toolBar->setAction(ui->actionTool_bar);

    // Connections
    connect(m_graphicsView, SIGNAL(itemDrop(const QString, const QString, const QPointF)), this, SLOT(createObject(const QString, const QString, const QPointF)));

    populateLibrary();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete dialogWindow;
}

void MainWindow::populateLibrary()
{
    // Setup expander widgets
    gatesList = new ListWidget();
    timerList = new ListWidget();
    logicList = new ListWidget();

    timerList->setMinimumSize(0, 100);
    logicList->setMinimumSize(0, 200);

    setItems();
    addElementsToLists();

    ui->expWidget->addItem(gatesList, "Gates");
    ui->expWidget->addItem(timerList, "Timer");
    ui->expWidget->addItem(logicList, "Logic");
}

void MainWindow::setItems()
{
    andButton = createItem("AND", QIcon(QPixmap(":Docs/GateIcons/AND.ico")));
    orButton = createItem("OR", QIcon(QPixmap(":Docs/GateIcons/OR.ico")));
    notButton = createItem("NOT", QIcon(QPixmap(":Docs/GateIcons/NOT.ico")));
    nandButton = createItem("NAND", QIcon(QPixmap(":Docs/GateIcons/NAND.ico")));
    norButton = createItem("NOR", QIcon(QPixmap(":Docs/GateIcons/NOR.ico")));
    xnorButton = createItem("XNOR", QIcon(QPixmap(":Docs/GateIcons/XNOR.ico")));
    xorButton = createItem("XOR", QIcon(QPixmap(":Docs/GateIcons/XOR.ico")));

    clockButton = createItem("CLOCK", QIcon(QPixmap(":Docs/GateIcons/CLOCK.ico")));

    muxButton = createItem("MULTIPLEXER", QIcon(QPixmap(":Docs/LogicIcons/MULTIPLEXER.ico")));
    demuxButton = createItem("DEMULTIPLEXER", QIcon(QPixmap(":Docs/LogicIcons/DEMULTIPLEXER.ico")));

}

void MainWindow::addElementsToLists()
{
    gatesList->addItem(andButton);
    gatesList->addItem(orButton);
    gatesList->addItem(notButton);
    gatesList->addItem(nandButton);
    gatesList->addItem(norButton);
    gatesList->addItem(xnorButton);
    gatesList->addItem(xorButton);

    timerList->addItem(clockButton);

    logicList->addItem(muxButton);
    logicList->addItem(demuxButton);

}

QListWidgetItem *MainWindow::createItem(const QString buttonText, const QIcon buttonIcon)
{
    QListWidgetItem* item = new QListWidgetItem(buttonIcon, buttonText);
    item->setTextAlignment(Qt::AlignCenter);

    QFont f;
    f.setBold(true);
    f.setItalic(true);
    item->setFont(f);

    return item;
}

void MainWindow::newPackage()
{
    auto const package = new Package;
    openOrCreateGraphicsView(package);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(isProjectSaved()) {
        event->accept();
        QApplication::closeAllWindows();
    } else {
        event->ignore();
    }
}

int MainWindow::indexForGraphicsView(GraphicsView * const graphicsView) const
{
    auto const tabWidget = ui->tabWidget;
    auto const SIZE = tabWidget->count();
    for (int i = 0; i < SIZE; ++i) {
        auto packageView = qobject_cast<GraphicsView *const>(tabWidget->widget(i));
        if (packageView == graphicsView)
            return i;
    }
    return -1;
}

void MainWindow::setGraphicsViewTabName(const int index, const QString &name)
{
    auto const tabWidget = ui->tabWidget;
    tabWidget->setTabText(index, name);
}

void MainWindow::openOrCreateGraphicsView(Package * const package)
{
    auto const FOUND = m_openPackages.constFind(package);

    if (FOUND != m_openPackages.constEnd())
        m_graphicsViewIndex = FOUND.value();
    else {
        auto const graphicsView = new GraphicsView{ nullptr, this, package };
        graphicsView->open();
        graphicsView->setSelectedNode(nullptr);
        graphicsView->showProperties();

//      connect(graphicsView, &GraphicsView::requestOpenFile,
//              [this](QString const filename) { openPackageFile(filename); });

        auto const IS_ROOT_PACKAGE = package->package() == nullptr;
        QString const TITLE{ (IS_ROOT_PACKAGE ? "New package" : package->name()) };
        m_graphicsViewIndex = ui->tabWidget->addTab(graphicsView, TITLE);
        m_openPackages[package] = m_graphicsViewIndex;
    }

    ui->tabWidget->setCurrentIndex(m_graphicsViewIndex);
}

void MainWindow::deleteElement()
{
    auto const graphicsView = graphicsViewForIndex(m_graphicsViewIndex);
    graphicsView->deleteElement();
}

TableWidget *MainWindow::propertiesTable()
{
    return ui->tableWidget;
}

void MainWindow::showEvent(QShowEvent *event)
{
    static bool s_firstTime{ true };

    if (s_firstTime) {
        s_firstTime = false;
        //newPackage();
    //  openPackage();

        auto const tab = ui->tabWidget;
        auto const index = tab->currentIndex();
        auto graphicsView = qobject_cast<GraphicsView *const>(tab->widget(index));
        //graphicsView->center();
    }

    QMainWindow::showEvent(event);
}

GraphicsView *MainWindow::graphicsViewForIndex(const int a_index) const
{
    auto const tabWidget = ui->tabWidget;
    auto const index = a_index == -1 ? tabWidget->currentIndex() : a_index;
    auto graphicsView = qobject_cast<GraphicsView *const>(tabWidget->widget(index));
    return graphicsView;
}

int MainWindow::openGraphicsViews() const
{
    return ui->tabWidget->count();
}

void MainWindow::showProperties(bool checked)
{
    ui->proportiesDockWidget->setVisible(checked);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Check if the pressed key is the "Delete" key
    if (event->key() == Qt::Key_Delete) {
        deleteElement();
    }
    // Pass the event to the base class to handle other key events
    QMainWindow::keyPressEvent(event);
}

void MainWindow::showLibrary(bool checked)
{
    ui->libraryDockWidget->setVisible(checked);
}

void MainWindow::createObject(const QString name, const QString iconPath, const QPointF pos)
{
    qDebug() << "Creating object" << name << "\t\tPos = " << pos;
    Node* node = new Node(iconPath);

    Element* element = getElement(name);
    node->setElement(element);
    node->setName(name);
    node->setPos(pos);
    m_graphicsView->scene()->addItem(node);
}

void MainWindow::on_actionNew_Project_triggered()
{
    if(isProjectSaved())
    {
        projectSaved = true;
        QString tempName = m_projectName;

        dialogWindow->on_newProjectButton_clicked();
        dialogWindow->exec();

        m_projectPath = dialogWindow->getProjectPath();
        m_projectName = dialogWindow->getProjectName();

        setWindowTitle(windowTitle().replace(tempName, m_projectName));

    }
}

void MainWindow::on_actionOpen_Project_triggered()
{
    if(isProjectSaved())
    {
        projectSaved = true;

        QString tempName = m_projectName;

        dialogWindow->on_openProjectButton_clicked();

        m_projectPath = dialogWindow->getProjectPath();
        m_projectName = dialogWindow->getProjectName();

        setWindowTitle(windowTitle().replace(tempName, m_projectName));

        // TODO: Load project from file
    }
}

void MainWindow::on_actionSave_Project_triggered()
{
    if (m_projectPath.isEmpty()) {
        on_actionSave_as_triggered();
    } else {
        saveProject(m_projectPath);
    }

    setWindowModified(false);
    projectSaved = true;
}

void MainWindow::on_actionSave_as_triggered()
{
    QString projectNewPath = QFileDialog::getExistingDirectory(this, tr("Save project"));
    saveProject(projectNewPath);
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About Application"),
             tr("The <b>Application</b> for Synopsys 2023"));
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QApplication::aboutQt();
}

void MainWindow::on_actionQuit_triggered()
{
    if(isProjectSaved()) {
        QMainWindow::close();
    }
}

void MainWindow::on_actionLibrary_triggered()
{
    if(ui->actionLibrary->isChecked()) {
        ui->libraryDockWidget->show();
    } else {
        ui->libraryDockWidget->hide();
    }
}

void MainWindow::on_actionProperties_triggered()
{
    if(ui->actionProperties->isChecked()) {
        ui->proportiesDockWidget->show();
    } else {
        ui->proportiesDockWidget->hide();
    }
}

void MainWindow::on_actionUsed_Elements_triggered()
{
    if(ui->actionUsed_Elements->isChecked()) {
        ui->usedElementsDockWidget_2->show();
    } else {
        ui->usedElementsDockWidget_2->hide();
    }
}

void MainWindow::on_actionTool_bar_triggered()
{
    if(ui->actionTool_bar->isChecked()) {
        toolBar->show();
    } else {
        toolBar->hide();
    }
}

bool MainWindow::isProjectSaved()
{
    if(projectSaved) {
       return true;
    }

    const QMessageBox::StandardButton act = QMessageBox::warning(this,tr("Aplication"),
                                                                 tr("The document has been modified.\n"
                                                                 "Do you want to save your changes?"),
                                                                 QMessageBox::Save | QMessageBox::Cancel);

    if (act == QMessageBox::Save) {
        on_actionSave_Project_triggered();
        return true;
    }

    return false;
}

void MainWindow::setupGraphicsView()
{
    m_graphicsView = new GraphicsView(ui->tab, this);
    m_graphicsView->setObjectName("graphicsView");
    m_graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_graphicsView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
    m_graphicsView->setSceneRect(QRectF(500, 500, 3000, 2000));
    m_graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    ui->gridLayout_3->addWidget(m_graphicsView, 0, 0, 1, 1);
}

void MainWindow::saveProject(const QString &path) const
{
    Q_UNUSED(path);
    // TODO: Write settings in file and save it
}
