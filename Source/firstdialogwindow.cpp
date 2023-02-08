#include "firstdialogwindow.h"
#include "ui_firstdialogwindow.h"

#include <QCloseEvent>
#include <QFileDialog>


FirstDialogWindow::FirstDialogWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FirstDialogWindow)
{
    ui->setupUi(this);

    QString dialogButtonsStyle ="QPushButton { "
                                "background-color: #242424;"
                                "color: #42b983;" //#dbdcdd;"
                                "border-color: #191919;"
                                "font: bold}"
                                ""
                                "QPushButton::hover {"
                                "background: #191919;}";

    ui->buttonBox->button(QDialogButtonBox::Ok)->setStyleSheet(dialogButtonsStyle);
    ui->buttonBox->button(QDialogButtonBox::Close)->setStyleSheet(dialogButtonsStyle);
}

FirstDialogWindow::~FirstDialogWindow()
{
    delete ui;
}

void FirstDialogWindow::closeEvent(QCloseEvent *event)
{
    closed = true;
    event->accept();
}


bool FirstDialogWindow::isRejected() const
{
    return closed;
}

QString FirstDialogWindow::getProjectName() const
{
    return projectName;
}

QString FirstDialogWindow::getProjectPath() const
{
    return projectPath;
}

void FirstDialogWindow::on_buttonBox_accepted()
{
    if(!ui->projectNameLine->text().isEmpty())
    {
        projectName = ui->projectNameLine->text();
    }

    return;
}

void FirstDialogWindow::on_buttonBox_rejected()
{
    closed = true;
}

void FirstDialogWindow::on_newProjectButton_clicked()
{
    projectPath = QFileDialog::getExistingDirectory(this, "New project directory");
    ui->projectPathLine->setText(projectPath);
}

void FirstDialogWindow::on_openProjectButton_clicked()
{
    QString temp = QFileDialog::getOpenFileName(this, "Open project", "*.txt");

    projectName = temp.split(QChar('/')).last().split(".").first();
    ui->projectNameLine->setText(projectName);

    projectPath = temp.remove("/" + projectName);
    ui->projectPathLine->setText(projectPath);
}

