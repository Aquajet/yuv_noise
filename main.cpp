#include <QtGui/QApplication>

#include "mainwindow.h"
#include "Console.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
	MessageOutput::setMessageOutputToGlobalConsole();
	
	MainWindow w;
    w.showMaximized();

    return a.exec();
}