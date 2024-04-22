#include "pathfinder.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PathFinder w;
    w.setWindowTitle("Path Finder");
    w.show();
    return a.exec();
}
