#include "Include/Nodes/node.h"

#include <QApplication>
#include <QComboBox>

#include "Include/colors.h"
#include "Include/package.h"
#include "Include/UI/graphicsview.h"
#include "Include/UI/tablewidget.h"

constexpr int32_t const SOCKET_SIZE = SocketItem::SIZE;
qreal const ROUNDED_SOCKET_SIZE = std::round(static_cast<qreal>(SOCKET_SIZE) / 10.0) * 10.0;
qreal const ROUNDED_SOCKET_SIZE_2 = ROUNDED_SOCKET_SIZE / 2.0;

QString nodetype2string(Node::Type a_type)
{
  switch (a_type) {
    case Node::Type::Element: return "Element";
    case Node::Type::Inputs: return "Inputs";
    case Node::Type::Outputs: return "Outputs";
  }

  return "UNKNOWN TYPE";
}

bool value_type_allowed(uint8_t const a_flags, ValueType const a_type)
{
    switch (a_type) {
        case ValueType::Bool: return a_flags & Element::IOSocket::CanHoldBool;
        case ValueType::Int: return a_flags & Element::IOSocket::CanHoldInt;
        case ValueType::Float: return a_flags & Element::IOSocket::CanHoldFloat;
    }

    assert(false);
    qDebug() << "MY WARNING: Returned ValueType Int in line 24 node.cpp";
    return Element::IOSocket::CanHoldBool;;
}

ValueType first_available_type_for_flags(const uint8_t flags)
{
    if (flags & Element::IOSocket::CanHoldBool) return ValueType::Bool;
    if (flags & Element::IOSocket::CanHoldInt) return ValueType::Int;
    if (flags & Element::IOSocket::CanHoldFloat) return ValueType::Float;

    assert(false);
    qDebug() << "MY WARNING: Returned ValueType Int in line 35 node.cpp";
    return ValueType::Int;
}

Node::Node(QGraphicsItem* parent)
    : QGraphicsItem{parent}
{
    m_nameFont.setFamily("Consolas");
    m_nameFont.setPointSize(8);

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
    iconify();
}

Node::~Node()
{
    for (auto &input : m_inputs) input->disconnectAll();
    for (auto &output : m_outputs) output->disconnectAll();

    if (m_element) {
        auto const package = m_element->package();
        package->remove(m_element);
    }
}

QRectF Node::boundingRect() const
{
    return m_boundingRect;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    (void)option;
    (void)widget;

    paintBorder(painter);

    if (!m_centralWidget || !m_centralWidget->isVisible())  {
        //qDebug() << "Node.cpp line 73: m_centralWidget->isVisible()";
        paintIcon(painter);
    }
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    (void)change;
    (void)value;

    switch (change) {
        case ItemSelectedHasChanged: {
            Node *lastSelected{};
            auto selectedItems = scene()->selectedItems();
            for (auto &&item : selectedItems) {
                if (item->type() == NODE_TYPE)
                    lastSelected = static_cast<Node *>(item);
            }
            m_graphicsView->setSelectedNode(lastSelected);

            m_graphicsView->showProperties();
            break;
        }
        case ItemPositionChange: {
            QPointF const POSITION{ value.toPointF() };
            qreal const X{ std::round(POSITION.x() / 10.0) * 10.0 };
            qreal const Y{ std::round(POSITION.y() / 10.0) * 10.0 };
            return QPointF{ X, Y };
        }
        case ItemPositionHasChanged: {
            if (m_element) {
                QPointF const POSITION{ value.toPointF() };

                switch (m_type) {
                    case Type::Element: m_element->setPosition(POSITION.x(), POSITION.y()); break;
                    case Type::Inputs: {
                        auto const package = reinterpret_cast<Package *>(m_element);
                        package->setInputsPosition(POSITION.x(), POSITION.y());
                        break;
                    }
                    case Type::Outputs: {
                        auto const package = reinterpret_cast<Package *>(m_element);
                        package->setOutputsPosition(POSITION.x(), POSITION.y());
                        break;
                    }
                }
            }
            break;
        }
        default: break;
    }

    return QGraphicsItem::itemChange(change, value);
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    auto const MODIFIERS = QApplication::keyboardModifiers();
    if(!((MODIFIERS & Qt::ControlModifier) && open()))
        (m_mode == Mode::Iconified) ? expand() : iconify();

    QGraphicsItem::mouseDoubleClickEvent(event);
}

