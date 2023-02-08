#include "Include/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    if (w.isAppReady()) {
        w.showMaximized();
        return a.exec();
    }

    return 0;
}
