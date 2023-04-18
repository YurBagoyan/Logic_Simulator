#include "Include/UI/graphicsview.h"

#include <QString>
#include <QMimeData>
#include <QPainter>
#include <QSortFilterProxyModel>
#include <QTimeLine>

#include "Include/Nodes/node.h"
#include "Include/Nodes/linkitem.h"
#include "Include/Nodes/nodePackage.h"
#include "Include/registry.h"
#include "Include/package.h"
#include "Include/mainwindow.h"
#include "Include/colors.h"
#include "Include/UI/tablewidget.h"

NodesListModel::NodesListModel(QObject *const parent)
  : QAbstractListModel{ parent }
{
}

int NodesListModel::rowCount(QModelIndex const &parent) const
{
    (void)parent;
    return m_nodes.count();
}

QVariant NodesListModel::data(QModelIndex const &index, int role) const
{
    if (!index.isValid()) return QVariant{};

    auto const node = m_nodes[index.row()];
    if (role == Qt::DecorationRole)
        return node->icon().scaled(QSize(50, 25));
    else if (role == Qt::DisplayRole)
        return QString("%1 (%2)").arg(node->name()).arg(node->element()->id());

    return QVariant{};
}

void NodesListModel::add(Node *const node)
{
    auto const ROW = rowCount();
    beginInsertRows(QModelIndex(), ROW, ROW);
    m_nodes.append(node);
    endInsertRows();
}

void NodesListModel::remove(Node *const node)
{
    auto const INDEX = m_nodes.indexOf(node);
    beginRemoveRows(QModelIndex(), INDEX, INDEX);
    m_nodes.removeAt(INDEX);
    endRemoveRows();
}

void NodesListModel::update(Node *const node)
{
    (void)node;
    auto const INDEX = m_nodes.indexOf(node);
    emit dataChanged(index(INDEX), index(INDEX));
}

Node *NodesListModel::nodeFor(QModelIndex const &index)
{
    if (!index.isValid()) return nullptr;
    auto const ROW = index.row();
    if (ROW < 0 || ROW > m_nodes.size()) return nullptr;

    return m_nodes[ROW];
}

////////////////   GraphicsView /////////////////////

GraphicsView::GraphicsView(MainWindow* const mainWindow, Package* const package)
    : QGraphicsView( new QGraphicsScene )
    , m_mainWindow { mainWindow }
    , m_elements{ mainWindow->usedElementsList() }
    , m_properties{ mainWindow->propertiesTable() }
    , m_nodesModel{ new NodesListModel{ this } }
    , m_nodesProxyModel{ new QSortFilterProxyModel{ this } }
    , m_package{ package }
    , m_scene{ scene() }
    , m_inputs{ new Node() }
    , m_outputs{ new Node() }
    , m_standalone{ m_package->package() == nullptr }
{

    if (m_standalone) {
        m_packageNode = static_cast<nodes::NodePackage *>(Registry::get().createNode("logic/package"));
        m_package->setNode(m_packageNode);
    } else {
        m_packageNode = m_package->node<nodes::NodePackage>();
        qDebug() << "Error: Else else: graphicsVirw: 97";
    }

    Q_ASSERT(m_package->node<Node*>());

    setGraphicsView();

    m_nodesProxyModel->setSourceModel(m_nodesModel);

    using NodeType = Node::Type;
    m_inputs->setType(NodeType::Inputs);
    m_inputs->setPos(m_package->inputsPosition().x, m_package->inputsPosition().y);
    m_inputs->setElement(m_package);
    m_inputs->setIcon(":Docs/LogicIcons/inputs.png");
    m_inputs->setGraphicsView(this);
    m_inputs->iconify();
    m_outputs->setType(NodeType::Outputs);
    m_outputs->setPos(m_package->outputsPosition().x, m_package->outputsPosition().y);
    m_outputs->setElement(m_package);
    m_outputs->setIcon(":Docs/LogicIcons/outputs.png");
    m_outputs->setGraphicsView(this);
    m_outputs->iconify();

    m_packageNode->setInputsNode(m_inputs);
    m_packageNode->setOutputsNode(m_outputs);
    m_packageNode->setElement(m_package);

    if (m_package->name().isEmpty()) { // <-----------------------
        auto &registry = Registry::get();
        m_package->setName(registry.elementName("logic/package"));
    }

    m_scene->addItem(m_inputs);
    m_scene->addItem(m_outputs);

    m_timer.setInterval(1000 / 60);

    connect(&m_timer, &QTimer::timeout, this, [this]() { m_scene->advance(); });
    m_timer.start();

    if (m_standalone) m_package->startDispatchThread();
}

