#ifndef GATE_H
#define GATE_H

#include <QObject>

enum class ElementType { AND, OR, NOT, NAND, NOR, XOR, XNOR, };

class Gate
{
    //Q_OBJECT
public:
    Gate(const ElementType type, bool input_1 = false, bool input_2 = false);

    // Geters
    size_t getId() const;
    QString getName() const;
    ElementType getType() const;
    QString getTypeInString() const;
    bool getInput_1() const;
    bool getInput_2() const;

    // Seters
    void setName(const QString name);
    void setInputs(bool input_1, bool input_2);


    virtual void calculate() = 0;

protected:
    void setNextId();

protected:
    size_t m_id;
    ElementType m_type;
    QString m_name;

    bool m_input_1;
    bool m_input_2;
    bool m_output;
};

#endif // GATE_H
