#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QObject>
#include <QListWidget>

#include <QMouseEvent>
#include <QByteArray>
#include <QDrag>
#include <QMimeData>

class ListWidget : public QListWidget
{
    Q_OBJECT
public:
    ListWidget(QWidget* parent = nullptr);
    using QListWidget::QListWidget;

private:
    void createDrag(const QPoint &pos, QListWidgetItem* item);

protected:
    void mousePressEvent(QMouseEvent* event) override;
};

#endif // LISTWIDGET_H
