#include "theme.h"
#include "canban.h"


Theme::Theme(QWidget *parent, QString name, QColor color) : QWidget(parent)
{
    selectedSort = NAME;

    nameLabel = new QLabel(name);
    nameLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    //nameLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    nameLabel->setStyleSheet("font: bold 16px;");

    tasksLayout = new QVBoxLayout();

    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->addStretch();
    nameLayout->addWidget(nameLabel);
    nameLayout->addStretch();

    burger = new QPushButton();
    burger->setIcon(QIcon("../images/burger.png"));
    burger->setStyleSheet("QAbstractButton { background: rgba(255,255,255,0); }");
    burger->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(burger, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));
    nameLayout->addWidget(burger);

    tasksLayout->addLayout(nameLayout);
    tasksLayout->insertStretch(-1);
    tasksLayout->setContentsMargins(5, 5, 5, 5);
    tasksLayout->setAlignment(Qt::AlignHCenter);

    setLayout(tasksLayout);

    setStyleSheet(QStringLiteral(".Theme { border-radius: 10px; background-color: %1; }").arg(color.name()));
    setContentsMargins(0, 5, 5, 0);

    setMinimumWidth(Canban::TASK_WIDTH + 10);
    setMaximumWidth(Canban::TASK_WIDTH + 10);
    show();
}

QString Theme::getName()
{
    return nameLabel->text();
}

void Theme::addTask(Task *task)
{
    if (getName() == QString("Выполненные")) {
        for (auto performer : task->getPerformers())
            performer->markTaskCompleted(task);
    }
    tasks.push_back(task);
    sortTasks();
}

void Theme::removeTask(Task *task)
{
    for (size_t i = 0; i < tasks.size(); i++) {
        if (tasks[i] == task) {
            tasksLayout->removeWidget(task);
            tasksLayout->update();
            tasks.erase(tasks.begin() + i);
            //delete task;
            break;
        }
    }
    sortTasks();
}

bool Theme::containsTask(Task *task)
{
    for (auto i: tasks)
        if (i == task)
            return true;
    return false;
}

Task *Theme::getTask(QString taskName)
{
    for (auto i : tasks)
        if (i->getName() == taskName)
            return i;
    return nullptr;
}

std::vector<Task *> Theme::getTasks()
{
    return tasks;
}


bool cmpDeadline(Task *a, Task *b) {
    return a->getDeadlineDateTime() < b->getDeadlineDateTime();
}

bool cmpCreated(Task *a, Task *b) {
    return a->getCreatedDateTime() > b->getCreatedDateTime();
}

bool cmpName(Task *a, Task *b) {
    return a->getName() < b->getName();
}

void Theme::sortTasks()
{
    for (size_t i = 0; i < tasks.size(); i++)
        tasksLayout->removeWidget(tasks[i]);

    switch (selectedSort) {
    case DEADLINE:
        sort(cmpDeadline);
        break;
    case CREATED:
        sort(cmpCreated);
        break;
    case NAME:
        sort(cmpName);
        break;
    }
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
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void Theme::sort(bool cmp(Task *, Task *)) {
    for (size_t i = 1; i < tasks.size(); i++)
        for (int j = i; j > 0; j--)
            if (!cmp(tasks[j - 1], tasks[j]))
                std::swap(tasks[j - 1], tasks[j]);
}

void Theme::ShowContextMenu(const QPoint& pos)
{
    QMenu *menu = new QMenu("Menu", this);
    QMenu *sort = new QMenu("Сортировка", menu);

    //QStringList sortNames = Canban::inst()->getThemeNames();
    for (int i = 0; i < sortDescr.size(); i++) {
        QAction *themeAction = new QAction(sortDescr[i]);

        sort->addAction(themeAction);
    }
    connect(sort, SIGNAL(triggered(QAction *)), this, SLOT(changeSort(QAction *)));

    menu->addMenu(sort);

    QAction *moveLeft = new QAction("Переместить влево");
    QAction *moveRight = new QAction("Переместить вправо");
    menu->addAction(moveLeft);
    menu->addAction(moveRight);
    connect(moveLeft, SIGNAL(triggered()), this, SLOT(moveLeft()));
    connect(moveRight, SIGNAL(triggered()), this, SLOT(moveRight()));

    menu->popup(burger->mapToGlobal(pos));
}

void Theme::changeSort(QAction *sortName)
{
    if (sortName->text() == sortDescr[0]) selectedSort = DEADLINE;
    if (sortName->text() == sortDescr[1]) selectedSort = CREATED;
    if (sortName->text() == sortDescr[2]) selectedSort = NAME;
    sortTasks();
}

void Theme::moveLeft()
{
    Canban::inst()->moveTheme(this, -1);
}

void Theme::moveRight()
{
    Canban::inst()->moveTheme(this, 1);
}
