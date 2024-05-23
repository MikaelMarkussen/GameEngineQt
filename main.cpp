#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include "DialogueDLL/Dialogue.h"

int main(int argc, char *argv[])
{
    //Dialogue lib test, can't find text file despite pathing being correct
    InitializeDialogue(std::vector<std::string> {"/..GEA2022/BasicDialogueTextMaster.txt"});

    //Forces the usage of desktop OpenGL - Qt uses OpenGL ES as default
    //Attribute must be set before Q(Gui)Application is constructed:
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    //Makes a Qt application
    QApplication a(argc, argv);

    //Makes the Qt MainWindow and shows it.
    MainWindow w;
    w.show();

    return a.exec();
}
