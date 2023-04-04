#ifndef EXPANDERWIDGET_H
#define EXPANDERWIDGET_H

#include <QIcon>
#include <QTreeWidget>

class ExpanderWidget : public QTreeWidget {
  Q_OBJECT
  Q_PROPERTY(int count READ count)

 public:
  explicit ExpanderWidget(QWidget *const a_parent = nullptr);

  int addItem(QWidget *const a_widget, QIcon const &a_iconSet, QString const &a_text);
  int addItem(QWidget *const a_widget, QString const &a_text);

  int insertItem(int const a_index, QWidget *const a_widget, QIcon const &a_iconSet, QString const &a_text);
  int insertItem(int const a_index, QWidget *const a_widget, QString const &a_text);

  int count() const;

  int indexOf(QWidget *const a_widget) const;

  QWidget *widget(int const a_index) const;
  QIcon itemIcon(int const a_index) const;
  QString itemText(int const a_index) const;

  void removeItem(int const a_index);

  void setItemIcon(int const a_index, QIcon const &a_icon);
  void setItemText(int const a_index, QString const &a_text);

 private:
  void onItemPressed(QTreeWidgetItem *const a_item, int const a_column);

 private:
  struct WidgetContainer {
    QString text{};
    QIcon icon{};
    QTreeWidgetItem *category{};
    QWidget *widget{};
  };
  QList<WidgetContainer> m_widgets{};
};

#endif // EXPANDERWIDGET_H
