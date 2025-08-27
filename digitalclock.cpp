#include "digitalclock.h"
#include "./ui_digitalclock.h"
#include <QApplication>
#include <QScreen>

DigitalClock::DigitalClock(QWidget *parent)
    : QMainWindow(parent),
    is24HourFormat(true),
    showSeconds(true),
    showDate(true),
    fontSize(48),
    currentColor(Qt::cyan)
    , ui(new Ui::DigitalClock)
{
    setupUI();
    setupTimer();
    updateTime();
}

DigitalClock::~DigitalClock()
{
    delete ui;
}

void DigitalClock::setupUI(){
    setWindowTitle("Dijital Saat");
    setMinimumSize(600,300);

    QWidget *central = new QWidget(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    mainLayout->addStretch(2);

    QWidget *centerContainer = new QWidget();
    centerContainer->setMaximumWidth(800);
    QVBoxLayout *centerLayout = new QVBoxLayout(centerContainer);
    centerLayout->setContentsMargins(20, 20, 20, 20);
    centerLayout->setSpacing(15);

    dynamicLayout = new QVBoxLayout();
    centerLayout->addLayout(dynamicLayout);

    clockWidget = new QWidget();
    clockWidget->setObjectName("clockWidget");
    clockWidget->setStyleSheet("background: rgba(0, 0, 0, 0.3); border-radius: 15px;");
    clockWidget->setFixedSize(500, 180); // Sabit boyut - responsive olmayacak

    QVBoxLayout *clockLayout = new QVBoxLayout(clockWidget);
    clockLayout->setContentsMargins(20, 20, 20, 20);
    clockLayout->setSpacing(10);

    timeLabel = new QLabel();
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->setStyleSheet("color: cyan; font-weight: bold;");
    clockLayout->addWidget(timeLabel);

    dateLabel = new QLabel();
    dateLabel->setAlignment(Qt::AlignCenter);
    dateLabel->setStyleSheet("color: cyan; font-size: 18px;");
    clockLayout->addWidget(dateLabel);

    QHBoxLayout *clockCenterLayout = new QHBoxLayout();
    clockCenterLayout->addStretch();
    clockCenterLayout->addWidget(clockWidget);
    clockCenterLayout->addStretch();
    centerLayout->addLayout(clockCenterLayout);

    QGroupBox *controlGroup = new QGroupBox("Settings");
    controlGroup->setFixedHeight(70);
    controlGroup->setMaximumWidth(700);
    controlGroup->setStyleSheet(
        "QGroupBox {"
        "background: rgba(0, 0, 0, 0.6);"
        "border: 1px solid rgba(255, 255, 255, 0.3);"
        "border-radius: 10px;"
        "color: white;"
        "font-weight: bold;"
        "margin-top: 10px;"
        "font-size: 12px;"
        "}"
        "QGroupBox::title {"
        "color: white;"
        "font-weight: bold;"
        "subcontrol-origin: margin;"
        "left: 10px;"
        "padding: 0 5px 0 5px;"
        "}"
        );

    QHBoxLayout *controlLayout = new QHBoxLayout(controlGroup);
    controlLayout->setContentsMargins(10, 20, 10, 10);
    controlLayout->setSpacing(8);

    format24CheckBox = new QCheckBox("24h");
    format24CheckBox->setChecked(is24HourFormat);
    format24CheckBox->setStyleSheet("color: white; font-size: 11px;");
    format24CheckBox->setFixedWidth(50);
    controlLayout->addWidget(format24CheckBox);

    showSecondsCheckBox = new QCheckBox("Sec");
    showSecondsCheckBox->setChecked(showSeconds);
    showSecondsCheckBox->setStyleSheet("color: white; font-size: 11px;");
    showSecondsCheckBox->setFixedWidth(50);
    controlLayout->addWidget(showSecondsCheckBox);

    showDateCheckBox = new QCheckBox("Date");
    showDateCheckBox->setChecked(showDate);
    showDateCheckBox->setStyleSheet("color: white; font-size: 11px;");
    showDateCheckBox->setFixedWidth(60);
    controlLayout->addWidget(showDateCheckBox);

    colorComboBox = new QComboBox();
    colorComboBox->addItems({"Cyan", "Green", "Red" , "Yellow" , "White" ,"Orange"});
    colorComboBox->setStyleSheet("color: white; background: rgba(255, 255, 255, 0.1); font-size: 10px; padding: 2px;");
    colorComboBox->setFixedWidth(70);
    controlLayout->addWidget(colorComboBox);

    fontSizeComboBox = new QComboBox();
    fontSizeComboBox->addItems({"24","36","48","60","72","96"});
    fontSizeComboBox->setStyleSheet("color: white; background: rgba(255, 255, 255, 0.1); font-size: 10px; padding: 2px;");
    fontSizeComboBox->setFixedWidth(50);
    controlLayout->addWidget(fontSizeComboBox);

    themeComboBox = new QComboBox();
    themeComboBox->addItems({"pink1","pink2","pink3","pink4","pink5","purp1","purp2","purp3","purp4","red1","red2","blue1"});
    themeComboBox->setStyleSheet("color: white; background: rgba(255, 255, 255, 0.1); font-size: 10px; padding: 2px;");
    themeComboBox->setFixedWidth(70);
    controlLayout->addWidget(themeComboBox);

    fullScreenButton = new QPushButton("Full");
    fullScreenButton->setStyleSheet("color: white; background: rgba(255, 255, 255, 0.1); padding: 3px; font-size: 10px;");
    fullScreenButton->setFixedWidth(50);
    controlLayout->addWidget(fullScreenButton);

    timerButton = new QPushButton("Timer");
    timerButton->setStyleSheet("color: white; background: rgba(255, 255, 255, 0.1); padding: 3px; font-size: 10px;");
    timerButton->setFixedWidth(50);
    controlLayout->addWidget(timerButton);

    QHBoxLayout *controlCenterLayout = new QHBoxLayout();
    controlCenterLayout->addStretch();
    controlCenterLayout->addWidget(controlGroup);
    controlCenterLayout->addStretch();
    centerLayout->addLayout(controlCenterLayout);

    QHBoxLayout *mainCenterLayout = new QHBoxLayout();
    mainCenterLayout->addStretch();
    mainCenterLayout->addWidget(centerContainer);
    mainCenterLayout->addStretch();
    mainLayout->addLayout(mainCenterLayout, 0);

    mainLayout->addStretch(2);

    setCentralWidget(central);

    connect(timerButton, &QPushButton::clicked,[this](){
        if (timerWidget) {
            dynamicLayout->removeWidget(timerWidget);
            timerWidget->deleteLater();
            timerWidget = nullptr;
            sayacLabel = nullptr;
            sayacTimer->stop();
            delete sayacTimer;
            sayacTimer = nullptr;
            timerButton->setText("Timer");
        } else {
            timerButton->setText("Clear");
            timerWidget = new QWidget();
            timerWidget->setStyleSheet("background: rgba(0, 0, 0, 0.4); border-radius: 10px;");
            timerWidget->setFixedSize(280, 100); // Sabit boyut

            QVBoxLayout *timerLayout = new QVBoxLayout(timerWidget);
            timerLayout->setContentsMargins(10, 10, 10, 10);
            timerLayout->setSpacing(5);

            // Sayaç label'ı
            sayacLabel = new QLabel("00:00");
            sayacLabel->setAlignment(Qt::AlignCenter);
            QFont sayacFont;
            sayacFont.setPointSize(20); // Sabit boyut
            sayacFont.setFamily("Arial");
            sayacFont.setBold(true);
            sayacLabel->setFont(sayacFont);
            sayacLabel->setStyleSheet("color: cyan; text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.9);");
            timerLayout->addWidget(sayacLabel);

            // Kompakt butonlar
            QPushButton *startBtn = new QPushButton("Start");
            QPushButton *stopBtn = new QPushButton("Stop");
            QPushButton *resetBtn = new QPushButton("Reset");

            QString btnStyle = "color: white; background: rgba(255, 255, 255, 0.2); padding: 2px; font-size: 9px; border-radius: 3px;";
            startBtn->setStyleSheet(btnStyle + "background: rgba(0, 255, 0, 0.3);");
            stopBtn->setStyleSheet(btnStyle + "background: rgba(255, 0, 0, 0.3);");
            resetBtn->setStyleSheet(btnStyle + "background: rgba(255, 255, 0, 0.3);");

            startBtn->setFixedSize(45, 25);
            stopBtn->setFixedSize(45, 25);
            resetBtn->setFixedSize(45, 25);

            QHBoxLayout *btnLayout = new QHBoxLayout();
            btnLayout->setSpacing(5);
            btnLayout->addStretch();
            btnLayout->addWidget(startBtn);
            btnLayout->addWidget(stopBtn);
            btnLayout->addWidget(resetBtn);
            btnLayout->addStretch();
            timerLayout->addLayout(btnLayout);

            sayacSeconds = 0;
            sayacTimer = new QTimer(this);
            sayacTimer->setInterval(1000);

            connect(sayacTimer, &QTimer::timeout, [this]() {
                sayacSeconds++;
                int dakika = sayacSeconds / 60;
                int saniye = sayacSeconds % 60;
                sayacLabel->setText(QString("%1:%2")
                                        .arg(dakika, 2, 10, QLatin1Char('0'))
                                        .arg(saniye, 2, 10, QLatin1Char('0')));
            });

            connect(startBtn, &QPushButton::clicked, [this]() {
                if (!sayacTimer->isActive())
                    sayacTimer->start();
            });

            connect(stopBtn, &QPushButton::clicked, [this]() {
                sayacTimer->stop();
            });

            connect(resetBtn, &QPushButton::clicked, [this]() {
                sayacTimer->stop();
                sayacSeconds = 0;
                sayacLabel->setText("00:00");
            });

            QHBoxLayout *timerCenterLayout = new QHBoxLayout();
            timerCenterLayout->addStretch();
            timerCenterLayout->addWidget(timerWidget);
            timerCenterLayout->addStretch();

            dynamicLayout->insertLayout(0, timerCenterLayout); // En üste ekle
        }
    });

    connect(fullScreenButton, &QPushButton::clicked,[this](){
        if(isFullScreen()){
            showNormal();
            fullScreenButton->setText("Full");
        }
        else{
            showFullScreen();
            fullScreenButton->setText("Exit");
        }
    });

    connect(fullScreenButton, &QPushButton::clicked,[this](){
        if(isFullScreen()){
            showNormal();
            fullScreenButton->setText("Full");
        }
        else{
            showFullScreen();
            fullScreenButton->setText("Exit");
        }
    });
    connect(format24CheckBox, &QCheckBox::toggled, this, &DigitalClock::toggleFormat);
    connect(showSecondsCheckBox, &QCheckBox::toggled, this, &DigitalClock::toggleSeconds);
    connect(showDateCheckBox, &QCheckBox::toggled, this, &DigitalClock::toggleDate);
    connect(colorComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),this, &DigitalClock::changeColor );
    connect(fontSizeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DigitalClock::changeFontSize);
    connect(themeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DigitalClock::changeTheme);

    connect(fullScreenButton, &QPushButton::clicked,[this](){
        if(isFullScreen()){
            showNormal();
            fullScreenButton->setText("Full Screen");
        }
        else{
            showFullScreen();
            fullScreenButton->setText("Exit Full Screen");
        }
    });

    changeFontSize();
    changeColor();
}

