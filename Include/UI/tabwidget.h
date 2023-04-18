#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QObject>
#include <QTabWidget>
#include <QTabBar>
#include <QLineEdit>
#include <QEvent>

class MainWindow;

class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    TabWidget(QWidget* parent = nullptr);
    void setMainWindow(MainWindow* const mainWindow) { m_mainWindow = mainWindow; }
    void closeTab(const int index);
    void addNewTab(const QString tabName = "New Tab");
private slots:
    void createObject(const QString name, const QString iconPath, const QPointF pos);


private:
    MainWindow* m_mainWindow;
};

class TabBar : public QTabBar
{
    Q_OBJECT

public:
    TabBar(QWidget *parent = nullptr) : QTabBar(parent) { }


private:
    void mouseDoubleClickEvent(QMouseEvent *event);
};


#endif // TABWIDGET_H
