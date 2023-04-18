#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QObject>
#include <QListWidget>

#include <QMouseEvent>
#include <QByteArray>
#include <QDrag>
#include <QMimeData>

class MainWindow;

class ListWidget : public QListWidget
{
    Q_OBJECT
public:
    enum {
        MetaDataType = Qt::UserRole, // type or path
        MetaDataName = Qt::UserRole + 1,
        MetaDataIcon = Qt::UserRole + 2,
        MetaDataIsPackage = Qt::UserRole + 3,
        MetaDataFilename = Qt::UserRole + 4,
    };

    ListWidget(QWidget* parent = nullptr);
    using QListWidget::QListWidget;

    void doResize();
    void setMainWindow(MainWindow* const mainWindow) { m_mainWindow = mainWindow; }

private:
    void createDrag(const QPoint &pos, QListWidgetItem* item);
    void startDrag(Qt::DropActions supportedActions) override;

protected:
   // void mousePressEvent(QMouseEvent* event) override;

private:
    MainWindow* m_mainWindow;
};

#endif // LISTWIDGET_H
