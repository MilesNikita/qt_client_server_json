#include "agent.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Agent ag;
    return a.exec();
}
