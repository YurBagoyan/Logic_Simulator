#include "Include/UI/expanderwidget.h"

#include <QHeaderView>

ExpanderWidget::ExpanderWidget(QWidget *const parent)
  : QTreeWidget{ parent }
{
    header()->hide();
    header()->setSectionResizeMode(QHeaderView::Stretch);

    setRootIsDecorated(false);
    setColumnCount(1);
    setIndentation(0);

    setVerticalScrollMode(ScrollPerPixel);

    connect(this, &ExpanderWidget::itemPressed, this, &ExpanderWidget::onItemPressed);
}

int ExpanderWidget::addItem(QWidget *const widget, QIcon const &iconSet, QString const &text)
{
    return insertItem(-1, widget, iconSet, text);
}

int ExpanderWidget::addItem(QWidget *const widget, QString const &text)
{
    return addItem(widget, {}, text);
}

int ExpanderWidget::insertItem(int const index, QWidget *const widget, QIcon const &iconSet, QString const &text)
{
    QIcon icon{};
    icon.addPixmap(QPixmap{ ":Docs/InterfaceIcons/expanded.ico" }, QIcon::Normal, QIcon::On);
    icon.addPixmap(QPixmap{ ":Docs/InterfaceIcons/collapsed.ico" }, QIcon::Normal, QIcon::Off);

    auto const category = new QTreeWidgetItem{ this };
    category->setText(0, text);
    category->setIcon(0, icon);
    setIconSize(QSize{ 25, 25 });

    auto font = category->font(0);
    font.setBold(true);
    font.setItalic(true);
    font.setPointSize(12);
    category->setFont(0, font);
    category->setTextAlignment(0, Qt::AlignHCenter);

    addTopLevelItem(category);

    auto const view = new QTreeWidgetItem{ category };

    setItemWidget(view, 0, widget);
    //setItemExpanded(category, true);
    category->setExpanded(true);

    int insertedIndex{};

    if (index == -1) {
      insertedIndex = m_widgets.count();
      m_widgets.push_back({ text, iconSet, category, widget });
    } else {
      insertedIndex = index;
      m_widgets.insert(index, { text, iconSet, category, widget });
    }

    return insertedIndex;
}

int ExpanderWidget::insertItem(int const index, QWidget *const widget, QString const &text)
{
    return insertItem(index, widget, {}, text);
}

int ExpanderWidget::count() const
{
    return m_widgets.count();
}

int ExpanderWidget::indexOf(QWidget *const widget) const
{
    int index = -1;

    auto it = std::find_if(std::begin(m_widgets), std::end(m_widgets),
                           [widget](auto const &a_first) { return a_first.widget == widget; });
    if (it != std::end(m_widgets)) index = static_cast<int>(std::distance(std::begin(m_widgets), it));

    return index;
}

QWidget *ExpanderWidget::widget(int const index) const
{
    return m_widgets[index].widget;
}

QIcon ExpanderWidget::itemIcon(int const index) const
{
    return m_widgets[index].icon;
}

QString ExpanderWidget::itemText(int const index) const
{
    return m_widgets[index].text;
}

void ExpanderWidget::removeItem(int const index)
{
    m_widgets.removeAt(index);
}

void ExpanderWidget::setItemIcon(int const index, QIcon const &icon)
{
    m_widgets[index].icon = icon;
}

void ExpanderWidget::setItemText(int const index, QString const &text)
{
    m_widgets[index].category->setText(0, text);
    m_widgets[index].text = text;
}

void ExpanderWidget::onItemPressed(QTreeWidgetItem *const item, int const column)
{
    (void)column;

    if (item && item->parent() == nullptr) {
        //setExpanded(a_item, !a_item->isExpanded());
        item->setExpanded(!item->isExpanded());

    }
}
