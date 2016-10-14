#include <QApplication>
#include <QString>
#include <thread>
#include <string>
#include <tjlutils.h>

#include "mainwindow.h"

const static int ROOT_USER_ID = 0;
const static QString MAIN_WINDOW_TITLE = "Chromebook Brightness Control";
const static std::string GKSUDO_STRING = "gksudo ";
const static std::string EXECUTABLE_NAME = "brightness-gui";

void launchRootProcess()
{
    SystemCommand systemCommand;
    systemCommand.setCommand(GKSUDO_STRING + EXECUTABLE_NAME);
    systemCommand.execute();
}

int main(int argc, char *argv[])
{
    if (geteuid() != ROOT_USER_ID) {
        std::thread launchRootThread(launchRootProcess);
        launchRootThread.detach();
        exit (EXIT_SUCCESS);
    }

    QApplication qApplication(argc, argv);

    MainWindow mainWindow;
    mainWindow.setWindowTitle(MAIN_WINDOW_TITLE);
    mainWindow.setWindowIcon(QIcon(":/icons/chromebookbrightness.png"));
    mainWindow.show();
    return qApplication.exec();
}