void Node::advance(int phase)
{
    if (!phase) return;

    updateOutputs();

    refreshCentralWidget();

    update();
}

void Node::paintBorder(QPainter *const painter)
{
    auto rect = boundingRect();

    QPen pen{};

    pen.setColor(get_color(Color::SocketBorder));
    pen.setWidth(2);
    QColor color{ 64, 65, 66, 220 }; // <--------------------
    QBrush brush{ color };
    painter->setPen(Qt::NoPen);
    painter->setBrush(brush);
    painter->drawRect(rect);

    if (m_showName) {
        QRectF nameRect{ 0.0, 0.0, m_boundingRect.width(), ROUNDED_SOCKET_SIZE };
        pen.setColor(get_color(Color::FontName));
        QColor nameBackground(get_color(Color::NameBackground));

        painter->setPen(Qt::NoPen);
        painter->setBrush(nameBackground);
        painter->drawRect(nameRect);

        pen.setColor(get_color(Color::FontName));
        painter->setFont(m_nameFont);
        painter->setPen(pen);

        QFontMetrics const METRICS{ m_nameFont };
        int const FONT_HEIGHT = METRICS.height();
        qreal const NAME_Y = (ROUNDED_SOCKET_SIZE / 2.0) + (FONT_HEIGHT - METRICS.strikeOutPos()) / 2.0 - 1.0;

        painter->drawText(QPointF{ 5.0, NAME_Y }, m_name);

    }

    pen.setColor(isSelected() ? QColor(156, 156, 156, 255) : QColor(58, 66, 71, 255));
    pen.setWidth(2);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(rect);
}

void Node::paintIcon(QPainter *const a_painter)
{
    auto const HALF_ICON_SIZE = m_icon.size() / 2;

    auto const Y = static_cast<int>(m_centralWidgetPosition.y());
    auto const WIDTH = HALF_ICON_SIZE.width();
    auto const HEIGHT = HALF_ICON_SIZE.height();
    a_painter->drawPixmap(static_cast<int>(m_centralWidgetPosition.x()), Y, WIDTH, HEIGHT, m_icon);
}

template<typename Container, class Comparator>
auto max_element(Container &container, Comparator comparator)
{
    return std::max_element(std::begin(container), std::end(container), comparator);
}


void Node::calculateBoundingRect()
{
    prepareGeometryChange();

    auto const INPUTS_COUNT = m_inputs.count();
    auto const OUTPUTS_COUNT = m_outputs.count();
    auto const SOCKETS_COUNT = std::max(INPUTS_COUNT, OUTPUTS_COUNT);
    auto const CENTRAL_SIZE =
        (m_centralWidget && m_centralWidget->isVisible()) ? m_centralWidget->boundingRect().size() : m_icon.size() / 2;
    auto const SOCKETS_HEIGHT = SOCKETS_COUNT * ROUNDED_SOCKET_SIZE;

    auto maxNameWidth = [](auto &&a_a, auto &&a_b) { return a_a->nameWidth() < a_b->nameWidth();};
    auto const LONGEST_INPUT = max_element(m_inputs, maxNameWidth);
    auto const LONGEST_OUTPUT = max_element(m_outputs, maxNameWidth);
    int const LONGEST_INPUTS_NAME_WIDTH = LONGEST_INPUT != std::end(m_inputs) ? (*LONGEST_INPUT)->nameWidth() : 0;
    int const LONGEST_OUTPUTS_NAME_WIDTH = LONGEST_OUTPUT != std::end(m_outputs) ? (*LONGEST_OUTPUT)->nameWidth() : 0;
    int const INPUTS_NAME_WIDTH = m_mode == Mode::Expanded ? LONGEST_INPUTS_NAME_WIDTH : 0;
    int const OUTPUTS_NAME_WIDTH = m_mode == Mode::Expanded ? LONGEST_OUTPUTS_NAME_WIDTH : 0;
    int const NAME_OFFSET = m_showName ? static_cast<int>(ROUNDED_SOCKET_SIZE_2) : 0;

    qreal width {CENTRAL_SIZE.width() };
    qreal height{};

    if (SOCKETS_HEIGHT > CENTRAL_SIZE.height())
        height = NAME_OFFSET + SOCKETS_HEIGHT + ROUNDED_SOCKET_SIZE;
    else {
        height = NAME_OFFSET + CENTRAL_SIZE.height() + ROUNDED_SOCKET_SIZE_2;
        if (SOCKETS_COUNT < 2) height += ROUNDED_SOCKET_SIZE_2;
    }

    width = ROUNDED_SOCKET_SIZE + INPUTS_NAME_WIDTH + CENTRAL_SIZE.width() + OUTPUTS_NAME_WIDTH + ROUNDED_SOCKET_SIZE;
    width = std::round(width / 10.0) * 10.0;
    height = std::round(height / 10.0) * 10.0;

    qreal const CENTRAL_X = ROUNDED_SOCKET_SIZE + INPUTS_NAME_WIDTH;
    qreal const CENTRAL_Y = NAME_OFFSET + (height / 2.0) - (CENTRAL_SIZE.height() / 2.0);
    m_centralWidgetPosition = QPointF{ CENTRAL_X, CENTRAL_Y };
    if (m_centralWidget) m_centralWidget->setPos(m_centralWidgetPosition);

    qreal yOffset{ ROUNDED_SOCKET_SIZE + NAME_OFFSET };
    for (auto &&input : m_inputs) {
        input->setPos(0.0, yOffset);
        yOffset += ROUNDED_SOCKET_SIZE;
    }

    yOffset = ROUNDED_SOCKET_SIZE + NAME_OFFSET;
    for (auto &&output : m_outputs) {
        output->setPos(width, yOffset);
        yOffset += ROUNDED_SOCKET_SIZE;
    }

    m_boundingRect = QRectF{ 0.0, 0.0, width, height };
}

