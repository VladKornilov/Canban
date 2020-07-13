#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>
#include <QImage>

#include <vector>

#include "task.h"

class Task;

class Employee
{
public:
    Employee(QString name, QString spec, QString info, int salary, QImage photo);

    QString getName() {return name; }
    QString getSpecialization() { return specialization; }
    QString getPersonalInfo() { return personalInfo; }
    int getHourSalary() { return hourSalary; }
    void setOverallSalary(int salary) { overallSalary = std::max(0, salary); }
    int getOverallSalary() { return overallSalary; }
    QImage getFullPhoto();
    QImage getSmallPhoto();
    void addTask(Task *task);
    void markTaskCompleted(Task *task);

    QString generateReport();

private:
    QString name;
    QString specialization;
    QString personalInfo;
    int hourSalary;
    int overallSalary;
    QImage photo;
    std::vector <Task *> tasks;
    std::vector <QString> completedTasks;

};

#endif // EMPLOYEE_H
