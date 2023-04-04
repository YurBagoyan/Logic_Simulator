#include "Include/UI/tabwidget.h"
#include "Include/UI/graphicsview.h"

#include <QInputDialog>

TabWidget::TabWidget(QWidget* parent)
    : QTabWidget(parent)
{
    connect(this, &TabWidget::tabCloseRequested, this, &TabWidget::closeTab);
    this->setTabBar(new TabBar);

}

void TabWidget::closeTab(const int index)
{
    // Do not close last tab
    if(this->count() > 1) {
        removeTab(index);
    }
}

void TabWidget::addNewTab()
{
    addTab(new GraphicsView(this), "New Tab");
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
