#include "Include/Nodes/nodePackage.h"

#include "Include/UI/graphicsview.h"
#include "Include/package.h"
#include "Include/mainwindow.h"

namespace nodes {

NodePackage::NodePackage()
{

}

void NodePackage::showProperties()
{
    showCommonProperties();

    //auto const package = static_cast<Package *>(m_element);
   // auto const PATH = QString::fromStdString(std::string(package->packagePath()));
    //auto const ICON = QString::fromStdString(std::string(package->packageIcon()));

    propertiesInsertTitle("Package");

    QTableWidgetItem *item{};

    int row = m_properties->rowCount();
     m_properties->insertRow(row);
    item = new QTableWidgetItem{ "Path" };
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    m_properties->setItem(row, 0, item);

    //QLineEdit *pathEdit = new QLineEdit{ PATH };
    //pathEdit->setPlaceholderText("<path>");
    //m_properties->setCellWidget(row, 1, pathEdit);
//    QObject::connect(pathEdit, &QLineEdit::textChanged,
//                    [this, package](QString const &a_text) { package->setPackagePath(a_text.toStdString()); });

    row = m_properties->rowCount();
    m_properties->insertRow(row);
    item = new QTableWidgetItem{ "Icon" };
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    m_properties->setItem(row, 0, item);

    //QLineEdit *iconEdit = new QLineEdit{ ICON };
    //iconEdit->setPlaceholderText("<icon>");
    //m_properties->setCellWidget(row, 1, iconEdit);
//    QObject::connect(iconEdit, &QLineEdit::textChanged, [this](QString const &a_text) {
//      (void)a_text;
//      qDebug() << "ICON:" << a_text;
//      /* TODO */
//    });

    showIOProperties(IOSocketsType::Inputs);
    showIOProperties(IOSocketsType::Outputs);
}

void NodePackage::handleEvent(Event const &event)
{
    Node::handleEvent(event);

    if (!(m_inputsNode && m_outputsNode))
        return;

    switch (event.type) {
        case EventType::ElementNameChanged: {
            auto const &EVENT = std::get<EventNameChanged>(event.payload);
            auto const IS_ROOT = m_element->package() == nullptr;
            if (!IS_ROOT) {
                auto const graphicsView = m_inputsNode->graphicsView();
                auto const mainWindow = graphicsView->mainWindow();
                auto const INDEX = mainWindow->indexForGraphicsView(graphicsView);
                mainWindow->setGraphicsViewTabName(INDEX, EVENT.to);
            }
            break;
        }
        /*
//    case EventType::IONameChanged: {
//      auto const &EVENT = std::get<EventIONameChanged>(a_event.payload);
//      if (EVENT.input) {
//        m_inputsNode->outputs()[EVENT.id]->setName(QString::fromStdString(EVENT.to));
//        m_inputsNode->calculateBoundingRect();
//      } else {
//        m_outputsNode->inputs()[EVENT.id]->setName(QString::fromStdString(EVENT.to));
//        m_outputsNode->calculateBoundingRect();
//      }
//      break;
//    }
*/
        case EventType::IOTypeChanged: {
            auto const &EVENT = std::get<EventIOTypeChanged>(event.payload);
            if (EVENT.input)
                m_inputsNode->setSocketType(IOSocketsType::Inputs, EVENT.id, EVENT.to);
            else
                m_outputsNode->setSocketType(IOSocketsType::Outputs, EVENT.id, EVENT.to);
            break;
        }

        case EventType::InputAdded: {
            auto const &INPUTS = m_element->inputs();
            auto const INPUTS_SIZE = static_cast<int>(INPUTS.size());
            auto const &LAST_INPUT = INPUTS.back();
            auto const OUTPUTS_SIZE = m_inputsNode->outputs().size();
            auto const ADD_SOCKET_NEEDED = OUTPUTS_SIZE < INPUTS_SIZE;
            assert(ADD_SOCKET_NEEDED == true);
            m_inputsNode->addSocket(SocketType::Output, static_cast<uint8_t>(OUTPUTS_SIZE),
                                      QString::fromStdString(LAST_INPUT.name.toStdString()), LAST_INPUT.type);
            m_inputsNode->calculateBoundingRect();
            break;
        }

        case EventType::InputRemoved: {
            m_inputsNode->removeSocket(SocketType::Output);
            m_inputsNode->calculateBoundingRect();
            break;
        }

        case EventType::OutputAdded: {
            auto const &OUTPUTS = m_element->outputs();
            auto const OUTPUTS_SIZE = static_cast<int>(OUTPUTS.size());
            auto const &LAST_OUTPUT = OUTPUTS.back();
            auto const INPUTS_SIZE = m_outputsNode->inputs().size();
            auto const ADD_SOCKET_NEEDED = INPUTS_SIZE < OUTPUTS_SIZE;
            assert(ADD_SOCKET_NEEDED == true);
            m_outputsNode->addSocket(SocketType::Input, static_cast<uint8_t>(INPUTS_SIZE),
                                       QString::fromStdString(LAST_OUTPUT.name.toStdString()), LAST_OUTPUT.type);
            m_outputsNode->calculateBoundingRect();
            break;
        }

        case EventType::OutputRemoved: {
            m_outputsNode->removeSocket(SocketType::Input);
            m_outputsNode->calculateBoundingRect();
            break;
        }
    }
}

bool NodePackage::open()
{
    auto const mainWindow = m_graphicsView->mainWindow();
    //mainWindow->openOrCreateGraphicsView(static_cast<package::Package *>(m_element));

    return true;

}

} // namespace nodes
