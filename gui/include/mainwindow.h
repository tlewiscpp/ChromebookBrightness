#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QSlider>
#include <QTimer>
#include <QMessageBox>
#include <QMainWindow>
#include <QKeyEvent>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <memory>
#include <thread>
#include <unistd.h>

#include <systemcommand.h>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    const static QString MAIN_WINDOW_TITLE;
    ~MainWindow();
    static const int ROOT_USER_ID{0};

private:
    const std::string ECHO_STRING{"echo "};
    const std::string TEE_TO_BACKLIGHT_STRING{" | tee /sys/class/backlight/intel_backlight/brightness"};
    const std::string CAT_TO_BACKLIGHT_STRING{"cat /sys/class/backlight/intel_backlight/brightness"};
    const std::string ERROR_STRING{"ERROR"};
    const int BRIGHTNESS_INCREMENT{50};
    const int MIN_BRIGHTNESS{25};
    const int MAX_BRIGHTNESS{7812};
    const int MIN_SLIDER_WIDTH{300};
    const int MIN_SLIDER_HEIGHT{20};
    const int BRIGHTNESS_CHECK_INTERVAL_MS{100};

    SystemCommand m_systemCommand;
    QTimer m_brightnessChangeTimer;
    int m_currentBrightness;
    int m_tempBrightness;
    std::string m_currentBrightnessString;
    std::unique_ptr<Ui::MainWindow> m_ui;

    bool isIntegerNumeric(const std::string &stringToCheck);
    bool isDigit(char charToCheck);
    void setBrightness(int newValue);
    bool eventFilter(QObject *obj, QEvent *event);
    int getShellPID();
    int getCurrentBrightness();

    template<typename T>
    QString toQString(const T &thingToConvert) { return QString::fromStdString(std::to_string(thingToConvert)); }
    QString toQString(const std::string &thingToConvert) { return QString::fromStdString(thingToConvert); }
    QString toQString(const char *thingToConvert) { return QString::fromStdString(thingToConvert); }


private slots:
    void onBrightnessSliderChanged(int newValue);
    void onCustomLineEditFocusGained();
    void onCustomLineEditFocusLost();
    void onMinButtonClicked(bool toggledOrNot);
    void onMaxButtonClicked(bool toggledOrNot);
    void checkForBrightnessUpdate();
};

#endif // MAINWINDOW_H
