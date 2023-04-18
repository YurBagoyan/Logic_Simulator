#include "Include/package.h"
#include "Include/registry.h"

#include <QElapsedTimer>
#include <QDebug>
#include <QTimer>
#include <QThread>

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
    pauseDispatchThread();

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

    element->m_package = this;
    element->m_id = index;
    element->reset();

    resumeDispatchThread();

    return element;
}

void Package::remove(size_t const id)
{
    pauseDispatchThread();

    qDebug() << "Removing element" << id;

    assert(id > 0);
    assert(id < m_elements.size());
    assert(std::find(std::begin(m_free), std::end(m_free), id) == std::end(m_free));

    delete m_elements[id];
    m_elements[id] = nullptr;
    m_free.emplace_back(id);

    resumeDispatchThread();
}

Element *Package::getElement(size_t const id) const
{
    assert(id < m_elements.size());
    assert(std::find(std::begin(m_free), std::end(m_free), id) == std::end(m_free));
    return m_elements[id];
}

bool Package::connect(size_t const sourceId, uint8_t const sourceSocket, size_t const targetId,
                      uint8_t const targetSocket)
{
    pauseDispatchThread();


    auto const source = getElement(sourceId);
    auto const target = getElement(targetId);

//    qDebug() << "Connecting source: " << sourceId << "->" << static_cast<int>(sourceSocket) <<
//              "to target: " << targetId << "->" << static_cast<int>(targetSocket);

    auto const &SOURCE = sourceId != 0 ? source->m_outputs : source->m_inputs;
    auto &TARGET = targetId != 0 ? target->m_inputs : target->m_outputs;
    auto const SOURCE_SIZE = SOURCE.size();
    auto const TARGET_SIZE = TARGET.size();
    assert(sourceSocket < SOURCE_SIZE && "Socket ID don't exist");
    assert(targetSocket < TARGET_SIZE && "Socket ID don't exist");

    TARGET[targetSocket].id = sourceId;
    TARGET[targetSocket].slot = sourceSocket;

//    qDebug() << "Notifying " << targetId << "(" << target << ") -> " << static_cast<int32_t>(targetSocket) << " when "  // <-------------
//             << sourceId << "(" << source << ") -> " << static_cast<int32_t>(sourceSocket) << "changes...";             // <----------------

    m_connections.emplace_back(Connection{ sourceId, sourceSocket, targetId, targetSocket });

    /// Member CallBacks
    //auto &dependencies = m_dependencies[sourceId];
    //auto const IT = std::find(std::begin(dependencies), std::end(dependencies), targetId);
    //if (IT == std::end(dependencies)) dependencies.push_back(targetId);

    resumeDispatchThread();

    return true;
}

bool Package::disconnect(size_t const sourceId, uint8_t const outputId, size_t const targetId,
                         uint8_t const inputId)
{
    pauseDispatchThread();

    Element *const target{ getElement(targetId) };

//    qDebug("Disconnecting source: {}@{} from target: {}@{}", sourceId, static_cast<int>(outputId),
//                        targetId, static_cast<int>(inputId));

    auto &targetInput = target->m_inputs[inputId];
    targetInput.id = 0;
    targetInput.slot = 0;
    resetIOSocketValue(targetInput);

    auto it = std::remove_if(std::begin(m_connections), std::end(m_connections), [=](Connection &a_connection) {
        return a_connection.from_id == sourceId && a_connection.from_socket == outputId &&
           a_connection.to_id == targetId && a_connection.to_socket == inputId;
    });
    m_connections.erase(it, std::end(m_connections));

    //auto &dependencies = m_dependencies[sourceId];
    //dependencies.erase(std::find(std::begin(dependencies), std::end(dependencies), targetId), std::end(dependencies));

    resumeDispatchThread();

    return true;
}

void Package::dispatchThreadFunction()
{
    using clock_t = std::chrono::high_resolution_clock;

    auto const ONE_MILLISECOND = std::chrono::milliseconds(1);

    auto last = clock_t::now() - ONE_MILLISECOND;

    while (!m_quit) {
//        QElapsedTimer timer;
//        qDebug() << "start clac";
//        timer.start();

        auto const NOW = clock_t::now();
        auto const DELTA = NOW - last;

        update(DELTA);
        calculate();

        last = NOW;

        auto const WAIT_START = clock_t::now();
        while ((clock_t::now() - WAIT_START) < ONE_MILLISECOND) std::this_thread::sleep_for(ONE_MILLISECOND);

        if (m_pause) {
            //qDebug() << "Pause requested..";
            m_paused = true;
            //qDebug() << "Pausing..";
            while (m_pause) std::this_thread::yield();
            m_paused = false;
            //qDebug() << "Pause stopped..";
        }


//        QThread::sleep(2);

//        qint64 elapsed = timer.nsecsElapsed();
//        qDebug() << "end of clac, TIME = " << elapsed << "nansec";
    }
}

void Package::startDispatchThread()
{
    if (m_dispatchThreadStarted) return;

    //qDebug() << "Starting dispatch thread..";
    m_dispatchThread = std::thread(&Package::dispatchThreadFunction, this);
    m_dispatchThreadStarted = true;
}

void Package::quitDispatchThread()
{
    if (!m_dispatchThreadStarted) return;

    //qDebug() << "Quitting dispatch thread..";

    if (m_pause) {
        //qDebug() << "Dispatch thread paused, waiting..";
        while (m_pause) std::this_thread::yield();
    }

    m_quit = true;
    if (m_dispatchThread.joinable()) {
        //qDebug() << "Waiting for dispatch thread join..";
        m_dispatchThread.join();
        //qDebug() << "After dispatch thread join..";
    }
    m_dispatchThreadStarted = false;
}

void Package::pauseDispatchThread()
{
    if (m_package) {
        m_package->pauseDispatchThread();
        return;
    }

    if (!m_dispatchThreadStarted) return;

    //m_pauseCount++;

    //qDebug() << "Trying to pause dispatch thread (" << m_pauseCount.load() << ")..";

    if (m_pauseCount > 1) return;

    m_pause = true;

    //qDebug() << "Pausing dispatch thread (" << m_pauseCount.load() << ")..";
    while (!m_paused) std::this_thread::yield();
}

void Package::resumeDispatchThread()
{
    if (m_package) {
        m_package->resumeDispatchThread();
        return;
    }

    if (!m_dispatchThreadStarted) return;

    //m_pauseCount--;

    //qDebug() << "Trying to resume dispatch thread (" << m_pauseCount.load() << ")..";

    if (m_pauseCount > 0) return;

    //qDebug() << "Resuming dispatch thread (" << m_pauseCount.load() << ")..";

    m_pause = false;
}
