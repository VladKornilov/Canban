#include <QColor>
#include <QTimer>

#include "canban.h"

Canban *Canban::instance = 0;

Canban::Canban(QWidget *parent) : QWidget(parent)
{
    //starting code
    themeNames.push_back("Новые задачи");
    themeNames.push_back("В работе");
    themeNames.push_back("Выполненные");
    QString colors[3] = {"#FFE473", "#B53AD4", "#60D6A9"};
    int i = 0;
    for (auto name : themeNames) {
        themes.push_back(new Theme(this, name, QColor(colors[i++])));
    }

    themesLayout = new QHBoxLayout();

    for (size_t i = 0; i < themes.size(); i++)
        themesLayout->addWidget(themes[i]);

    themesLayout->addStretch(-1);

    setLayout(themesLayout);

    QTimer *timeTimer = new QTimer();
    timeTimer->setInterval(1000);
    connect(timeTimer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timeTimer->start();
}

void Canban::addNewTheme(Theme *theme)
{
    themeNames.push_back(theme->getName());
    themes.push_back(theme);
    themesLayout->insertWidget(themesLayout->count() - 1, theme);
}

void Canban::addNewTask(Task *task)
{
    for (int i = 0; i < themeNames.size(); i++) {
        if (themeNames[i] == "Новые задачи") {
            task->setThemeName(themeNames[i]);
            themes[i]->addTask(task);
            for (auto perf : task->getPerformers())
                perf->addTask(task);

            return;
        }
    }

}

void Canban::addExistingTask(Task *task)
{
    for (int i = 0; i < themeNames.size(); i++) {
        if (themeNames[i] == task->getThemeName()) {
            if (themes[i]->getTask(task->getName()) == nullptr) {
                themes[i]->addTask(task);
                for (auto perf : task->getPerformers())
                    perf->addTask(task);
                return;
            }
        }
    }
}

void Canban::moveTask(Task *task, Theme *toTheme)
{
    Theme *fromTheme = nullptr;
    for (auto i : themes) {
        if (i->containsTask(task)) {
            fromTheme = i;
            break;
        }
    }
    if (fromTheme != nullptr && toTheme != nullptr) {
        fromTheme->removeTask(task);
        toTheme->addTask(task);
        task->setThemeName(toTheme->getName());
    }
}

void Canban::moveTheme(Theme *theme, int to)
{
    int from = -1;
    for (size_t i = 0; i < themes.size(); i++)
        if (themes[i] == theme) from = i;

    to = from + to;
    if (to < 0 || to >= (int)themes.size()) return;

    themesLayout->removeWidget(themes[from]);
    themesLayout->replaceWidget(themes[to], themes[from]);
    themesLayout->insertWidget(from, themes[to]);
    themesLayout->update();
    std::swap(themes[from], themes[to]);
    std::swap(themeNames[from], themeNames[to]);
}

Task *Canban::getTask(QString taskName)
{
    for (auto i : themes) {
        Task *task = i->getTask(taskName);
        if (task)
            return task;
    }
    return nullptr;
}

void Canban::removeTask(QString taskName)
{
    for (auto i : themes)
        i->removeTask(i->getTask(taskName));
}

void Canban::editEmployee(Employee *empl)
{
    for (auto theme : themes) {
        for (auto task : theme->getTasks()) {
            auto perf = task->getPerformers();
            if (std::find(perf.begin(), perf.end(), empl) != perf.end())
                task->updateVisual();
        }
    }
}

void Canban::fireEmployee(Employee *empl)
{
    for (auto theme : themes)
        for (auto task : theme->getTasks())
            task->removePerformer(empl);

    employees.erase(std::find(employees.begin(), employees.end(), empl));
    delete empl;
}

QStringList Canban::getThemeNames()
{
    return themeNames;
}

Theme *Canban::getTheme(QString name)
{

    for (int i = 0; i < themeNames.length(); i++) {
        if (themeNames[i] == name)
            return themes[i];
    }
    return nullptr;
}

QStringList Canban::getEmployeeNames()
{
    QStringList names;
    for (auto i : employees)
        names += i->getName();
    return names;
}

Employee *Canban::getEmployee(QString name)
{
    for (auto i : employees)
        if (i->getName() == name)
            return i;
    return nullptr;
}

void Canban::updateTime()
{
    for (Theme *theme : themes)
        theme->updateTime();
}
