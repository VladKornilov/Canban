#ifndef THEME_H
#define THEME_H

#include <QWidget>
#include <QPushButton>
#include <QDate>
#include <QString>
#include <QStyleOption>
#include <QPainter>
#include <QPushButton>
#include <QMenu>
#include <vector>



#include "task.h"

class Theme : public QWidget
{
    Q_OBJECT
public:
    Theme(QWidget *parent, QString name, QColor color);
    QString getName();
    void addTask(Task *task);
    void removeTask(Task *task);
    bool containsTask(Task *task);
    Task *getTask(QString taskName);
    std::vector <Task *> getTasks();
    void sortTasks();
    void generateReport();

    void updateTime();

protected:
    void paintEvent(QPaintEvent *);

private:
    void sort(bool cmp(Task *, Task *));

    enum SortType {DEADLINE, CREATED, NAME};
    QStringList sortDescr = (QStringList() << "Дедлайн" << "Дата создания" << "Название");

    SortType selectedSort;
    QLabel *nameLabel;
    QPushButton *burger;
    QVBoxLayout *tasksLayout;
    std::vector <Task*> tasks;

public slots:
    void ShowContextMenu(const QPoint& pos);

private slots:
    void changeSort(QAction *sortName);
    void moveLeft();
    void moveRight();
};

#endif // THEME_H