void Node::showProperties()
{
    showCommonProperties();
    switch (m_type) {
      case Type::Element:
        showIOProperties(IOSocketsType::Inputs);
        showIOProperties(IOSocketsType::Outputs);
        break;
      case Type::Inputs: showIOProperties(IOSocketsType::Inputs); break;
      case Type::Outputs: showIOProperties(IOSocketsType::Outputs); break;
    }
}

void Node::handleEvent(Event const &event)
{
    switch (event.type) {
        case EventType::ElementNameChanged: break;
        case EventType::IOTypeChanged: break;
        case EventType::InputAdded: {
            auto const &INPUTS = m_element->inputs();
            auto const SIZE = inputs().size();
            auto const &INPUT = INPUTS.back();
            addSocket(SocketType::Input, static_cast<uint8_t>(SIZE), INPUT.name, INPUT.type);
            calculateBoundingRect();
            break;
        }
        case EventType::InputRemoved: {
            removeSocket(SocketType::Input);
            calculateBoundingRect();
            break;
        }
        case EventType::OutputAdded: {
            auto const &OUTPUTS = m_element->outputs();
            auto const SIZE = outputs().size();
            auto const &OUTPUT = OUTPUTS.back();
            addSocket(SocketType::Output, static_cast<uint8_t>(SIZE), OUTPUT.name, OUTPUT.type);
            calculateBoundingRect();
            break;
        }
        case EventType::OutputRemoved: {
            removeSocket(SocketType::Output);
            calculateBoundingRect();
            break;
        }
        case EventType::IONameChanged: break;
    }
}

void Node::showCommonProperties()
{
    m_properties->setRowCount(0);
    propertiesInsertTitle("Element");

    QTableWidgetItem *item{};
    int const ID{ static_cast<int>(m_element->id()) };
    QString const TYPE{ QString::fromLocal8Bit(m_element->type()) };
    QString const NAME{ m_element->name() };

    int row = m_properties->rowCount();
    m_properties->insertRow(row);
    item = new QTableWidgetItem{ "ID" };
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    m_properties->setItem(row, 0, item);

    item = new QTableWidgetItem{ ID };
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    item->setData(Qt::DisplayRole, ID);
    m_properties->setItem(row, 1, item);

    row = m_properties->rowCount();
    m_properties->insertRow(row);
    item = new QTableWidgetItem{ "Type" };
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    m_properties->setItem(row, 0, item);

    item = new QTableWidgetItem{ TYPE };
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    m_properties->setItem(row, 1, item);

    row = m_properties->rowCount();
    m_properties->insertRow(row);
    item = new QTableWidgetItem{ "Name" };
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    m_properties->setItem(row, 0, item);

    QLineEdit *nameEdit = new QLineEdit{ NAME };
    QString nameLineStyle =  "QLineEdit {"
                            "   background: #292a2b;"
                            "   color: #dbdcdd;"
                            "   border: transparent;"
                            "}";
    nameEdit->setStyleSheet(nameLineStyle);
    m_properties->setCellWidget(row, 1, nameEdit);
    QObject::connect(nameEdit, &QLineEdit::textChanged, [this](QString const &text) { setName(text); });
}

