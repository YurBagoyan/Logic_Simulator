#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Include/firstdialogwindow.h"
#include "Include/UI/toolbar.h"
#include "Include/UI/listwidget.h"
#include "Include/package.h"
#include "Include/UI/graphicsview.h"

#include <QMainWindow>
#include <QCloseEvent>
#include <QLayout>
#include <QGraphicsScene>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    bool isAppReady() { return !dialogWindow->isRejected(); }

    void populateLibrary();
    void deleteElement();

    void openOrCreateGraphicsView(Package *const package);
    int openPackageViews() const;

public: // Getters and Setters
    void setGraphicsViewTabName(int const index, QString const &name);


    int graphicsViewIndex() const { return m_graphicsViewIndex; }
    int indexForGraphicsView(GraphicsView *const graphicsView) const;

    GraphicsView *packageView() const { return graphicsViewForIndex(m_graphicsViewIndex); }
    TableWidget* propertiesTable();

protected:
    void showEvent(QShowEvent *event) override;
    GraphicsView *graphicsViewForIndex(int const a_index = -1) const;
    int openGraphicsViews() const;

    void keyPressEvent(QKeyEvent *event) override;

    void showLibrary(bool checked);
    void showProperties(bool checked);

private: // Helpers
    void setItems();
    void addElementsToLists();
    QListWidgetItem* createItem(const QString buttonText, const QIcon buttonIcon);

    void newPackage();

    bool isProjectSaved();
    void setupGraphicsView();
    void saveProject(const QString &path) const;


private: // Memebrs
    FirstDialogWindow* dialogWindow;
    Ui::MainWindow *ui;

    GraphicsView* m_graphicsView;

    QString m_projectName{"Unnamed"};
    QString m_projectPath;
    bool projectSaved{true};

    ToolBar* toolBar;
    ListWidget* gatesList;
    ListWidget* timerList;
    ListWidget* logicList;

    QListWidgetItem* andButton;
    QListWidgetItem* orButton;
    QListWidgetItem* notButton;
    QListWidgetItem* nandButton;
    QListWidgetItem* norButton;
    QListWidgetItem* xnorButton;
    QListWidgetItem* xorButton;

    QListWidgetItem* clockButton;
    QListWidgetItem* muxButton;
    QListWidgetItem* demuxButton;

    int m_graphicsViewIndex{ -1 };
    QMap<Package*, int> m_openPackages{};

private slots:
    void createObject(const QString name, const QString iconPath, const QPointF pos);
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

    /// View menu
    void on_actionLibrary_triggered();
    void on_actionProperties_triggered();
    void on_actionUsed_Elements_triggered();
    void on_actionTool_bar_triggered();

};
#endif // MAINWINDOW_H
