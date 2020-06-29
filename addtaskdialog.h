#ifndef ADDTASKDIALOG_H
#define ADDTASKDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QCalendarWidget>
#include <QTimeEdit>
#include <QDateTime>
#include <QGridLayout>
#include <QComboBox>

#include "employee.h"


class AddTaskDialog : public QDialog
{
    Q_OBJECT
public:
    AddTaskDialog();

    QString getTaskName() { return taskName->text(); }
    QString getTaskDescr() { return taskDescr->toPlainText(); }
    QColor getTaskColor() { return taskColor; }
    QDateTime getDateTime() { return QDateTime(calendar->selectedDate(), timeEdit->time()); }
    std::vector <Employee *> getPerformers();

private:
    QLineEdit *taskName;
    QTextEdit *taskDescr;
    QPushButton *colorButton;
    QColor taskColor;
    QPushButton *addPerfButton;
    QCalendarWidget *calendar;
    QTimeEdit *timeEdit;
    QGridLayout *layout;

    QHBoxLayout *setupPerfLayout();
    QLabel *performerPhotos[3];
    QComboBox *performerNames[3];
    size_t numOfPerformers = 0;
    std::vector <Employee *> performers;

private slots:
    void setColor();
    void addFirstPerformer();
    void addSecondPerformer();
    void addThirdPerformer();
    void replacePhoto(QString perfName);
};

#endif // ADDTASKDIALOG_H