void Node::showIOProperties(const IOSocketsType type)
{
    bool const INPUTS{ type == IOSocketsType::Inputs };
    auto &ios = INPUTS ? m_element->inputs() : m_element->outputs();

    int const IOS_SIZE{ static_cast<int>(ios.size()) };
    uint8_t const MIN_IOS_SIZE{ INPUTS ? m_element->minInputs() : m_element->minOutputs() };
    uint8_t const MAX_IOS_SIZE{ INPUTS ? m_element->maxInputs() : m_element->maxOutputs() };
    bool const ADDING_DISABLED{ MIN_IOS_SIZE == MAX_IOS_SIZE };

    QTableWidgetItem *item{};
    int row = m_properties->rowCount();

    propertiesInsertTitle(INPUTS ? "Inputs" : "Outputs");

    row = m_properties->rowCount();
    m_properties->insertRow(row);
    item = new QTableWidgetItem{ "Count" };
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    m_properties->setItem(row, 0, item);

    auto const count = new QSpinBox;
    QString spinBoxStyle =  "QSpinBox{"
                            "    background: #292a2b;"
                            "   color: #dbdcdd;"
                            "   border: transparent;"
                            "}";
    count->setStyleSheet(spinBoxStyle);

    count->setRange(MIN_IOS_SIZE, MAX_IOS_SIZE);
    count->setValue(static_cast<int>(ios.size()));
    m_properties->setCellWidget(row, 1, count);
    QObject::connect(count, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [INPUTS, this](int a_value) {
        auto const SIZE = INPUTS ? m_element->inputs().size() : m_element->outputs().size();
        if (static_cast<int>(SIZE) < a_value)
            INPUTS ? addInput() : addOutput();
        else
            INPUTS ? removeInput() : removeOutput();
    });
    count->setDisabled(ADDING_DISABLED);

    for (int i = 0; i < IOS_SIZE; ++i) {
        row = m_properties->rowCount();
        m_properties->insertRow(row);

        auto const &IO = ios[static_cast<size_t>(i)];

        if (IO.flags & Element::IOSocket::CanChangeName) {
            QLineEdit *const ioName{ new QLineEdit{ IO.name } };
            QString lineStyle =  "QLineEdit {"
                                    "   background: #292a2b;"
                                    "   color: #dbdcdd;"
                                    "   border: transparent;"
                                    "}";
            ioName->setStyleSheet(lineStyle);

            QObject::connect(ioName, &QLineEdit::editingFinished, [type, i, ioName, this]() {
                m_element->setIOName(type == IOSocketsType::Inputs, static_cast<uint8_t>(i), ioName->text());
            });
            m_properties->setCellWidget(row, 0, ioName);
        } else {
            item = new QTableWidgetItem{ IO.name };
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            m_properties->setItem(row, 0, item);
        }

        auto const comboBox = new QComboBox;
        QString const comboBoxStyle = "QComboBox {"
                                      "     background: #292a2b;"
                                      "     color: #dbdcdd;"
                                      "     border: transparent;"
                                      "}";
        comboBox->setStyleSheet(comboBoxStyle);
        if (IO.flags & Element::IOSocket::CanHoldBool)
            comboBox->addItem(ValueType_to_QString(ValueType::Bool), static_cast<int>(ValueType::Bool));
        if (IO.flags & Element::IOSocket::CanHoldInt)
            comboBox->addItem(ValueType_to_QString(ValueType::Int), static_cast<int>(ValueType::Int));
        if (IO.flags & Element::IOSocket::CanHoldFloat)
            comboBox->addItem(ValueType_to_QString(ValueType::Float), static_cast<int>(ValueType::Float));
        int const INDEX{ comboBox->findData(static_cast<int>(IO.type)) };
        comboBox->setCurrentIndex(INDEX);
        m_properties->setCellWidget(row, 1, comboBox);
        QObject::connect(comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),
                       [type, i, comboBox, this](int a_index) {
                         ValueType const VALUE_TYPE{ static_cast<ValueType>(comboBox->itemData(a_index).toInt()) };
                         setSocketType(type, static_cast<uint8_t>(i), VALUE_TYPE);
                       });
    }
}

