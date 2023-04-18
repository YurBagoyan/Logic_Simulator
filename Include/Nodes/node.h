#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QVector>

#include "Include/Nodes/socketitem.h"
#include "Include/Elements/element.h"

class GraphicsView;
class TableWidget;

inline QString ValueType_to_QString(ValueType const a_type)
{
    switch (a_type) {
        case ValueType::Bool: return "Bool";
        case ValueType::Int: return "Int";
        case ValueType::Float: return "Float";
    }
    return "Unknown";
}

constexpr int NODE_TYPE{ QGraphicsItem::UserType + 1 };

class Node : public QGraphicsItem
{
public:
    using Sockets = QVector<SocketItem *>;
    using SocketType = SocketItem::Type;
    enum class Type { Element, Inputs, Outputs };
    enum class IOSocketsType { Inputs, Outputs };

    explicit Node(QGraphicsItem* parent = nullptr);
    ~Node();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *widget) override;
    QVariant itemChange(GraphicsItemChange change, QVariant const &value) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void advance(int phase) override;

    void paintBorder(QPainter *const painter);
    void paintIcon(QPainter *const painter);

    void calculateBoundingRect();

    virtual void showProperties();
    virtual void refreshCentralWidget() {}
    virtual void elementSet() {}
    virtual void handleEvent(Event const &event);
    virtual bool open() { return false; }

    void showCommonProperties();
    void showIOProperties(IOSocketsType const type);

    void changeInputName(int const id, QString const &name);
    void changeOutputName(int const id, QString const &name);
    void addSocket(SocketType const type, uint8_t const id, QString const &name, ValueType const valueType);
    void removeSocket(SocketType const type);
    void setSocketType(IOSocketsType const socketType, uint8_t const socketId, ValueType const type);

    void showName();
    void hideName();
    void iconify();
    void expand();

public: /// Geters and Seters
    void setType(Type const type) { m_type = type; }
    void setName(const QString& name);
    void setIcon(const QString& iconPath);
    void setElement(Element* const element);
    void setGraphicsView(GraphicsView* const graphicsView) { m_graphicsView = graphicsView; }
    void setPropertiesTable(TableWidget *const properties);

    int type() const override { return NODE_TYPE; }
    QString name() const { return m_name; }
    QString iconPath() const { return m_iconPath; }
    QPixmap icon() const { return m_icon; }
    GraphicsView* graphicsView() const { return m_graphicsView; }

    Element* element() const { return m_element; }
    Sockets const &inputs() const { return m_inputs; }
    Sockets const &outputs() const { return m_outputs; }

    void setPath(QString const &a_path) { m_path = a_path; }
    QString path() const { return m_path; }

protected:
    void setCentralWidget(QGraphicsItem *centralWidget);
    void propertiesInsertTitle(QString const &title);
    void changeIOName(IOSocketsType const type, int const id, QString const &name);

private:
    void addInput();
    void removeInput();
    void setInputName(uint8_t const socketId, QString const &name);

    void addOutput();
    void removeOutput();
    void setOutputName(uint8_t const socketId, QString const &name);

    void updateOutputs();

protected:
    QGraphicsItem* m_centralWidget{};
    Element* m_element{};
    GraphicsView* m_graphicsView{};
    TableWidget* m_properties{};

private:
    enum class Mode { Iconified, Expanded } m_mode{};

    Type m_type;
    bool m_showName{ true };
    QString m_name{};
    QString m_path{};
    QString m_iconPath{};
    QPixmap m_icon{};

    Sockets m_inputs{};
    Sockets m_outputs{};

    QRectF m_boundingRect;
    QPointF m_centralWidgetPosition;

    QFont m_nameFont;
};

#endif // NODE_H
