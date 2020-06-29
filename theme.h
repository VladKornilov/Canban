#ifndef DAY_H
#define DAY_H

#include <QWidget>
#include <vector>
#include <QDate>
#include <QString>
#include "task.h"



class Theme : public QWidget
{
    Q_OBJECT
public:
    //Theme(QWidget *parent);
    Theme(QWidget *parent, QString name, QColor *color);
    void addTask(Task *task);
    void removeTask(Task *task);
    void sortTasks();
    void generateReport();

    void updateTime();

protected:
    void paintEvent(QPaintEvent *);

private:
    enum SortType {DEADLINE, CREATED, NAME, PRIORITY};
    QString sortDescr[4] = {
        QStringLiteral(u"Дедлайн"),
        QStringLiteral(u"Дата создания"),
        QStringLiteral(u"Название"),
        QStringLiteral(u"Приоритет")};

    SortType selectedSort;
    QLabel *nameLabel;
    QVBoxLayout *tasksLayout;
    std::vector <Task*> tasks;

private slots:
    void pressBurger();
};

#endif // DAY_H