void Node::changeInputName(const int id, const QString &name)
{
    changeIOName(IOSocketsType::Inputs, id, name);
}

void Node::changeOutputName(const int id, const QString &name)
{
    changeIOName(IOSocketsType::Outputs, id, name);
}

void Node::propertiesInsertTitle(QString const &title)
{
    int const ROW = m_properties->rowCount();
    m_properties->insertRow(ROW);
    QTableWidgetItem *const item = new QTableWidgetItem{ title };
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    item->setBackground(QColor(64, 65, 66, 255));
    item->setForeground(QColor(219, 220, 221, 255));
    m_properties->setItem(ROW, 0, item);
    m_properties->setSpan(ROW, 0, 1, 2);
}

void Node::changeIOName(const IOSocketsType type, const int id, const QString &name)
{
    bool const INPUTS{ type == IOSocketsType::Inputs };

    SocketItem *socket{};
    if (m_type == Type::Element)
        socket = INPUTS ? m_inputs[id] : m_outputs[id];
    else
        socket = m_type == Type::Inputs ? m_outputs[id] : m_inputs[id];

    socket->setName(name);
    calculateBoundingRect();
}


void Node::iconify()
{
    if (m_element) {
        m_element->iconify(true);
        if (m_element->iconifyingHidesCentralWidget() && m_centralWidget) {
            m_centralWidget->hide();
        }
    }

    m_mode = Mode::Iconified;

    for (auto &&input : m_inputs) input->hideName();
    for (auto &&output : m_outputs) output->hideName();

    calculateBoundingRect();
}

void Node::expand()
{
    if (m_element) {
      m_element->iconify(false);
      if (m_element->iconifyingHidesCentralWidget() && m_centralWidget) m_centralWidget->show();
    }

    m_mode = Mode::Expanded;

    for (auto &&input : m_inputs) input->showName();
    for (auto &&output : m_outputs) output->showName();

    calculateBoundingRect();
}

void Node::addInput()
{
    uint8_t const SIZE{ static_cast<uint8_t>(m_element->inputs().size()) };
    QString const INPUT_NAME{ QString("#%1").arg(SIZE + 1) };

    ValueType const TYPE{ first_available_type_for_flags(m_element->defaultNewInputFlags()) };
    m_element->addInput(TYPE, INPUT_NAME, m_element->defaultNewInputFlags());

    m_graphicsView->showProperties();
}

void Node::removeInput()
{
    m_element->removeInput();
    m_graphicsView->showProperties();
}

void Node::setInputName(const uint8_t socketId, const QString &name)
{
    m_element->setInputName(socketId, name);
    m_inputs[socketId]->setName(name);
    calculateBoundingRect();
    m_graphicsView->showProperties();
}

void Node::addOutput()
{
    uint8_t const SIZE{ static_cast<uint8_t>(m_element->outputs().size()) };
    QString const OUTPUT_NAME{ QString("#%1").arg(SIZE + 1) };

    ValueType const TYPE{ first_available_type_for_flags(m_element->defaultNewOutputFlags()) };
    m_element->addOutput(TYPE, OUTPUT_NAME, m_element->defaultNewOutputFlags());

    m_graphicsView->showProperties();
}

void Node::removeOutput()
{
    m_element->removeOutput();
    m_graphicsView->showProperties();
}

void Node::setOutputName(const uint8_t socketId, const QString &name)
{
    m_element->setOutputName(socketId, name);
    m_outputs[socketId]->setName(name);
    calculateBoundingRect();
    m_graphicsView->showProperties();
}

void Node::addSocket(const SocketType type, const uint8_t id, const QString &name, const ValueType valueType)
{
    auto const socket = new SocketItem{ this, type };
    socket->setElementId(m_type == Type::Element ? m_element->id() : 0);
    socket->setSocketId(id);

    socket->setName(name);
    socket->setValueType(valueType);

    if (m_mode == Mode::Iconified)
        socket->hideName();
    else
        socket->showName();

    if (type == SocketType::Input)
        m_inputs.push_back(socket);
    else
        m_outputs.push_back(socket);
}

void Node::removeSocket(const SocketType type)
{
    switch (type) {
        case SocketType::Input:
            delete m_inputs.back();
            m_inputs.pop_back();
            break;

        case SocketType::Output:
            delete m_outputs.back();
            m_outputs.pop_back();
            break;
    }
}

