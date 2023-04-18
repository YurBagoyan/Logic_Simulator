#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QAbstractListModel>
#include <QHash>
#include <QTimer>

#include "Include/package.h"
#include "Include/Nodes/nodePackage.h"


namespace nodes {
class NodePackage;
}

class MainWindow;
class Node;
class Package;
class LinkItem;

class TableWidget;
class QListView;
class QSortFilterProxyModel;

class NodesListModel : public QAbstractListModel {
  Q_OBJECT

public:
    explicit NodesListModel(QObject *const parent);

    int rowCount(QModelIndex const &parent = QModelIndex()) const override;
    QVariant data(QModelIndex const &index, int role = Qt::DisplayRole) const override;

    void add(Node *const node);
    void remove(Node *const node);
    void update(Node *const node);

    Node *nodeFor(QModelIndex const &index);

private:
    QList<Node *> m_nodes{};
};

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    using Nodes = QHash<size_t, Node*>;
    GraphicsView(MainWindow* const mainWindow  = nullptr, Package* const package = nullptr);
    ~GraphicsView();

    void open();

    void wheelEvent(QWheelEvent *event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void drawBackground(QPainter *painter, QRectF const &arect) override;

    void acceptDragLink() { m_dragLink = nullptr; }
    void cancelDragLink();

    bool canClose();
    void center();
    bool snapToGrid() const { return m_snapToGrid; }

    void deleteElement();

    void updateName(Node *const node);
    void selectItem(QModelIndex const &index);

    void showProperties();

public:  /// Geters and setters
    void setScene(QGraphicsScene* scene);
    void setDragLink(LinkItem *link) { m_dragLink = link; }
    void setSelectedNode(Node *const node);


    QGraphicsScene* scene() const;
    Node *getNode(size_t const id) const { return m_nodes[id]; }
    Nodes const &nodes() const { return m_nodes; }
    Nodes &nodes() { return m_nodes; }
    LinkItem *dragLink() const { return m_dragLink; }

    MainWindow const* mainWindow() const { return m_mainWindow; }
    MainWindow* mainWindow() { return m_mainWindow; }
    Package const *package() const { return m_package; }
    Package *package() { return m_package; }

    NodesListModel *model() const { return m_nodesModel; }
    QSortFilterProxyModel *proxyModel() const { return m_nodesProxyModel; }

signals:
    void itemDrop(const QString name, const QString iconPath, const QPointF pos);

private:
    void setGraphicsView();
    void updateGrid(qreal const scale);

private:
    MainWindow *const m_mainWindow{};
    QListView *const m_elements{};
    TableWidget * m_properties{};
    NodesListModel *const m_nodesModel{};
    QSortFilterProxyModel *const m_nodesProxyModel{};
    Package *const m_package{};
    Nodes m_nodes{};
    QGraphicsScene* m_scene{};
    QTimer m_timer{};

    Node *const m_inputs{};
    Node *const m_outputs{};

    nodes::NodePackage* m_packageNode{};
    Node *m_dragNode{};
    Node *m_selectedNode{};
    LinkItem *m_dragLink{};

    int32_t m_scheduledScalings{};
    enum class GridDensity { Large, Small } m_gridDensity{};
    QString m_filename{};
    bool m_snapToGrid{};
    bool m_standalone{};
};
#endif // GRAPHICSVIEW_H
