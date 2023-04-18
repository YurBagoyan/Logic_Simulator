#include "Include/mainwindow.h"
#include "ui_mainwindow.h"

#include <QSortFilterProxyModel>
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
#include <QShortcut>

#include "Include/Nodes/node.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

//    dialogWindow = new FirstDialogWindow();
//    dialogWindow->exec();

//    if(dialogWindow->isRejected()) {
//        return;
//    }

//    m_projectName = dialogWindow->getProjectName();
//    m_projectPath = dialogWindow->getProjectPath();

    // Setup mainwindow
    ui->setupUi(this);

    //setWindowTitle(windowTitle() + " - " + m_projectName + "[*]");  // * for unsaved project
    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->setMainWindow(this);
    ui->tabWidget->addNewTab("Main");

    // Setup dock widgets
    ui->libraryDockWidget->setAction(ui->actionLibrary);
    ui->proportiesDockWidget->setAction((ui->actionProperties));
    ui->usedElementsDockWidget_2->setAction(ui->actionUsed_Elements);

    // Setup tool bar
    toolBar = new ToolBar("Tool Bar", ui, this);
    this->addToolBar(toolBar);
    toolBar->setAction(ui->actionTool_bar);

    // Connections
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::tabChanged);

    connect(ui->usedElementsList, &QListView::doubleClicked, this, [this](QModelIndex const &index) {
        (void)index;

        auto const view = graphicsView();
        if (!view) return;

        view->selectItem(index);
    });

    connect(ui->searchLine, &QLineEdit::textChanged, [this](QString const &search) {
        //ui->clearSearchText->setDisabled(search.isEmpty());

        auto const view = graphicsView();
        if (!view) return;

        auto const model = view->proxyModel();
        model->setFilterWildcard(search);
    });

    auto const shortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_F), this);
    connect(shortcut, &QShortcut::activated, this, [this] { ui->searchLine->setFocus(); });


    // Setup used element list
    ui->usedElementsList->setSpacing(3);
    ui->usedElementsList->setUniformItemSizes(false);
    ui->usedElementsList->setIconSize(QSize(30, 30));
    auto const view = graphicsViewForIndex(0);
    ui->usedElementsList->setModel(reinterpret_cast<QAbstractItemModel*>(view->proxyModel()));
    populateLibrary();

    removeActions();
}

MainWindow::~MainWindow()
{
    delete ui;
    //delete dialogWindow;
}

void MainWindow::populateLibrary()
{
    // Setup expander widgets
//    gatesList = new ListWidget();
//    timerList = new ListWidget();
//    logicList = new ListWidget();

//    timerList->setMinimumSize(0, 100);
//    logicList->setMinimumSize(0, 200);

//    setItems();
//    addElementsToLists();

//    ui->expWidget->addItem(gatesList, "Gates");
//    ui->expWidget->addItem(timerList, "Timer");
//    ui->expWidget->addItem(logicList, "Logic");


    auto const &REGISTRY = Registry::get();

    auto const &ELEMENTS_SIZE = REGISTRY.size();
    for (size_t i = 0; i < ELEMENTS_SIZE; ++i) {
        auto const &info = REGISTRY.metaInfoAt(i);
        std::string const path =  info.type.toStdString();
        std::string category = path;

        if (auto const it = path.find_first_of('/'); it != std::string::npos) category = path.substr(0, it);
        category[0] = static_cast<char>(std::toupper(category[0]));

        if(info.name != "Package") {
            addElement(QString::fromStdString(category), info.name, info.type, info.icon);
        }
    }

    ui->expWidget->sortItems(0, Qt::AscendingOrder);

}

void MainWindow::removeActions()
{
    ui->menuFile->removeAction(ui->actionNew_Project);
    ui->menuFile->removeAction(ui->actionOpen_Project);
    ui->menuFile->removeAction(ui->actionSave_Project);
    ui->menuFile->removeAction(ui->actionSave_as);
}

void MainWindow::addElement(const QString &category, const QString &name, const QString &type, const QString &icon)
{
    ExpanderWidget *const library{ ui->expWidget };

    ListWidget* list{};

    int const count{ library->count() };
    for (int i = 0; i < count; ++i) {
        QString const text{ library->itemText(i) };
        if (text != category) continue;

        list = qobject_cast<ListWidget *>(library->widget(i));
        assert(list);
        break;
    }

    if (list == nullptr) {
        list = new ListWidget{ this };
        library->addItem(list, category);
    }

    QListWidgetItem *const item = new QListWidgetItem{ name };
    //item->setTextAlignment(Qt::AlignCenter);
    item->setToolTip(setToolTip(type));
    item->setData(ListWidget::MetaDataIsPackage, false);
    item->setData(ListWidget::MetaDataType, type);
    item->setData(ListWidget::MetaDataName, name);
    item->setData(ListWidget::MetaDataIcon, icon);
    item->setIcon(QIcon(icon));
    list->addItem(item);
    list->doResize();
    list->sortItems();

    list->setMainWindow(this);
}

