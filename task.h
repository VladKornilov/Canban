#ifndef TASK_H
#define TASK_H

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QColor>
#include <QDateTime>

#include "employee.h"

class Task : public QWidget
{
    Q_OBJECT
public:
    Task(QString name, QString descr, QColor color, std::vector <Employee *> performers,
         QDateTime deadline, QDateTime created = QDateTime::currentDateTime());
    void setName(QString name);
    QString getName();

    void setDeadline(int y, int mo, int d, int h, int mi, int s);

    QDateTime getCreatedDateTime();
    QString getCreatedString();

    QDateTime getDeadlineDateTime();
    QString getDeadlineString();

    void updateTime();

protected:
    void paintEvent(QPaintEvent *);

private:
    QString name;
    QString description;
    //std::vector <Employee *> performers;
    //priority

    QDateTime created;
    QDateTime deadline;

    //GUI
    QLabel *nameLabel;
    QLabel *descriptionLabel;
    QLabel *createdLabel;
    QLabel *deadlineLabel;
    QLabel *timeleftLabel;
    QLabel *timeLabel;
    QLabel *perfLabel[3];

private slots:
    void pressBurger();
};

#endif // TASK_H
