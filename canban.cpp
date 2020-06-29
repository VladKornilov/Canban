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


    for (size_t i = 0; i < themes.size(); i++)
        themesLayout->addWidget(themes[i]);
    themesLayout->addStretch(-1);

    setLayout(themesLayout);
}

void Canban::addNewTask(Task *task)
{
    themes[0]->addTask(task);
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
