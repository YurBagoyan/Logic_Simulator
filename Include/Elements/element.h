#ifndef ELEMENT_H
#define ELEMENT_H

#include <chrono>
#include <string>
#include <QString>

#include "Include/strings.h"

class Package;

enum class ValueType { Bool, Int, Float };

enum class EventType {
    ElementNameChanged,
    IONameChanged,
    IOTypeChanged,
    InputAdded,
    InputRemoved,
    OutputAdded,
    OutputRemoved
};

struct EventNameChanged {
    QString from;
    QString to;
};

struct EventIONameChanged {
    QString from;
    QString to;
    uint8_t id;
    bool input;
};

struct EventIOTypeChanged {
    bool input;
    uint8_t id;
    ValueType from;
    ValueType to;
};

struct EventEmpty {
};

using EventValue = std::variant<EventNameChanged, EventIONameChanged, EventIOTypeChanged, EventEmpty>;
struct Event {
    EventType type{};
    EventValue payload{};
};

using EventCallback = std::function<void(Event const &)>;


class Element
{
public:
    using duration_t = std::chrono::duration<double, std::milli>;
    using Value = std::variant<bool, int32_t, float>;
    // Position
    template<typename T>
    struct Vec2 {
      T x{};
      T y{};
    };

    using Vec = Vec2<int32_t>;
    using VecToF = Vec2<float>;
    using VecToD = Vec2<double>;

    struct IOSocket {
        enum Flags {
            CanHoldBool = 1 << 0,
            CanHoldInt = 1 << 1,
            CanHoldFloat = 1 << 2,
            CanChangeName = 1 << 3,
            CanHoldAllValues = CanHoldBool | CanHoldInt | CanHoldFloat,
            DefaultFlags = CanHoldAllValues | CanChangeName
        };
        Value value{};
        ValueType type{};

        size_t id{};
        uint8_t slot{};
        uint8_t flags{};
        QString name{};
    };
    using IOSockets = std::vector<IOSocket>;

public:
    Element() = default;
    virtual ~Element() = default;

    virtual char const *type() const noexcept = 0;
    virtual string::hash_t hash() const noexcept = 0;

    virtual void calculate() {}
    virtual void reset() {}

    virtual void update(duration_t const &delta) { (void)delta; }

    void iconify(bool const iconify);
    bool isIconified() const { return m_isIconified; }

    bool connect(size_t const sourceId, uint8_t const outputId, uint8_t const inputId);

    bool addInput(ValueType const type, QString const &name, uint8_t const flags);
    void setInputName(uint8_t const input, QString const &name);
    void removeInput();
    void clearInputs();

    bool addOutput(ValueType const type, QString const &name, uint8_t const flags);
    void setOutputName(uint8_t const output, QString const &name);
    void removeOutput();
    void clearOutputs();

    void registerEventHandler(EventCallback const &a_handler) { m_handler = a_handler; }

protected:
    void handleEvent(Event const &event);
    virtual void onEvent(Event const &event) { (void)event; }

    void setMinInputs(uint8_t const a_min);
    void setMaxInputs(uint8_t const a_max);
    void setDefaultNewInputFlags(uint8_t const a_flags) { m_defaultNewInputFlags = a_flags; }

    void setMinOutputs(uint8_t const a_min);
    void setMaxOutputs(uint8_t const a_max);
    void setDefaultNewOutputFlags(uint8_t const a_flags) { m_defaultNewOutputFlags = a_flags; }

public: /// Seters and geters
    void setID(size_t id) {m_id = id;};
    void setName(QString const &name);
    void setNode(void *const a_node) { m_node = a_node; }
    void setPackage(Package* package) {m_package = package;};
    void setPosition(double const x, double const y);
    void setPosition(VecToD const &position) { m_position = position; }
    void setIconifyingHidesCentralWidget(bool const a_hide) { m_iconifyingHidesCentralWidget = a_hide; }

    size_t id() const noexcept { return m_id; }
    QString name() const noexcept { return m_name; }
    VecToD const &position() const { return m_position; }
    bool iconifyingHidesCentralWidget() const { return m_iconifyingHidesCentralWidget; }
    Package* package() const { return m_package; }

    uint8_t minInputs() const { return m_minInputs; }
    uint8_t maxInputs() const { return m_maxInputs; }
    uint8_t defaultNewInputFlags() const { return m_defaultNewInputFlags; }
    uint8_t minOutputs() const { return m_minOutputs; }
    uint8_t maxOutputs() const { return m_maxOutputs; }
    uint8_t defaultNewOutputFlags() const { return m_defaultNewOutputFlags; }

    IOSockets &inputs() { return m_inputs; }
    IOSockets const &inputs() const { return m_inputs; }
    IOSockets &outputs() { return m_outputs; }
    IOSockets const &outputs() const { return m_outputs; }
    void setIOName(bool const input, uint8_t const id, QString const &name);
    void setIOValueType(bool const input, uint8_t const id, ValueType const type);

    void resetIOSocketValue(IOSocket &io);

public:
    IOSockets m_inputs{};
    IOSockets m_outputs{};

protected:
    friend class Package;
    Package *m_package{};

private:
    size_t m_id;
    QString m_name;
    VecToD m_position;

    bool m_isIconified;
    bool m_iconifyingHidesCentralWidget;
    uint8_t m_minInputs;
    uint8_t m_maxInputs{ std::numeric_limits<uint8_t>::max() };
    uint8_t m_minOutputs;
    uint8_t m_maxOutputs{ std::numeric_limits<uint8_t>::max() };
    uint8_t m_defaultNewInputFlags;
    uint8_t m_defaultNewOutputFlags;
    EventCallback m_handler;
    void *m_node;
};

#endif // ELEMENT_H
