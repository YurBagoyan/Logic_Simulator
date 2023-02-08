#include "Include/UI/graphicsview.h"

GraphicsView::GraphicsView(QWidget* parent)
    : QGraphicsView(parent)
{
    m_scene = new QGraphicsScene(this);
    m_scene->setBackgroundBrush(QPixmap(":/Images/background.png"));

    setTransformationAnchor(AnchorUnderMouse);
    setupView();
    setScene(m_scene);
}

GraphicsView::~GraphicsView()
{
    delete m_scene;
}

void GraphicsView::setupView()
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    setSceneRect(500, 500, 3000, 2000);
}

#include <QRectF>

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    const double scaleFactor = 0.8;

    if(event->angleDelta().y() > 0) {   // Zoom in
         scale(1/scaleFactor, 1/scaleFactor);
    } else {                            // Zoom out
        scale(scaleFactor, scaleFactor);
    }

    event->accept();
}

