#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QLayout>
#include <QGraphicsScene>

#include "Include/firstdialogwindow.h"
#include "Include/UI/toolbar.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool isAppReady() {
        return !dialogWindow->isRejected();
    }

private slots:
    void closeEvent(QCloseEvent *event) override;

    /// File menu
    void on_actionNew_Project_triggered();
    void on_actionOpen_Project_triggered();
    void on_actionSave_Project_triggered();
    void on_actionSave_as_triggered();
    void on_actionQuit_triggered();

    /// About menu
    void on_actionAbout_triggered();
    void on_actionAbout_Qt_triggered();

    // View menu
    void on_actionLibrary_triggered();
    void on_actionProperties_triggered();
    void on_actionUsed_Elements_triggered();

private:
    QLayout* createTempGatesLayout();
    QLayout* createTempLogicLayout();
    bool isProjectSaved();
    void saveProject(const QString &path) const;

private:
    FirstDialogWindow* dialogWindow;

    Ui::MainWindow *ui;
    //QGraphicsScene* scene;

    ToolBar* toolBar;

    QString m_projectName{"Unnamed"};
    QString m_projectPath;
    bool projectSaved{true};
};
#endif // MAINWINDOW_H
