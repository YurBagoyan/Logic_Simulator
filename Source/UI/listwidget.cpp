#include "Include/UI/listwidget.h"
#include "Include/mainwindow.h"

#include <QBuffer>

ListWidget::ListWidget(QWidget* parent)
    : QListWidget{}
{
    setViewMode(QListView::ListMode);
    setIconSize(QSize(70, 50));
    setSpacing(5);

    setDragEnabled(true);
    setDragDropOverwriteMode(true);
    setDropIndicatorShown(true);
    setDragDropMode(DragOnly);
    setDefaultDropAction(Qt::CopyAction);

    setStyleSheet(  "QListWidget {"
                    "   background: #2e2f30;"
                    "   color: #dbdcdd;"
                    "   border-bottom: 1px solid red;"
                    "   border: transparent;"
                    "}"
                    "QListWidgetItem:hover{"
                    "   background: #1d545c;"
                    "   "
                    "}"
                    ""
                    "QScrollBar:vertical {"
                    "   border:  #404142;"
                    "   background:  #2e2f30;"
                    "   width: 10px;"
                    "   margin: 0px 0px 0px 0px;"
                    "}"
                    ""
                    "QScrollBar::handle:vertical {"
                    "   background: #404142;"
                    "   border: 1px solid #404142;"
                    "   min-height: 20px;"
                    "border-radius: 5px;"
                    "}"
                    ""
                    "QScrollBar::handle:vertical:hover {"
                    "   background: gray;"
                    "}"

                    "QScrollBar::add-line:vertical {"
                    "    background: transparent;"
                    "    height: 20px;"
                    "    subcontrol-position: bottom;"
                    "    subcontrol-origin: margin;"
                    "}"
                    ""
                    "QScrollBar::sub-line:vertical {"
                    "   background: transparent;"
                    "    height: 20px;"
                    "    subcontrol-position: top;"
                    "    subcontrol-origin: margin;"
                    "}"
                    ""
                    "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
                    "    width: 0px;"
                    "    height: 0px;"
                    "}");


}

void ListWidget::doResize()
{
    doItemsLayout();
    setFixedHeight(qMax(contentsSize().height(), 1));
}

void ListWidget::startDrag(Qt::DropActions supportedActions)
{
    (void)supportedActions;

    auto const graphicsView = m_mainWindow->graphicsView();
    if (!graphicsView) return;

    auto const item = currentItem();
    auto const TYPE = item->data(ListWidget::MetaDataType).toString();
    auto const IS_PACKAGE = item->data(ListWidget::MetaDataIsPackage).toByteArray();
    auto const NAME = item->data(ListWidget::MetaDataName).toByteArray();
    auto const ICON = item->data(ListWidget::MetaDataIcon).toByteArray();
    auto const FILE = item->data(ListWidget::MetaDataFilename).toByteArray();

    auto const mimeData = new QMimeData;
    mimeData->setText(TYPE);
    mimeData->setData("metadata/is_package", IS_PACKAGE);
    mimeData->setData("metadata/name", NAME);
    mimeData->setData("metadata/icon", ICON);
    mimeData->setData("metadata/filename", FILE);

    auto const drag = new QDrag{ this };
    drag->setMimeData(mimeData);
    drag->exec(Qt::CopyAction);
}

void ListWidget::createDrag(const QPoint &pos, QListWidgetItem* item)
{
    if(item == nullptr) {
        return;
    }

    /// Convert item info to QByteArray
    auto const type = item->text().toUtf8();
    QString iconPath;

    if(type == "Multiplexer" || type == "Demultiplexer")
        iconPath = ":Docs/LogicIcons/" + item->text() + ".ico";
    else
         iconPath = ":Docs/GateIcons/" + item->text() + ".ico";


    auto const iconPathByte = iconPath.toUtf8();

    QMimeData* mimeData = new QMimeData();
    mimeData->setData("metadata/name", type);
    mimeData->setData("metadata/icon", iconPathByte);

    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);

    QPoint p(30,20);
    QPixmap pix = item->icon().pixmap(60, 40);
    drag->setHotSpot(p);
    drag->setPixmap(pix);
    drag->exec(Qt::CopyAction);

}

