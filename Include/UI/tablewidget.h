#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QObject>
#include <QTableWidget>
#include <QStringList>
#include <QLabel>
#include <QSpinBox>
#include <QLineEdit>

class TableWidget : public QTableWidget
{
    Q_OBJECT
public:
    TableWidget(QWidget* parent = nullptr);

private:
    void setHeaders();
    void setLabels();
    void setElementFields();
    void setInputFields();
    void setOutputFields();
    void setWidgetStyleSheet();

    void setAllFieldsReadOnly();

private:
    QStringList headers;

    QLabel* labelElements;
    QLabel* labelInputs;
    QLabel* labelOutput;

    // Elements
    QLineEdit* nameLine;

    // Input
    QSpinBox* inSpinBox_1;
    QSpinBox* inSpinBox_2;

    // Output

};

#endif // TABLEWIDGET_H


