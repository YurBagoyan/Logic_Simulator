#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QRect>


class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget* parent = nullptr);
    ~GraphicsView();

private:
    void setupView();

    virtual void wheelEvent(QWheelEvent* event) override;

private:
    QGraphicsScene* m_scene;

};

#endif // GRAPHICSVIEW_H
