#ifndef DIGITALCLOCK_H
#define DIGITALCLOCK_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QFont>
#include <QDateTime>
#include <QGroupBox>
#include <QPalette>

QT_BEGIN_NAMESPACE
namespace Ui {
class DigitalClock;
}
QT_END_NAMESPACE

class DigitalClock : public QMainWindow
{
    Q_OBJECT

public:
    DigitalClock(QWidget *parent = nullptr);
    ~DigitalClock();

private slots:
    void updateTime();
    void toggleFormat();
    void changeColor();
    void changeFontSize();
    void toggleSeconds();
    void changeTheme();
    void toggleDate();

private:
    Ui::DigitalClock *ui;
    void setupUI();
    void setupTimer();
    void setSayac();
    void startTime();
    void applyTimeFormat();
    void updateDisplay();
    void setClockColor(const QColor &color);

    QWidget *timerWidget = nullptr;
    QLabel *sayacLabel = nullptr;
    QTimer *sayacTimer = nullptr;
    QLabel *timeLabel;
    int sayacSeconds;

    QWidget *clockWidget;
    QLabel *dateLabel;
    QLabel *timer;
    QVBoxLayout *dynamicLayout;
    QCheckBox *format24CheckBox;
    QCheckBox *showSecondsCheckBox;
    QCheckBox *showDateCheckBox;
    QComboBox *colorComboBox;
    QComboBox *fontSizeComboBox;
    QComboBox *themeComboBox;
    QPushButton *fullScreenButton;
    QPushButton *timerButton;

    bool is24HourFormat;
    bool showSeconds;
    bool showDate;
    int fontSize;
    int font2;
    QColor currentColor;
};
#endif // DIGITALCLOCK_H
