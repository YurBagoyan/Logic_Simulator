#include "Include/UI/tabwidget.h"
#include "Include/UI/graphicsview.h"
#include "Include/mainwindow.h"

#include <QInputDialog>

#include "Include/Elements/all.h"

Element* getElement(const QString name)
{
    using namespace elements;

    if(name == "AND")
        return new gates::And();
    else if(name == "NAND")
        return new gates::Nand();
    else if(name == "NOR")
        return new gates::Nor();
    else if(name == "NOT")
        return new gates::Not();
    else if(name == "OR")
        return new gates::Or;
    else if(name == "XNOR")
        return new gates::Xnor;
    else if(name == "XOR")
        return new gates::Xor;
    else if(name == "CLOCK")
        return new timers::Clock();
    else if(name == "Multiplexer")
        return new logic::Multiplexer();
    else if(name == "Demultiplexer")
        return new logic::Demultiplexer();
    else if(name == "Package")
        return new Package();

    return nullptr;
}

TabWidget::TabWidget(QWidget* parent)
    : QTabWidget(parent)
{
    connect(this, &TabWidget::tabCloseRequested, this, &TabWidget::closeTab);
    this->setTabBar(new TabBar);

}

void TabWidget::closeTab(const int index)
{
    //Do not close last tab
    if(this->count() > 1) {
        removeTab(index);
    }
}

void TabWidget::addNewTab(const QString tabName)
{
    addTab(new GraphicsView(m_mainWindow, new Package), tabName);
    connect(widget(TabWidget::count() - 1), SIGNAL(itemDrop(QString,QString,QPointF)), this, SLOT(createObject(QString,QString,QPointF)));
}

void TabWidget::createObject(const QString name, const QString iconPath, const QPointF pos)
{
//    qDebug() << "Creating object" << name;
//    Node* node = new Node(iconPath);

//    Element* element = getElement(name);
//    node->setElement(element);
//    node->setName(name);
//    node->setPos(pos);
//    GraphicsView* current = reinterpret_cast<GraphicsView*>(currentWidget());
//    current->scene()->addItem(node);
}


void TabBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    int index = tabAt(event->pos());
    if (index != -1) {
        bool ok;

        QInputDialog* inputDialog = new QInputDialog;
        inputDialog->setStyleSheet("QInputDialog { background: yellow; }");


        QLineEdit *lineEdit = inputDialog->findChild<QLineEdit*>();
        if (lineEdit) {
            lineEdit->setStyleSheet("QLineEdit { background: yellow; }");
        }

        QString text = inputDialog->getText(this, tr("Rename tab"),
                                             tr("Enter new name:"), QLineEdit::Normal,
                                             tabText(index), &ok);
        if (ok && !text.isEmpty()) {
            setTabText(index, text);
        }
    }
}