GraphicsView::~GraphicsView()
{
    m_timer.stop();
    if (m_standalone) {
        m_package->quitDispatchThread();
        delete m_package;
    }
}

void GraphicsView::setGraphicsView()
{
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    setOptimizationFlags(QGraphicsView::DontSavePainterState | QGraphicsView::DontAdjustForAntialiasing);
    //setDragMode(QGraphicsView::ScrollHandDrag);

    setResizeAnchor(QGraphicsView::NoAnchor);
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    m_scene = new QGraphicsScene();
    m_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    m_scene->setSceneRect(500, 500, 3000, 2000);
    //m_scene->setBackgroundBrush(QPixmap(":/Images/GateIcons/background_white.png"));
    m_scene->setObjectName("GraphicsViewScene");

    m_inputs->setPropertiesTable(m_properties);
    m_outputs->setPropertiesTable(m_properties);
    m_packageNode->setPropertiesTable(m_properties);

    setAcceptDrops(true);
    setScene(m_scene);
}

void GraphicsView::open()
{
    auto const &inputsPosition = m_package->inputsPosition();
    auto const &outputsPosition = m_package->outputsPosition();
    m_inputs->setPos(inputsPosition.x, inputsPosition.y);
    m_outputs->setPos(outputsPosition.x, outputsPosition.y);

    Registry &registry{ Registry::get() };

    auto const &elements = m_package->elements();
    size_t const SIZE{ elements.size() };
    for (size_t i = 1; i < SIZE; ++i) {
        auto const element = elements[i];
        auto const node = registry.createNode(element->hash());
        auto const nodeName = registry.elementName(element->hash());
        auto const nodeIcon = registry.elementIcon(element->hash());
        auto const nodePath = QString::fromLocal8Bit(element->type());

        element->setNode(node);
        m_nodes[element->id()] = node;

        element->isIconified() ? node->iconify() : node->expand();
        node->setGraphicsView(this);
        node->setPropertiesTable(m_properties);
        node->setName(nodeName);
        node->setPath(nodePath);
        node->setIcon(nodeIcon);
        node->setPos(element->position().x, element->position().y);
        node->setElement(element);
        m_scene->addItem(node);

        m_nodesModel->add(node);
        m_nodesProxyModel->sort(0);
    }

    auto const &connections = m_package->connections();
    for (auto const &connection : connections) {
        auto const SOURCE_ID = connection.from_id;
        auto const SOURCE_SOCKET = connection.from_socket;
        auto const TARGET_ID = connection.to_id;
        auto const TARGET_SOCKET = connection.to_socket;

        auto const source = SOURCE_ID != 0 ? getNode(SOURCE_ID) : m_packageNode->inputsNode();
        auto const target = TARGET_ID != 0 ? getNode(TARGET_ID) : m_packageNode->outputsNode();
        auto const sourceSocket = source->outputs()[SOURCE_SOCKET];
        auto const targetSocket = target->inputs()[TARGET_SOCKET];
        sourceSocket->connect(targetSocket);
    }
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    // Set the zoom in limit
    const qreal zoomInLimit = 2.5;

    // Set the zoom out limit
    const qreal zoomOutLimit = 0.4;

    // Get the current scale factor
    qreal currentScaleFactor = transform().m11();

    // Calculate the new scale factor
    qreal newScaleFactor = pow((double)2, event->angleDelta().y() / 240.0) * currentScaleFactor;

    // Check if the new zoom factor is within the limits
    if (newScaleFactor < zoomOutLimit)
    {
        newScaleFactor = zoomOutLimit;
    }
    else if (newScaleFactor > zoomInLimit)
    {
        newScaleFactor = zoomInLimit;
    }

    // Apply the zoom
    scale(newScaleFactor / currentScaleFactor, newScaleFactor / currentScaleFactor);
    event->accept();
}

void GraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
    event->acceptProposedAction();

    auto const mimeData = event->mimeData();

    if (mimeData->hasFormat("metadata/name") && mimeData->hasFormat("metadata/icon")) {
        auto const isPackage = mimeData->data("metadata/is_package") == "true";
        auto const pathString = isPackage ? QString{ "logic/package" } : mimeData->text();
        auto const name = mimeData->data("metadata/name");
        auto const icon = mimeData->data("metadata/icon");
        auto const file = mimeData->data("metadata/filename");
        auto const stringData = pathString.toLatin1();
        auto const path = stringData.data();

        auto const DROP_POSITION = mapToScene(event->pos());

        Registry &registry{ Registry::get() };

        assert(m_dragNode == nullptr);
        m_dragNode = registry.createNode(path);
        m_dragNode->setGraphicsView(this);
        m_dragNode->setPropertiesTable(m_properties);
        m_dragNode->setName(name);
        m_dragNode->setPath(isPackage ? name : "");
        m_dragNode->setIcon(icon);
        m_dragNode->setPos(DROP_POSITION);
        m_scene->addItem(m_dragNode);
        m_dragNode->calculateBoundingRect();
        event->accept();
    } else
        QGraphicsView::dragEnterEvent(event);

}

void GraphicsView::dragLeaveEvent(QDragLeaveEvent *event)
{
    if (m_dragNode) {
        m_scene->removeItem(m_dragNode);
        delete m_dragNode;
        m_dragNode = nullptr;
    }

    event->accept();
}

void GraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
    event->acceptProposedAction();

    auto const dropPosition = mapToScene(event->position().toPoint());
    if (m_dragNode)
        m_dragNode->setPos(dropPosition);
    else if (m_dragLink) {
        QGraphicsView::dragMoveEvent(event);
        m_dragLink->setTo(mapToScene(event->position().toPoint()));
    }
}

void GraphicsView::dropEvent(QDropEvent *event)
{
    if (event->source() == this) {
        return;
    }

    auto const mimeData = event->mimeData();
    if (mimeData->hasFormat("metadata/name") && mimeData->hasFormat("metadata/icon")) {
        m_package->pauseDispatchThread();

        auto const isPackage = mimeData->data("metadata/is_package") == "true";
        auto const file = mimeData->data("metadata/filename");
        auto const pathString = event->mimeData()->text();
        auto const stringData = pathString.toLatin1();
        char const *const path{ isPackage ? "logic/package" : stringData.data() };

        auto const element = m_package->add(path);
        element->setNode(m_dragNode);

        if (element->name().isEmpty())
            element->setName(m_dragNode->name());


        if (isPackage) {
            //auto const package = static_cast<package::Package *>(element);
            //package->open(QString{ file });
        }

        m_dragNode->setElement(element);
        m_dragNode->iconify();
        m_dragNode->expand();

        m_nodes[element->id()] = m_dragNode;
        m_nodesModel->add(m_dragNode);
        m_nodesProxyModel->sort(0);

        m_dragNode = nullptr;

        m_package->resumeDispatchThread();
    }

    QGraphicsView::dropEvent(event);
}


void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    m_snapToGrid = event->modifiers() & Qt::ShiftModifier;
}

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    m_snapToGrid = event->modifiers() & Qt::ShiftModifier;

    int width = this->width();
    int height = this->height();

    if (event->key() == Qt::Key_R) {
        QPointF center{2000, 1500};
        centerOn(center);
    } else {
        QGraphicsView::keyReleaseEvent(event);
    }

    auto const selected = m_scene->selectedItems();
    for (auto &&item : selected) {
//        if (item->type() == NODE_TYPE) {
//        }
    }
}

void GraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    QBrush brush(get_color(Color::ViewBackground));
    painter->fillRect(rect, brush);

    QPen penNormal{ QColor(156, 156, 156, 255) };
    QPen penAxis{ QColor(156, 156, 156, 255) };

    qreal const LEFT{ rect.left() };
    qreal const RIGHT{ rect.right() };
    qreal const TOP{ rect.top() };
    qreal const BOTTOM{ rect.bottom() };
    qreal const GRID_DENSITY{ (m_gridDensity == GridDensity::Large ? 100.0 : 10.0) };

    qreal const START_X{ std::round(LEFT / GRID_DENSITY) * GRID_DENSITY };
    qreal const START_Y{ std::round(TOP / GRID_DENSITY) * GRID_DENSITY };

    if (m_gridDensity == GridDensity::Small) {
      penAxis.setWidth(2);
      penNormal.setWidth(2);
    }

    for (qreal x = START_X; x < RIGHT; x += GRID_DENSITY) {
        QPen const PEN{ ((x >= -0.1 && x <= 0.1) ? penAxis : penNormal) };
        painter->setPen(PEN);
        painter->drawLine(QPointF{ x, TOP }, QPointF{ x, BOTTOM });
    }

    for (qreal y = START_Y; y < BOTTOM; y += GRID_DENSITY) {
        QPen const PEN{ ((y >= -0.1 && y <= 0.1) ? penAxis : penNormal) };
        painter->setPen(PEN);
        painter->drawLine(QPointF{ LEFT, y }, QPointF{ RIGHT, y });
    }
}

void GraphicsView::cancelDragLink()
{
    delete m_dragLink;
    m_dragLink = nullptr;
}

bool GraphicsView::canClose()
{
    return true;
}

void GraphicsView::center()
{
    centerOn(0.0, 0.0);
}

void GraphicsView::deleteElement()
{
    auto selectedItems = m_scene->selectedItems();

    m_timer.stop();
    while (m_timer.isActive()) {
        // empty
    }

    for (auto &&item : selectedItems) {
        switch (item->type()) {
            case NODE_TYPE: {
                auto const node = reinterpret_cast<Node *>(item);
                auto const ID = node->element()->id();
                m_nodes.remove(ID);
                m_nodesModel->remove(node);
                m_nodesProxyModel->sort(0);

                if (node == m_selectedNode) setSelectedNode(nullptr);
                delete node;
                break;
            }
            case LINK_TYPE: {
                auto const link = reinterpret_cast<LinkItem *>(item);
                auto const from = link->from();
                auto const to = link->to();

                from->disconnect(to);
                break;
            }
        }
    }

    m_timer.start();
    showProperties();
}

void GraphicsView::updateName(Node *const node)
{
    m_nodesModel->update(node);
    m_nodesProxyModel->sort(0);
}

void GraphicsView::selectItem(QModelIndex const &index)
{
    auto const INDEX = m_nodesProxyModel->mapToSource(index);
    auto const node = m_nodesModel->nodeFor(INDEX);

    assert(node);

    scene()->clearSelection();

    node->setSelected(true);
    setSelectedNode(node);
    showProperties();

    centerOn(node);
}

void GraphicsView::showProperties()
{
    m_properties->clear();
    m_properties->setColumnCount(2);
    m_properties->setHorizontalHeaderLabels(QString("Name;Value").split(";"));

    if(!m_selectedNode) return;

    m_selectedNode->showProperties();
    m_properties->horizontalHeader()->setStretchLastSection(true);
}

void GraphicsView::setSelectedNode(Node *const node)
{
    if (node == nullptr) {
        m_selectedNode = m_packageNode;
    } else
        m_selectedNode = node;
}

void GraphicsView::updateGrid(qreal const scale)
{
    GridDensity const newDensity{ (scale >= 0.85 ? GridDensity::Large : GridDensity::Small) };

    if (newDensity == m_gridDensity)
        return;

    m_gridDensity = newDensity;
}

void GraphicsView::setScene(QGraphicsScene *scene)
{
    m_scene = scene;
    QGraphicsView::setScene(m_scene);
}

QGraphicsScene* GraphicsView::scene() const
{
    return m_scene;
}

