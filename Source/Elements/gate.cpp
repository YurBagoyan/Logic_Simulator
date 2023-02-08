#include "Include/Elements/gate.h"

#include <QDebug>

Gate::Gate(const GateType type, bool input_1, bool input_2)
    : m_type(type)
    , m_input_1(input_1)
    , m_input_2(input_2)
{
    setNextId();
    // Gate name = type_id
    m_name = getTypeInString() + "_" + QString::number(m_id);
    qDebug() << m_name;

}


size_t Gate::getId() const
{
    return m_id;
}

QString Gate::getName() const
{
    return m_name;
}

GateType Gate::getType() const
{
    return m_type;
}

QString Gate::getTypeInString() const
{
    switch (m_type) {
      case GateType::AND: return "AND";
      case GateType::OR: return "OR";
      case GateType::NOT: return "NOT";
      case GateType::NAND: return "NAND";
      case GateType::NOR: return "NOR";
      case GateType::XOR: return "XOR";
      case GateType::XNOR: return "XNOR";
      default: return "Unknown";
    }
}

void Gate::setName(const QString name)
{
    m_name = name;
}

void Gate::setNextId()
{
    // unique ID for each type of gate
    static size_t next_and_id;
    static size_t next_or_id;
    static size_t next_not_id;
    static size_t next_nand_id;
    static size_t next_nor_id;
    static size_t next_xor_id;
    static size_t next_xnor_id;

    switch (m_type) {
      case GateType::AND:   m_id = next_and_id++; break;
      case GateType::OR:    m_id = next_or_id++; break;
      case GateType::NOT:   m_id = next_not_id++; break;
      case GateType::NAND:  m_id = next_nand_id++; break;
      case GateType::NOR:   m_id = next_nor_id++; break;
      case GateType::XOR:   m_id = next_xor_id++; break;
      case GateType::XNOR:  m_id = next_xnor_id++; break;
      default: break;
    }

}