void Node::setSocketType(const IOSocketsType socketType, const uint8_t socketId, const ValueType type)
{
    assert(m_element);

    bool const INPUTS{ socketType == IOSocketsType::Inputs };
    auto &io = INPUTS ? m_element->inputs()[socketId] : m_element->outputs()[socketId];

    if (!value_type_allowed(io.flags, type)) {
        qDebug() << "ERROR: Changing io's {}@{} type to {} is not allowed.", m_element->id(), io.id,
                            ValueType_to_QString(type);
        return;
    }

    SocketItem *socket{};
    if (m_type == Type::Element)
        socket = INPUTS ? m_inputs[socketId] : m_outputs[socketId];
    else
        socket = m_type == Type::Inputs ? m_outputs[socketId] : m_inputs[socketId];

    if (socket->valueType() == type) return;

    socket->disconnectAll();

    m_element->setIOValueType(INPUTS, socketId, type);

    socket->setValueType(type);
}

void Node::updateOutputs()
{
    if (!m_element || m_type == Type::Outputs) return;

    auto const IS_ELEMENT = m_type == Type::Element;
    auto const &ELEMENT_IOS = IS_ELEMENT ? m_element->outputs() : m_element->inputs();
    auto const &NODE_IOS = m_outputs;
    size_t const SIZE{ ELEMENT_IOS.size() };
    for (size_t i = 0; i < SIZE; ++i) {
        switch (ELEMENT_IOS[i].type) {
            case ValueType::Bool: {
                bool const SIGNAL{ std::get<bool>(ELEMENT_IOS[i].value) };
                NODE_IOS[static_cast<int>(i)]->setSignal(SIGNAL);
                  break;
            }
            default: break;
        }
    }
}

void Node::showName()
{
    m_showName = true;
    calculateBoundingRect();
}

void Node::hideName()
{
    m_showName = false;
    calculateBoundingRect();
}

void Node::setName(const QString& name)
{
    m_name = name;

    if (m_graphicsView) m_graphicsView->updateName(this);

    if (m_element) {
        m_element->setName(name);
        setToolTip(QString("%1 (%2)").arg(name).arg(m_element->id()));
    } else {
        setToolTip(QString("%1").arg(name));
    }
}

void Node::setIcon(const QString& iconPath)
{
    m_iconPath = iconPath;
    m_icon.load(iconPath);
}

void Node::setElement(Element* const element)
{
    if (m_element) return;

    m_type = Type::Element;  // <====================
    //qDebug() << "m_type = Type::Element: Node.cpp 697";


    m_element = element;
    if (m_type == Type::Element)
        m_element->registerEventHandler([this](Event const &event) { handleEvent(event); });

    auto const &INPUTS = m_element->inputs();
    auto const &OUTPUTS = m_element->outputs();

    switch (m_type) {
        case Type::Element:
            for (size_t i = 0; i < INPUTS.size(); ++i) {
                QString const NAME{ INPUTS[i].name };
                addSocket(SocketType::Input, static_cast<uint8_t>(i), NAME, INPUTS[i].type);
            }
            for (size_t i = 0; i < OUTPUTS.size(); ++i) {
                QString const NAME{ OUTPUTS[i].name };
                addSocket(SocketType::Output, static_cast<uint8_t>(i), NAME, OUTPUTS[i].type);
            }

            m_element->setPosition(x(), y());
            m_element->iconify(m_mode == Mode::Iconified);

            setName(m_element->name());
            updateOutputs();
            break;

        case Type::Inputs:
            for (size_t i = 0; i < INPUTS.size(); ++i) {
                QString const NAME{ INPUTS[i].name };
                addSocket(SocketType::Output, static_cast<uint8_t>(i), NAME, INPUTS[i].type);
            }
            break;

        case Type::Outputs:
            for (size_t i = 0; i < OUTPUTS.size(); ++i) {
                QString const NAME{ OUTPUTS[i].name};
                addSocket(SocketType::Input, static_cast<uint8_t>(i), NAME, OUTPUTS[i].type);
            }
            break;
    }

    elementSet();

    calculateBoundingRect();
}

void Node::setPropertiesTable(TableWidget * const properties)
{
    m_properties = properties;
}

void Node::setCentralWidget(QGraphicsItem *a_centralWidget)
{
    if (m_centralWidget) delete m_centralWidget;
    m_centralWidget = a_centralWidget;
    m_centralWidget->setParentItem(this);
    m_centralWidget->setPos(m_centralWidgetPosition);
}
