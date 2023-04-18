#ifndef EXPANDERWIDGET_H
#define EXPANDERWIDGET_H

#include <QIcon>
#include <QTreeWidget>

class ExpanderWidget : public QTreeWidget {
    Q_OBJECT
    Q_PROPERTY(int count READ count)

public:
    explicit ExpanderWidget(QWidget *const parent = nullptr);

    int addItem(QWidget *const widget, QIcon const &iconSet, QString const &text);
    int addItem(QWidget *const widget, QString const &text);

    int insertItem(int const index, QWidget *const widget, QIcon const &iconSet, QString const &text);
    int insertItem(int const index, QWidget *const widget, QString const &text);

    int count() const;

    int indexOf(QWidget *const widget) const;

    QWidget *widget(int const index) const;
    QIcon itemIcon(int const index) const;
    QString itemText(int const index) const;

    void removeItem(int const index);

    void setItemIcon(int const index, QIcon const &icon);
    void setItemText(int const index, QString const &text);

private:
    void onItemPressed(QTreeWidgetItem *const item, int const column);

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
