#include "Include/Elements/element.h"
#include "Include/package.h"

#include "qdebug.h"

void Element::iconify(const bool iconify)
{
    qDebug() << "before";

    if(m_isIconified) {
        qDebug() << "done";
        m_isIconified = iconify;
    }
}

bool Element::connect(const size_t sourceId, const uint8_t outputId, const uint8_t inputId)
{
    return m_package->connect(sourceId, outputId, m_id, inputId);
}

bool Element::addInput(const ValueType type, const QString &name, const uint8_t flags)
{
    if (m_inputs.size() + 1 > m_maxInputs) {
        return false;
    }

    IOSocket input{};
    input.name = name;
    input.type = type;
    input.flags = flags;

    resetIOSocketValue(input);
    m_inputs.emplace_back(input);

    handleEvent(Event{ EventType::InputAdded, EventEmpty{} });

    return true;
}

void Element::setInputName(const uint8_t input, const QString &name)
{
    auto const OLD_NAME = m_inputs[input].name;
    if (OLD_NAME == name) return;

    m_inputs[input].name = name;

    handleEvent(Event{ EventType::IONameChanged, EventIONameChanged{ OLD_NAME, name, input, true } });
}

void Element::removeInput()
{
    m_inputs.pop_back();

    handleEvent(Event{ EventType::InputRemoved, EventEmpty{} });
}

void Element::clearInputs()
{
    m_inputs.clear();
}

bool Element::addOutput(const ValueType type, const QString &name, const uint8_t flags)
{
    if (m_outputs.size() + 1 > m_maxOutputs) return false;

    IOSocket output{};
    output.name = name;
    output.type = type;
    output.flags = flags;

    resetIOSocketValue(output);
    m_outputs.emplace_back(output);

    handleEvent(Event{ EventType::OutputAdded, EventEmpty{} });

    return true;
}

void Element::setOutputName(const uint8_t output, const QString &name)
{
    auto const OLD_NAME = m_outputs[output].name;
    if (OLD_NAME == name) return;

    m_outputs[output].name = name;

    handleEvent(Event{ EventType::IONameChanged, EventIONameChanged{ OLD_NAME, name, output, false } });
}

void Element::removeOutput()
{
    m_outputs.pop_back();

    handleEvent(Event{ EventType::OutputRemoved, EventEmpty{} });
}

void Element::clearOutputs()
{
    m_outputs.clear();
}

void Element::setName(const QString &name)
{
    const QString OLD_NAME = m_name;
    m_name = name;

    handleEvent(Event{ EventType::ElementNameChanged, EventNameChanged{ OLD_NAME, name } });
}

void Element::setPosition(const double x, const double y)
{
    m_position.x = x;
    m_position.y = y;
}

void Element::setIOName(const bool input, const uint8_t id, const QString &name)
{
    if (input)
        setInputName(id, name);
    else
        setOutputName(id, name);
}

void Element::resetIOSocketValue(IOSocket &io)
{
    switch (io.type) {
        case ValueType::Bool: io.value = false; break;
        case ValueType::Int: io.value = 0; break;
        case ValueType::Float: io.value = 0.0f; break;
    }
}

void Element::handleEvent(Event const &event)
{
    onEvent(event);
    if (m_handler) {
        m_handler(event);
    }
}

void Element::setMinInputs(uint8_t const min)
{
    if (min > m_maxInputs) return;
    m_minInputs = min;
}

void Element::setMaxInputs(uint8_t const max)
{
    if (max < m_minInputs) return;
    m_maxInputs = max;
}

void Element::setMinOutputs(uint8_t const min)
{
    if (min > m_maxOutputs) return;
    m_minOutputs = min;
}

void Element::setMaxOutputs(uint8_t const max)
{
    if (max < m_minOutputs) return;
    m_maxOutputs = max;
}

void Element::setIOValueType(const bool input, const uint8_t id, const ValueType type)
{
    auto &io = input ? m_inputs[id] : m_outputs[id];
    ValueType const OLD_TYPE = io.type;

    if (OLD_TYPE == type) return;

    io.type = type;
    resetIOSocketValue(io);


    handleEvent(Event{ EventType::IOTypeChanged, EventIOTypeChanged{ input, id, OLD_TYPE, type }});
}
