#include "Include/UI/expanderwidget.h"

#include <QHeaderView>

ExpanderWidget::ExpanderWidget(QWidget *const a_parent)
  : QTreeWidget{ a_parent }
{
  header()->hide();
  header()->setSectionResizeMode(QHeaderView::Stretch);

  setRootIsDecorated(false);
  setColumnCount(1);
  setIndentation(0);

  setVerticalScrollMode(ScrollPerPixel);

  connect(this, &ExpanderWidget::itemPressed, this, &ExpanderWidget::onItemPressed);





}

int ExpanderWidget::addItem(QWidget *const a_widget, QIcon const &a_iconSet, QString const &a_text)
{
  return insertItem(-1, a_widget, a_iconSet, a_text);
}

int ExpanderWidget::addItem(QWidget *const a_widget, QString const &a_text)
{
  return addItem(a_widget, {}, a_text);
}

int ExpanderWidget::insertItem(int const a_index, QWidget *const a_widget, QIcon const &a_iconSet,
                               QString const &a_text)
{
  QIcon icon{};
  icon.addPixmap(QPixmap{ ":Docs/InterfaceIcons/expanded.ico" }, QIcon::Normal, QIcon::On);
  icon.addPixmap(QPixmap{ ":Docs/InterfaceIcons/collapsed.ico" }, QIcon::Normal, QIcon::Off);

  auto const category = new QTreeWidgetItem{ this };
  category->setText(0, a_text);
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

  setItemWidget(view, 0, a_widget);
  //setItemExpanded(category, true);
  category->setExpanded(true);

  int insertedIndex{};

  if (a_index == -1) {
    insertedIndex = m_widgets.count();
    m_widgets.push_back({ a_text, a_iconSet, category, a_widget });
  } else {
    insertedIndex = a_index;
    m_widgets.insert(a_index, { a_text, a_iconSet, category, a_widget });
  }

  return insertedIndex;
}

int ExpanderWidget::insertItem(int const a_index, QWidget *const a_widget, QString const &a_text)
{
  return insertItem(a_index, a_widget, {}, a_text);
}

int ExpanderWidget::count() const
{
  return m_widgets.count();
}

int ExpanderWidget::indexOf(QWidget *const a_widget) const
{
  int index = -1;

  auto it = std::find_if(std::begin(m_widgets), std::end(m_widgets),
                         [a_widget](auto const &a_first) { return a_first.widget == a_widget; });
  if (it != std::end(m_widgets)) index = static_cast<int>(std::distance(std::begin(m_widgets), it));

  return index;
}

QWidget *ExpanderWidget::widget(int const a_index) const
{
  return m_widgets[a_index].widget;
}

QIcon ExpanderWidget::itemIcon(int const a_index) const
{
  return m_widgets[a_index].icon;
}

QString ExpanderWidget::itemText(int const a_index) const
{
  return m_widgets[a_index].text;
}

void ExpanderWidget::removeItem(int const a_index)
{
  m_widgets.removeAt(a_index);
}

void ExpanderWidget::setItemIcon(int const a_index, QIcon const &a_icon)
{
  m_widgets[a_index].icon = a_icon;
}

void ExpanderWidget::setItemText(int const a_index, QString const &a_text)
{
  m_widgets[a_index].category->setText(0, a_text);
  m_widgets[a_index].text = a_text;
}

void ExpanderWidget::onItemPressed(QTreeWidgetItem *const a_item, int const a_column)
{
  (void)a_column;

    if (a_item && a_item->parent() == nullptr) {
        //setExpanded(a_item, !a_item->isExpanded());
        a_item->setExpanded(!a_item->isExpanded());

    }
}
