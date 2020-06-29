#include "employee.h"



Employee::Employee(QString name, QString spec, QString info, int salary, QImage photo)
{
    this->name = name;
    specialization = spec;
    personalInfo = info;
    hourSalary = salary;
    this->photo = photo;
}


QImage Employee::getFullPhoto()
{
    return photo;
}

QImage Employee::getSmallPhoto()
{
    return photo.scaled(32, 32);
}
