#include "employee.h"



Employee::Employee(QString name, QString spec, QString info, int salary, QImage photo)
{
    this->name = name;
    specialization = spec;
    personalInfo = info;
    hourSalary = salary;
    overallSalary = 0;
    this->photo = photo;
}


QImage Employee::getFullPhoto()
{
    return photo.scaled(256, 256);
}

QImage Employee::getSmallPhoto()
{
    return photo.scaled(32, 32);
}

void Employee::addTask(Task *task)
{
    tasks.push_back(task);
}

void Employee::markTaskCompleted(Task *task)
{
    auto pos = std::find(tasks.begin(), tasks.end(), task);
    if (pos != tasks.end()) {
        tasks.erase(pos);
        completedTasks.push_back(task->getName());
        int hours = task->getCreatedDateTime().secsTo(task->getDeadlineDateTime()) / 3600;
        overallSalary += hours * hourSalary;
    }
}

QString fillString1(QString symbols, int count) {
    QString str;
    while (count--) str += symbols;
    return str;
}

QString Employee::generateReport()
{
    int reportWidth = 70;
    QString report = fillString1("=", reportWidth) + "\n";
    report += "|" + fillString1(" ", reportWidth - 2) + "|\n";
    report += "| " + QString("Имя сотрудника: ") + name + fillString1(" ", reportWidth - 3 - 16 - name.length()) + "|\n";
    report += "|" + fillString1(" ", reportWidth - 2) + "|\n";
    report += "| " + QString("Должность: ") + specialization + fillString1(" ", reportWidth - 3 - 11 - specialization.length()) + "|\n";
    report += "|" + fillString1(" ", reportWidth - 2) + "|\n";
    report += "| " + QString("Заметка: ") + fillString1(" ", reportWidth - 3 - 9) + "|\n";

    int descPos = 0;
    while (descPos < personalInfo.length()) {
        QString line = personalInfo.mid(descPos, reportWidth - 4);
        descPos += line.length();
        report += "| " + line + fillString1(" ", reportWidth - 3 - line.length()) + "|\n";
    }
    report += "|" + fillString1(" ", reportWidth - 2) + "|\n";

    report += "| " + QString("Часовая оплата: $") + QString::number(hourSalary) + fillString1(" ", reportWidth - 3 - 17 - QString::number(hourSalary).length()) + "|\n";
    report += "|" + fillString1(" ", reportWidth - 2) + "|\n";
    report += "| " + QString("Общий доход:    $") + QString::number(overallSalary) + fillString1(" ", reportWidth - 3 - 17 - QString::number(overallSalary).length()) + "|\n";
    report += "|" + fillString1(" ", reportWidth - 2) + "|\n";


    report += "| " + QString("Активные задачи: ") + QString::number(tasks.size()) + "\n";
    QString activeTasks;
    for (auto task : tasks) {
        activeTasks += task->getName() + "; ";
    }
    report += "| " + activeTasks + "\n";
    report += "|" + fillString1(" ", reportWidth - 2) + "|\n";

    report += "| " + QString("Выполненные задачи: ") + QString::number(completedTasks.size()) + "\n";
    QString complTasks;
    for (auto task : completedTasks) {
        complTasks += task + "; ";
    }
    report += "| " + complTasks + "\n";
    report += "|" + fillString1(" ", reportWidth - 2) + "|\n";

    report += fillString1("=", reportWidth) + "\n";
    return report;
}
