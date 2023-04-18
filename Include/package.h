#ifndef PACKAGE_H
#define PACKAGE_H

#include <atomic>
#include <thread>

#include "Include/Elements/element.h"
#include "Include/registry.h"
#include "Include/strings.h"

class Package : public Element
{
public:
    using Elements = std::vector<Element *>;
    struct Connection {
        size_t from_id{};
        uint8_t from_socket{};
        size_t to_id{};
        uint8_t to_socket{};
    };
    using Connections = std::vector<Connection>;

    Package();
    ~Package() override;

    static constexpr char const *const TYPE{ "logic/package" };
    static constexpr string::hash_t const HASH{ string::hash(TYPE) };

    void calculate() override;
    void update(duration_t const &delta) override { m_delta = delta; }

    Element *add(char const *const name) { return add(string::hash(name)); }
    Element *add(string::hash_t const hash);

    void remove(Element *const element) { remove(element->id()); }
    void remove(size_t const id);

    bool connect(size_t const sourceId, uint8_t const sourceSocket, size_t const targetId, uint8_t const targetSocket);
    bool disconnect(size_t const sourceId, uint8_t const outputId, size_t const targetId, uint8_t const inputId);

    void dispatchThreadFunction();

    void startDispatchThread();
    void quitDispatchThread();
    void pauseDispatchThread();
    void resumeDispatchThread();

public: /// Setters and Getters
    void setPackageDescription(QString const &a_description) { m_packageDescription = a_description; }
    void setPackagePath(QString const &path) { m_packagePath = path; }
    void setPackageIcon(QString const &icon) { m_packageIcon = icon; }

    void setInputsPosition(double const x, double const y);
    void setInputsPosition(VecToD const position) { m_inputsPosition = position; }
    void setOutputsPosition(double const x, double const y);
    void setOutputsPosition(VecToD const position) { m_outputsPosition = position; }


    Element* getElement(size_t const id) const;
    Elements const &elements() const { return m_elements; }
    Connections const &connections() const { return m_connections; }
    string::hash_t hash() const noexcept override { return HASH; }
    char const *type() const noexcept override { return TYPE; }

    QString packageDescription() const { return m_packageDescription; }
    QString packagePath() const { return m_packagePath; }
    QString packageIcon() const { return m_packageIcon; }

    VecToD const &inputsPosition() const { return m_inputsPosition; }
    VecToD const &outputsPosition() const { return m_outputsPosition; }

private:
    duration_t m_delta{};
    QString m_packageDescription{ "A package" };
    QString m_packagePath{};
    QString m_packageIcon{ ":Docs/LogicIcons/Unknown.ico" };
    VecToD m_inputsPosition{ -400.0, 0.0 };
    VecToD m_outputsPosition{ 400.0, 0.0 };
    Elements m_elements{};
    Connections m_connections{};

    std::vector<size_t> m_free{};

    //using Callbacks = spp::sparse_hash_map<size_t, std::vector<size_t>>;
    //Callbacks m_dependencies{};
    std::thread m_dispatchThread{};
    std::atomic_bool m_dispatchThreadStarted{};
    std::atomic_bool m_quit{};
    std::atomic_bool m_pause{};
    std::atomic_bool m_paused{};
    std::atomic_uint32_t m_pauseCount{100};
    bool m_isExternal{};

};

inline void Package::setInputsPosition(double const x, double const y)
{
    m_inputsPosition.x = x;
    m_inputsPosition.y = y;
}

inline void Package::setOutputsPosition(double const x, double const y)
{
    m_outputsPosition.x = x;
    m_outputsPosition.y = y;
}

#endif // PACKAGE_H