void DigitalClock::changeTheme() {
    QString selected = themeComboBox->currentText();
    QString backgroundPath;

    if(selected == "pink1") backgroundPath = ":/Photo/photo/pink1.jpg";
    else if(selected == "pink2") backgroundPath = ":/Photo/photo/pink2.jpg";
    else if(selected == "pink3") backgroundPath = ":/Photo/photo/pink3.gif";
    else if(selected == "pink4") backgroundPath = ":/Photo/photo/pink4.jpg";
    else if(selected == "pink5") backgroundPath = ":/Photo/photo/pink5.jpg";
    else if(selected == "purp1") backgroundPath = ":/Photo/photo/purp2.jpg";
    else if(selected == "purp2") backgroundPath = ":/Photo/photo/purp3.jpg";
    else if(selected == "purp3") backgroundPath = ":/Photo/photo/Purp4.jpg";
    else if(selected == "purp4") backgroundPath = ":/Photo/photo/purple1.jpg";
    else if(selected == "red1") backgroundPath = ":/Photo/photo/red1.jpg";
    else if(selected == "red2") backgroundPath = ":/Photo/photo/red1.jpg";
    else if(selected == "blue1") backgroundPath = ":/Photo/photo/blue1.jpg";

    if(!backgroundPath.isEmpty()) {
        this->setStyleSheet(QString(
                                "QMainWindow {"
                                "background-image: url(%1);"
                                "background-repeat: no-repeat;"
                                "background-position: center;"
                                "background-size: cover;"
                                "}"

                                "QWidget#clockWidget {"
                                "background: rgba(0, 0, 0, 0.3);"
                                "border-radius: 10px;"
                                "}"

                                "QGroupBox {"
                                "background: rgba(0, 0, 0, 0.5);"
                                "border: 1px solid rgba(255, 255, 255, 0.3);"
                                "border-radius: 10px;"
                                "color: white;"
                                "font-weight: bold;"
                                "}"

                                "QGroupBox::title {"
                                "color: white;"
                                "font-weight: bold;"
                                "}"

                                "QCheckBox, QComboBox, QPushButton {"
                                "color: white;"
                                "background: rgba(255, 255, 255, 0.1);"
                                "border: 1px solid rgba(255, 255, 255, 0.3);"
                                "border-radius: 5px;"
                                "padding: 5px;"
                                "}"

                                "QPushButton:hover {"
                                "background: rgba(255, 255, 255, 0.2);"
                                "}"
                                ).arg(backgroundPath));
        clockWidget->setObjectName("clockWidget");
    } else {
        this->setStyleSheet("QMainWindow { background-color: #2b2b2b; }");
        clockWidget->setStyleSheet("background-color: black; border-radius: 10px;");
    }
}

