#include "Include/package.h"
#include "Include/registry.h"
#include "qdebug.h"


Package::Package()
  : Element{}
{
    m_elements.push_back(this);

    setDefaultNewInputFlags(IOSocket::DefaultFlags);
    setDefaultNewOutputFlags(IOSocket::DefaultFlags);
}

Package::~Package()
{
    size_t const SIZE{ m_elements.size() };
    for (size_t i = 1; i < SIZE; ++i) delete m_elements[i];
}

void Package::calculate()
{
    for (auto &&connection : m_connections) {
        Element *const source{ getElement(connection.from_id) };
        Element *const target{ getElement(connection.to_id) };

        auto const IS_SOURCE_SELF = connection.from_id == 0;
        auto const IS_TARGET_SELF = connection.to_id == 0;

        auto const &SOURCE_IO = IS_SOURCE_SELF ? source->inputs() : source->outputs();
        auto &targetIO = IS_TARGET_SELF ? target->outputs() : target->inputs();
        targetIO[connection.to_socket].value = SOURCE_IO[connection.from_socket].value;
    }

    for (auto &&element : m_elements) {
        if (!element || element == this) continue;

        element->update(m_delta);
        element->calculate();
    }
}

Element *Package::add(string::hash_t const hash)
{
//  //pauseDispatchThread();

    qDebug() << "Adding element..";

    Registry &registry{ Registry::get() };

    Element *const element{ registry.createElement(hash) };
    assert(element);

    size_t index{};
    if (m_free.empty()) {
        index = m_elements.size();
        m_elements.emplace_back(element);
    } else {
        index = m_free.back();
        assert(m_elements[index] == nullptr);
        m_elements[index] = element;
        m_free.pop_back();
    }

    element->setPackage(this);  // <-------------
    element->setID(index);          // <-------------
    element->reset();

//  resumeDispatchThread();

    return element;
}

void Package::remove(size_t const id)
{
  //pauseDispatchThread();

    qDebug() << "Removing element {}.." << id;

    assert(id > 0);
    assert(id < m_elements.size());
    assert(std::find(std::begin(m_free), std::end(m_free), id) == std::end(m_free));

    delete m_elements[id];
    m_elements[id] = nullptr;
    m_free.emplace_back(id);

  //resumeDispatchThread();
}

Element *Package::getElement(size_t const id) const
{
    //assert(id < m_elements.size());
    //assert(std::find(std::begin(m_free), std::end(m_free), id) == std::end(m_free));
    return m_elements[id];
}

bool Package::connect(size_t const sourceId, uint8_t const sourceSocket, size_t const targetId,
                      uint8_t const targetSocket)
{
  //pauseDispatchThread();


    auto const source = getElement(sourceId);
    auto const target = getElement(targetId);


    qDebug("Connecting source: {}@{} to target: {}@{}", sourceId, static_cast<int>(sourceSocket),
                        targetId, static_cast<int>(targetSocket));

    auto const &SOURCE = sourceId != 0 ? source->m_outputs : source->m_inputs;
    auto &TARGET = targetId != 0 ? target->m_inputs : target->m_outputs;
    auto const SOURCE_SIZE = SOURCE.size();
    auto const TARGET_SIZE = TARGET.size();
    assert(sourceSocket < SOURCE_SIZE && "Socket ID don't exist");
    assert(targetSocket < TARGET_SIZE && "Socket ID don't exist");

    TARGET[targetSocket].id = sourceId;
    TARGET[targetSocket].slot = sourceSocket;

//  spaghetti::log::debug("Notifying {}({})@{} when {}({})@{} changes..", targetId, target->name(),
//                        static_cast<int32_t>(targetSocket), sourceId, source->name(),
//                        static_cast<int32_t>(sourceSocket));

    m_connections.emplace_back(Connection{ sourceId, sourceSocket, targetId, targetSocket });

  /// Member CallBacks
//  auto &dependencies = m_dependencies[sourceId];
//  auto const IT = std::find(std::begin(dependencies), std::end(dependencies), targetId);
//  if (IT == std::end(dependencies)) dependencies.push_back(targetId);

  //resumeDispatchThread();

  return true;
}

bool Package::disconnect(size_t const sourceId, uint8_t const outputId, size_t const targetId,
                         uint8_t const inputId)
{
  //pauseDispatchThread();

    Element *const target{ getElement(targetId) };

//  spaghetti::log::debug("Disconnecting source: {}@{} from target: {}@{}", a_sourceId, static_cast<int>(a_outputId),
//                        a_targetId, static_cast<int>(a_inputId));

    auto &targetInput = target->m_inputs[inputId];
    targetInput.id = 0;
    targetInput.slot = 0;
    resetIOSocketValue(targetInput);

    auto it = std::remove_if(std::begin(m_connections), std::end(m_connections), [=](Connection &a_connection) {
        return a_connection.from_id == sourceId && a_connection.from_socket == outputId &&
           a_connection.to_id == targetId && a_connection.to_socket == inputId;
    });
    m_connections.erase(it, std::end(m_connections));

  //auto &dependencies = m_dependencies[a_sourceId];
  //dependencies.erase(std::find(std::begin(dependencies), std::end(dependencies), a_targetId), std::end(dependencies));

  //resumeDispatchThread();

  return true;
}
