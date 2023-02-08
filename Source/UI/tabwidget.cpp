#include "Include/UI/tabwidget.h"
#include "qevent.h"

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

void TabBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    int index = tabAt(event->pos());
    if (index >= 0) {
        QLineEdit *edit = new QLineEdit(tabText(index), this);

        connect(edit, &QLineEdit::editingFinished, this, [this, edit, index]() {
            setTabText(index, edit->text());
            edit->deleteLater();
        });

        edit->installEventFilter(this);
        edit->setFocus();
        edit->show();
    }
}

bool TabBar::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::FocusOut) {
        QLineEdit *edit = qobject_cast<QLineEdit*>(obj);
        if (edit) {
            edit->deleteLater();
        }
    }
    return false;
}
