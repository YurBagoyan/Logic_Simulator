#include "Include/UI/tablewidget.h"


#include <QHeaderView>

TableWidget::TableWidget(QWidget* parent)
    : QTableWidget(parent)
{
    setColumnCount(2);
    //setRowCount(10);
    verticalHeader()->setHidden(true);
    horizontalHeader()->setStretchLastSection(true);

    setHeaders();

    //setElementFields();
    //setInputFields();
    //setOutputFields();

    setWidgetStyleSheet();


}

void TableWidget::setHeaders()
{
    headers << "Name" << "Value";
    setHorizontalHeaderLabels(headers);
}

void TableWidget::setLabels()
{
    labelElements = new QLabel("Element", this);
    labelInputs = new QLabel("Inputs", this);
    labelOutput = new QLabel("Output", this);

    const QString labelStyle = "QLabel {"
                               "    background: #808080;"
                               "}";

    labelElements->setStyleSheet(labelStyle);
    labelInputs->setStyleSheet(labelStyle);
    labelOutput->setStyleSheet(labelStyle);

    labelElements->setAlignment(Qt::AlignCenter);
    labelInputs->setAlignment(Qt::AlignCenter);
    labelOutput->setAlignment(Qt::AlignCenter);

    setCellWidget(0, 0, labelElements);
    setCellWidget(4, 0, labelInputs);
    setCellWidget(7, 0, labelOutput);

    setSpan(0, 0, 1, 2); // labelElements
    setSpan(4, 0, 1, 2); // labelInputs
    setSpan(7, 0, 1, 2); // labelOutput
}

void TableWidget::setElementFields()
{
    nameLine = new QLineEdit(this);
    nameLine->setText("AND_15");

    QString nameLineStyle =  "QLineEdit {"
                            "   background: #292a2b;"
                            "   color: #dbdcdd;"
                            "   border: transparent;"
                            "}";
    nameLine->setStyleSheet(nameLineStyle);

    setCellWidget(3, 1, nameLine);
}

void TableWidget::setInputFields()
{
    inSpinBox_1 = new QSpinBox(this);
    inSpinBox_2 = new QSpinBox(this);

    inSpinBox_1->setMaximum(1);
    inSpinBox_1->setMinimum(0);

    inSpinBox_2->setMaximum(1);
    inSpinBox_2->setMinimum(0);

    setCellWidget(5, 1, inSpinBox_1);
    setCellWidget(6, 1, inSpinBox_2);

    QString spinBoxStyle =  "QSpinBox{"
                            "    background: #292a2b;"
                            "   color: #dbdcdd;"
                            "   border: transparent;"
                            "}";

    inSpinBox_1->setStyleSheet(spinBoxStyle);
    inSpinBox_2->setStyleSheet(spinBoxStyle);

}

void TableWidget::setOutputFields()
{

}

void TableWidget::setWidgetStyleSheet()
{
    setStyleSheet(  "QTableWidget {"
                    "   background: #292a2b;"
                    "   color: #dbdcdd;"
                    "}"

                    "TableWidget::item {"
                        "color: #dbdcdd;"
                    "   border: 0.5px solid #3e3f40;"
                    "   border-top-color: transparent;"
                    "   border-left-color:transparent;"
                    "}"

                    "QTableWidget::item:selected {"
                    "	background: #1d545c;"
                    "}"

                    "QTableWidget::item:hover {"
                    "	background: #1d545c;"
                    "}"

                    "QHeaderView::section{"
                    "   background-color: qlineargradient(spread:pad, x1:0.508, y1:0, x2:0.52, y2:1, stop:0 rgba(71, 72, 72, 255), stop:0.458101 rgba(71, 72, 72, 255), stop:0.575419 rgba(54, 54, 54, 255), stop:0.994413 rgba(54, 54, 54, 255));"
                    "   color: #dbdcdd;"
                    "   padding-left: 4px;"
                    "   border: 0.5px solid #191919;"
                    "}");


}

void TableWidget::setAllFieldsReadOnly()
{
//    for (int row = 0; row < this->rowCount(); ++row) {
//        for (int col = 0; col < this->columnCount(); ++col) {
//            item(row, col)->setFlags(item(row, col)->flags() &  ~Qt::ItemIsEditable);
//        }
//    }
}