QString MainWindow::setToolTip(QString const elementType)
{
    QString toolTip;

    if (elementType == "gates/and")
        toolTip = "AND Gate: An AND gate outputs a high signal (1) only when all of its inputs are high. It can be useful for combining multiple conditions in a logic circuit.";
    else if (elementType == "gates/nand")
        toolTip = "NAND Gate: A NAND gate is like an AND gate, but with a NOT gate on the output. It outputs a low signal (0) only when all of its inputs are high, and outputs a high signal otherwise.";
    else if (elementType == "gates/nor")
        toolTip = "NOR Gate: A NOR gate is like an OR gate, but with a NOT gate on the output. It outputs a high signal (1) only when all of its inputs are low, and outputs a low signal otherwise.";
    else if (elementType == "gates/not")
        toolTip = "NOT Gate: A NOT gate (also known as an inverter) outputs the opposite of its input. If the input is high (1), the output is low (0), and vice versa.";
    else if (elementType == "gates/or")
        toolTip = "OR Gate: An OR gate outputs a high signal (1) when any of its inputs are high. It can be useful for creating a logical ""or"" condition between two or more inputs.";
    else if (elementType == "gates/xnor")
        toolTip = "XNOR Gate: An XNOR gate (short for ""exclusive NOR"") is like an XOR gate, but with a NOT gate on the output. It outputs a high signal (1) only when both of its inputs are either high or low.";
    else if (elementType == "gates/xor")
        toolTip = "XOR Gate: An XOR gate (short for ""exclusive OR"") outputs a high signal (1) only when exactly one of its inputs is high. It can be useful for comparing two binary inputs.";
    else if (elementType == "timers/clock")
        toolTip = "Clock: A clock is a signal that is used to synchronize the timing of digital circuits. It generates a series of pulses at a fixed frequency, which is used to control the sequencing of operations within a circuit. Clocks are often used in digital systems to ensure that data is transferred at a consistent rate.";
    else if (elementType == "logic/multiplexer_int")
        toolTip = "Multiplexer: A multiplexer (also known as a MUX) is a digital circuit that selects one of several input signals and forwards the selected input to a single output. It can be thought of as a data selector that allows you to choose which input to pass through based on a control signal. Multiplexers can be used to switch between different sources of data or to combine multiple inputs into a single output.";
    else if (elementType == "logic/demultiplexer_int")
        toolTip = "Demultiplexer: A demultiplexer (also known as a DEMUX) is the opposite of a multiplexer - it takes a single input signal and directs it to one of several possible outputs based on a control signal. It can be thought of as a data distributor that allows you to route a single input to multiple outputs. Demultiplexers can be used to route data to different devices or components within a digital system.";
    else if (elementType == "values/int_info")
        toolTip = "Integer Output: An integer output widget is a user interface component that displays a numerical value as an integer.";
    else if (elementType == "ui/toggle_button")
        toolTip = "Boolean Toggle Button: A Boolean toggle button is a user interface component that allows the user to switch between two states (usually ""on"" or ""off"") by clicking or tapping on the button. It typically displays a visual indicator (such as a colored background or icon) to indicate the current state of the button. In digital systems, Boolean toggle buttons can be used to control the state of a circuit or signal.";
    else if (elementType == "ui/push_button")
        toolTip = "Boolean Push Button: A Boolean push button is a user interface component that allows the user to trigger an action by pressing and releasing the button. It typically displays a label or icon that describes the action that will be triggered when the button is pressed. In digital systems, Boolean push buttons can be used to initiate a process or sequence of operations within a circuit.";
    else if (elementType == "values/const_int")
        toolTip = "Integer Input: An integer input widget is a user interface component that allows the user to enter a numerical value as an integer.";
    else if (elementType == "ui/bcd")
        toolTip = "Binary Coded Decimal (BCD): BCD is a coding scheme used to represent decimal numbers using binary digits. In BCD, each decimal digit is represented by a four-bit binary code.";
    else if (elementType == "values/output_widget")
        toolTip = "Boolean Output: To make the output value more easily recognizable, it's often helpful to use color indicators to represent the two possible states. For example, green could represent ""true"" or ""on"", while red could represent ""false"" or ""off"". ";
    else if (elementType == "Trigger/random_bool")
        toolTip = "Trigger with Random Output: A trigger with random output is a circuit that generates a random output signal when triggered. ";


    return toolTip;
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

int MainWindow::indexForGraphicsView(GraphicsView* const graphicsView) const
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
        auto const graphicsView = new GraphicsView{this, package };
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
    return ui->propertiesTableWidget;
}

QListView *MainWindow::usedElementsList()
{
    return ui->usedElementsList;
}

GraphicsView* MainWindow::graphicsViewForIndex(const int a_index) const
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


void MainWindow::closeEvent(QCloseEvent *event)
{
    if(isProjectSaved()) {
        event->accept();
        QApplication::closeAllWindows();
    } else {
        event->ignore();
    }
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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Check if the pressed key is the "Delete" key
    if (event->key() == Qt::Key_Delete) {
        deleteElement();
    }
    // Pass the event to the base class to handle other key events
    QMainWindow::keyPressEvent(event);
}

void MainWindow::showProperties(bool checked)
{
    ui->proportiesDockWidget->setVisible(checked);
}

void MainWindow::showLibrary(bool checked)
{
    ui->libraryDockWidget->setVisible(checked);
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

void MainWindow::saveProject(const QString &path) const
{
    Q_UNUSED(path);
    // TODO: Write settings in file and save it
}

void MainWindow::tabChanged(const int index)
{
    m_graphicsViewIndex = index;
    if (index >= 0) {
        auto const view = graphicsViewForIndex(index);
        ui->usedElementsList->setModel(reinterpret_cast<QAbstractItemModel*>(view->proxyModel()));
    }
}

void MainWindow::on_actionDeleteElement_triggered()
{
    deleteElement();
}

