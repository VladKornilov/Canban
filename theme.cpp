#include <QStyleOption>
#include <QPainter>
#include <QPushButton>

#include "canban.h"
#include "theme.h"
#include "task.h"

Theme::Theme(QWidget *parent, QString name, QColor *color) : QWidget(parent)
{
    selectedSort = DEADLINE;

    nameLabel = new QLabel(name);
    nameLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    //nameLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    nameLabel->setStyleSheet("font: bold 16px;");

    tasksLayout = new QVBoxLayout();

    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->addStretch();
    nameLayout->addWidget(nameLabel);
    nameLayout->addStretch();

    QPushButton *burger = new QPushButton();
    burger->setIcon(QIcon("images/burger.png"));
    burger->setStyleSheet("QAbstractButton { background: rgba(255,255,255,0); } QAbstractButton:pressed {border: 2px solid #000000; }");
    connect(burger, SIGNAL(released()), this, SLOT(pressBurger()));

    nameLayout->addWidget(burger);

    tasksLayout->addLayout(nameLayout);

    tasksLayout->insertStretch(-1);


    tasksLayout->setMargin(5);
    tasksLayout->setAlignment(Qt::AlignHCenter);

    setLayout(tasksLayout);
    setStyleSheet(QStringLiteral(".Theme { border-radius: 10px; background-color: %1; }").arg(color->name()));
    setContentsMargins(0, 5, 5, 0);

    setMinimumWidth(Canban::TASK_WIDTH + 10);
    setMaximumWidth(Canban::TASK_WIDTH + 10);


    show();

}

void Theme::addTask(Task *task)
{
    tasks.push_back(task);
    sortTasks();
}

void Theme::removeTask(Task *task)
{
    for (size_t i = 0; i < tasks.size(); i++)
        if (tasks[i] == task) {
            tasks.erase(tasks.begin() + i);
            break;
        }
    sortTasks();
}


bool cmpDeadline(Task *a, Task *b) {
    return a->getDeadlineDateTime() < b->getCreatedDateTime();
}

bool cmpCreated(Task *a, Task *b) {
    return a->getCreatedDateTime() > b->getCreatedDateTime();
}

bool cmpName(Task *a, Task *b) {
    return a->getName() < b->getName();
}

bool cmpPriority(Task &a, Task &b) {
    //TODO
    a.getName();
    b.getName();
    return true;
}

void Theme::sortTasks()
{
    switch (selectedSort) {
    case DEADLINE:
       // std::sort(tasks.begin(), tasks.end(), cmpDeadline);
        break;
    case CREATED:
        //std::sort(tasks.begin(), tasks.end(), cmpCreated);
        break;
    case NAME:
        //std::sort(tasks.begin(), tasks.end(), cmpName);
        break;
    case PRIORITY:
        //std::sort(tasks.begin(), tasks.end(), cmpPriority);
        break;
    }
    for (size_t i = 0; i < tasks.size(); i++)
        tasksLayout->removeWidget(tasks[i]);
    for (size_t i = 0; i < tasks.size(); i++)
        tasksLayout->insertWidget(tasksLayout->count()-1, tasks[i]);

}

void Theme::updateTime()
{
    for (Task *task : tasks)
        task->updateTime();
}

void Theme::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

#include <iostream>

void Theme::pressBurger()
{
    std::cout << "toggled" << std::endl;
}