void DigitalClock::setupTimer(){
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout , this, &DigitalClock::updateTime);
    timer->start(1000);
}

void DigitalClock::updateTime(){
    QDateTime currentDateTime =  QDateTime::currentDateTime();

    QString timeFormat;

    if(is24HourFormat){
        timeFormat = showSeconds ? "hh:mm:ss" : "hh:mm";
    }  else{
        timeFormat =  showSeconds ? "hh:mm:ss AP" : "hh:mm AP";
    }

    QString timeText = currentDateTime.toString(timeFormat);
    timeLabel->setText(timeText);

    if(showDate){
        QString dateTime = currentDateTime.toString("dddd, MMMM d, yyyy");
        dateLabel->setText(dateTime);
        dateLabel->setVisible(true);
    }
    else {
        dateLabel->setVisible(false);
    }

}

void DigitalClock::changeColor(){
    QString colorName = colorComboBox->currentText().toLower();
    QColor color;

    if (colorName == "cyan") color = Qt::cyan;
    else if (colorName == "green") color = Qt::green;
    else if (colorName == "red") color = Qt::red;
    else if (colorName == "yellow") color = Qt::yellow;
    else if (colorName == "white") color = Qt::white;
    else if (colorName == "orange") color = QColor(255, 165, 0);
    else color = Qt::cyan;

    currentColor = color;
    setClockColor(color);
}

void DigitalClock::toggleFormat()
{
    is24HourFormat = format24CheckBox->isChecked();
    updateTime();
}

void DigitalClock::changeFontSize(){
    fontSize = fontSizeComboBox->currentText().toInt();

    QFont timeFont;
    timeFont.setPointSize(fontSize);
    timeFont.setFamily("Arial");
    timeFont.setBold(true);
    timeLabel->setFont(timeFont);

    QFont dateFont;
    dateFont.setPointSize(fontSize / 3);
    dateFont.setFamily("Arial");
    dateLabel->setFont(dateFont);
}

void DigitalClock::toggleSeconds()
{
    showSeconds = showSecondsCheckBox->isChecked();
    updateTime();
}


void DigitalClock::toggleDate()
{
    showDate = showDateCheckBox->isChecked();
    updateTime();
}

void DigitalClock::setClockColor(const QColor &color)
{
    QString colorName = color.name();
    timeLabel->setStyleSheet(QString(
                                 "color: %1; "
                                 "font-weight: bold; "
                                 "text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.8);"
                                 ).arg(colorName));

    dateLabel->setStyleSheet(QString(
                                 "color: %1; "
                                 "text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.8);"
                                 ).arg(colorName));
}
