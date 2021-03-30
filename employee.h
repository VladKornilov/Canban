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

    void setName(QString emplName) { name = emplName; }
    QString getName() { return name; }
    void setSpecialization(QString spec) { specialization = spec; }
    QString getSpecialization() { return specialization; }
    void setPersonalInfo(QString info) { personalInfo = info; }
    QString getPersonalInfo() { return personalInfo; }
    void setHourSalary(int salary) { hourSalary = salary; };
    int getHourSalary() { return hourSalary; }
    void setOverallSalary(int salary) { overallSalary = std::max(0, salary); }
    int getOverallSalary() { return overallSalary; }
    void setPhoto(QImage photo) { this->photo = photo; }
    QImage getFullPhoto() { return photo.scaled(128, 128); }
    QImage getSmallPhoto() { return photo.scaled(32, 32); }
    void addTask(Task *task) { tasks.push_back(task); }
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
