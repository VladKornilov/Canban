#include <QColor>

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
        themes.push_back(new Theme(this, name, new QColor(colors[i++])));
    }

    themesLayout = new QHBoxLayout();


    for (size_t i = 0; i < themes.size(); i++) {
        //QScrollArea *themeScroll = new QScrollArea();
        //themeScroll->setWidget(themes[i]);
        //themeScroll->ho
        themesLayout->addWidget(themes[i]);
        //themesLayout->addWidget(themeScroll);
    }
    themesLayout->addStretch(-1);

    setLayout(themesLayout);
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
            themes[i]->addTask(task);
            for (auto perf : task->getPerformers())
                perf->addTask(task);
            return;
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
    for (auto i : themes) {
        i->removeTask(i->getTask(taskName));
    }
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


