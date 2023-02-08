#include "Include/mainwindow.h"

#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QPushButton>
#include <QFile>
#include <QHeaderView>

#include <QGraphicsScene>
#include <QGridLayout>

#include <QToolBar>
#include <QAction>

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

    // Setup expander widgets
    ui->gatesExpWidget->setTitle("Gates");
    ui->gatesExpWidget->setContentLayout(*createTempGatesLayout());

    ui->logicExpWidget->setTitle("Logic");
    ui->logicExpWidget->setContentLayout(*createTempLogicLayout());

    ui->libraryDockWidget->setAction(ui->actionLibrary);
    ui->proportiesDockWidget->setAction((ui->actionProperties));
    ui->usedElementsDockWidget_2->setAction(ui->actionUsed_Elements);


    // Setup work area
//    scene = new QGraphicsScene(this);
//    scene->setBackgroundBrush(QPixmap(":/Images/background.png"));
//    ui->graphicsView->setScene(scene);


    toolBar = new ToolBar("Tool Bar", ui, this);
    this->addToolBar(toolBar);


}

MainWindow::~MainWindow()
{
    delete ui;
    delete dialogWindow;
    delete toolBar;
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

// Temp solution
QLayout* MainWindow::createTempGatesLayout()
{
    QPushButton* ORButton = new QPushButton("OR");
    QPushButton* ANDButton = new QPushButton("AND");
    QPushButton* NOTButton = new QPushButton("NOT");

    const QString style =   "QPushButton {"
                            "color: #ff9500;"
                            "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
                            "                  stop:0 rgba(71, 72, 72, 255), stop:0.458101 rgba(71, 72, 72, 255), "
                            "                  stop:0.575419 rgba(54, 54, 54, 255), stop:0.994413 rgba(54, 54, 54, 255));"
                            "border-style: outset;"
                            "border-width: 1px;"
                            "border-radius: 10px;"
                            "border-color: beige;"
                            "font: bold 14px;}"
                            ""
                            "QPushButton:hover {"
                            " background-color: gray;}";


    ORButton->setStyleSheet(style);
    ANDButton->setStyleSheet(style);
    NOTButton->setStyleSheet(style);

    QVBoxLayout* gatesLayout = new QVBoxLayout();
    gatesLayout->addWidget(ORButton);
    gatesLayout->addWidget(ANDButton);
    gatesLayout->addWidget(NOTButton);

    return gatesLayout->layout();
}

QLayout* MainWindow::createTempLogicLayout()
{
    QPushButton* clockButton = new QPushButton("Clock");
    QPushButton* multiplexerButton = new QPushButton("Multiplexer");
    QPushButton* demultiplexerButton= new QPushButton("Demultiplexer");


    const QString style =   "QPushButton {"
                            "color: #ff9500;"
                            "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
                            "                  stop:0 rgba(71, 72, 72, 255), stop:0.458101 rgba(71, 72, 72, 255), "
                            "                  stop:0.575419 rgba(54, 54, 54, 255), stop:0.994413 rgba(54, 54, 54, 255));"
                            "border-style: outset;"
                            "border-width: 1px;"
                            "border-radius: 10px;"
                            "border-color: beige;"
                            "font: bold 14px; }"
                            ""
                            "QPushButton:hover {"
                            "background-color: gray;}";

    clockButton->setStyleSheet(style);
    multiplexerButton->setStyleSheet(style);
    demultiplexerButton->setStyleSheet(style);

    QVBoxLayout* logicLayout = new QVBoxLayout();
    logicLayout->addWidget(clockButton);
    logicLayout->addWidget(multiplexerButton);
    logicLayout->addWidget(demultiplexerButton);

    return logicLayout->layout();
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
