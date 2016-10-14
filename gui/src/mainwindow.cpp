#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(std::make_unique<Ui::MainWindow>())
{
    this->m_ui->setupUi(this);
    this->m_ui->brightnessSlider->setRange(MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    this->m_ui->brightnessSlider->setTickInterval(BRIGHTNESS_INCREMENT);
    this->m_currentBrightness = getCurrentBrightness();
    this->m_ui->brightnessSlider->setValue(this->m_currentBrightness);

    connect(this->m_ui->brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(onBrightnessSliderChanged(int)));
    connect(this->m_ui->brightnessCustomLineEdit, SIGNAL(focusLost()), this, SLOT(onCustomLineEditFocusLost()));
    connect(this->m_ui->brightnessCustomLineEdit, SIGNAL(focusGained()), this, SLOT(onCustomLineEditFocusGained()));
    connect(this->m_ui->minButton, SIGNAL(clicked(bool)), this, SLOT(onMinButtonClicked(bool)));
    connect(this->m_ui->maxButton, SIGNAL(clicked(bool)), this, SLOT(onMaxButtonClicked(bool)));
    connect(&m_brightnessChangeTimer, SIGNAL(timeout()), this, SLOT(checkForBrightnessUpdate()));

    this->m_ui->brightnessSlider->setMinimumSize(MIN_SLIDER_WIDTH, MIN_SLIDER_HEIGHT);
    this->m_currentBrightnessString = std::to_string(this->m_currentBrightness);
    this->m_ui->brightnessCustomLineEdit->setText(toQString(this->m_currentBrightnessString));
    this->m_ui->brightnessCustomLineEdit->clearFocus();

    this->m_ui->centralWidgetGridLayout->installEventFilter(this);
    m_brightnessChangeTimer.setInterval(BRIGHTNESS_CHECK_INTERVAL_MS);
    m_brightnessChangeTimer.start();
}

void MainWindow::checkForBrightnessUpdate()
{
    int tempBrightnessUpdate = getCurrentBrightness();
    if (tempBrightnessUpdate != this->m_currentBrightness) {
        this->m_currentBrightness = tempBrightnessUpdate;
        setBrightness(this->m_currentBrightness);
    }
}

bool MainWindow::isIntegerNumeric(const std::string &stringToCheck)
{
    if (stringToCheck == "") {
        return false;
    }
    std::string copyString{stringToCheck};
    for (auto &it : copyString) {
        if ((it == '-') && (it != *std::begin(copyString))) {
            return false;
        } else if (it == '-') {
            continue;
        } else if (!isDigit(it)) {
            return false;
        }
    }
    return true;
}

bool MainWindow::isDigit(char charToCheck)
{
    return ((charToCheck == '0') || (charToCheck == '1') || (charToCheck == '2') || (charToCheck == '3') || (charToCheck == '4') || (charToCheck == '5') || (charToCheck == '6') || (charToCheck == '7') || (charToCheck == '8') || (charToCheck == '9'));
}

void MainWindow::onCustomLineEditFocusGained()
{
    this->m_ui->brightnessCustomLineEdit->setText(toQString(getCurrentBrightness()));
}

void MainWindow::onCustomLineEditFocusLost()
{
    //emit(this->m_ui->okayButton->clicked());
    //this->m_ui->brightnessCustomLineEdit->setText(toQString(getCurrentBrightness()));
}

void MainWindow::onBrightnessSliderChanged(int newValue)
{
    setBrightness(newValue);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type()== QEvent::KeyPress) {
        QKeyEvent* key {static_cast<QKeyEvent*>(event)};
        if (key->key() == Qt::Key_Direction_R) {
            setBrightness(this->m_currentBrightness + BRIGHTNESS_INCREMENT);
        } else if (key->key() == Qt::Key_Direction_L) {
            setBrightness(this->m_currentBrightness - BRIGHTNESS_INCREMENT);
        } else {
            return QObject::eventFilter(obj, event);
        }
        return true;
    } else {
        return QObject::eventFilter(obj, event);
    }
    return false;
}

void MainWindow::setBrightness(int newValue)
{
    if (newValue > MAX_BRIGHTNESS) {
        this->m_systemCommand.setCommand(ECHO_STRING + std::to_string(MAX_BRIGHTNESS) + TEE_TO_BACKLIGHT_STRING);
        this->m_systemCommand.execute();
        this->m_currentBrightness = MAX_BRIGHTNESS;
    } else if (newValue < MIN_BRIGHTNESS) {
        this->m_systemCommand.setCommand(ECHO_STRING + std::to_string(MIN_BRIGHTNESS) + TEE_TO_BACKLIGHT_STRING);
        this->m_systemCommand.execute();
        this->m_currentBrightness = MIN_BRIGHTNESS;
    } else {
        this->m_systemCommand.setCommand(ECHO_STRING + std::to_string(newValue) + TEE_TO_BACKLIGHT_STRING);
        this->m_systemCommand.execute();
        this->m_currentBrightness = newValue;
    }
    this->m_currentBrightnessString = std::to_string(this->m_currentBrightness);
    this->m_ui->brightnessCustomLineEdit->setText(QString::fromStdString(this->m_currentBrightnessString));
    this->m_ui->brightnessSlider->setValue(this->m_currentBrightness);
}

void MainWindow::onMinButtonClicked(bool toggledOrNot)
{
    setBrightness(MIN_BRIGHTNESS);
    Q_UNUSED(toggledOrNot);
}

void MainWindow::onMaxButtonClicked(bool toggledOrNot)
{
    setBrightness(MAX_BRIGHTNESS);
    Q_UNUSED(toggledOrNot);
}

int MainWindow::getShellPID()
{
    return getpid();
}

int MainWindow::getCurrentBrightness()
{
    int brightness{0};
    this->m_systemCommand.setCommand(CAT_TO_BACKLIGHT_STRING);
    this->m_systemCommand.execute();
    if (!this->m_systemCommand.hasError()) {
        try {
            brightness = std::stoi(this->m_systemCommand.outputAsString());
        } catch (std::exception &e) {
            std::unique_ptr<QMessageBox> errorBox{std::make_unique<QMessageBox>()};
            errorBox->setText("Exception caught while trying to cast potential brightness (" + toQString(this->m_systemCommand.outputAsString()) + ") to string, exiting program" );
            errorBox->setWindowTitle(QString::fromStdString(ERROR_STRING));
            errorBox->setWindowIcon(QIcon(":/icons/chromebookbrightness.png"));
            errorBox->exec();
            exit(EXIT_FAILURE);
        }
        if (brightness < MIN_BRIGHTNESS) {
            std::cout << "WARNING: Current brightness is less than minimum brightness value (" << MIN_BRIGHTNESS << ")"  << std::endl;
            brightness = MIN_BRIGHTNESS;
        } else if (brightness > MAX_BRIGHTNESS) {
            std::cout << "WARNING: Current brightness is greater than maximum brightness value (" << MAX_BRIGHTNESS << ")" <<  std::endl;
            brightness = MAX_BRIGHTNESS;
        }
    } else {
        std::unique_ptr<QMessageBox> errorBox{std::make_unique<QMessageBox>()};
        errorBox->setText("Exception caught while trying to cast potential brightness (" + toQString(this->m_systemCommand.outputAsString()) + ") to string, exiting program" );
        errorBox->setWindowTitle(QString::fromStdString(ERROR_STRING));
        errorBox->setWindowIcon(QIcon(":/icons/chromebookbrightness.png"));
        errorBox->exec();
        exit(EXIT_FAILURE);
    }
    return brightness;
}

MainWindow::~MainWindow()
{
}
