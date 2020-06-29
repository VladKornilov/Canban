#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>
#include <QImage>

#include <vector>

//#include "task.h"

class Employee
{
public:
    Employee(QString name, QString spec, QString info, int salary, QImage photo);

    QString getName() {return name; }
    QString getPersonalInfo() { return personalInfo; }
    int getHourSalary() { return hourSalary; }
    QString getSpecialization() { return specialization; }
    QImage getFullPhoto();
    QImage getSmallPhoto();

private:
    QString name;
    QString specialization;
    QString personalInfo;
    int hourSalary;
    QImage photo;
    //std::vector <Task *> tasks;

};

#endif // EMPLOYEE_H
