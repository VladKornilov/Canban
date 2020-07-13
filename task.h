#ifndef TASK_H
#define TASK_H

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QColor>
#include <QDateTime>
#include <QPushButton>

#include "employee.h"

class Employee;

class Task : public QWidget
{
    Q_OBJECT
public:
    Task(QString name, QString descr, QString theme, QColor color, std::vector <Employee *> performers,
         QDateTime deadline, QDateTime created = QDateTime::currentDateTime());

    void setName(QString name) { this->name = name; }
    QString getName() { return name; }

    QString getDescription() { return description; }

    void setThemeName(QString name) { themeName = name; }
    QString getThemeName() { return themeName; }

    QString getColor() { return color.name(); }

    std::vector <Employee *> getPerformers() { return performers; }

    void setDeadline(int y, int mo, int d, int h, int mi, int s);

    QDateTime getCreatedDateTime() { return created; }
    QString getCreatedString() { return created.toString("dd.MM.yy HH:mm"); }

    QDateTime getDeadlineDateTime() { return deadline; }
    QString getDeadlineString() { return deadline.toString("dd.MM.yy HH:mm"); }

    void updateTime();

    QString generateReport();

protected:
    void paintEvent(QPaintEvent *);

private:

    QString name;
    QString description;
    QString themeName;
    QColor color;
    std::vector <Employee *> performers;
    //priority

    QDateTime created;
    QDateTime deadline;

    //GUI
    QPushButton *burger;
    QLabel *nameLabel;
    QLabel *descriptionLabel;
    QLabel *createdLabel;
    QLabel *deadlineLabel;
    QLabel *timeleftLabel;
    QLabel *timeLabel;
    QLabel *perfLabel[3];

public slots:
    void ShowContextMenu(const QPoint& pos);

private slots:
    void changeTheme(QAction *themeName);
    void removeTask();
};

#endif // TASK_H
