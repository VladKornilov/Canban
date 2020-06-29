#ifndef CANBAN_H
#define CANBAN_H

#include <QGraphicsView>
#include <vector>
#include "theme.h"
#include "employee.h"



class Canban : public QWidget
{
    //Q_OBJECT
public:
    static void create(QWidget *parent) {
        instance = new Canban(parent);
    }
    static Canban *inst() {
        return instance;
    }
    void addNewTask(Task *task);
    void addNewEmployee(Employee *empl) { employees.push_back(empl); }
    QStringList getEmployeeNames();
    Employee *getEmployee(QString name);
    void updateTime();

    static const int TASK_WIDTH = 250;
    const QStringList specialities = {"Junior-программист",
                                     "Middle-программист",
                                     "Senior-программист",
                                     "Project-менеджер"};

private:
    Canban(QWidget *parent);
    static Canban *instance;

    QHBoxLayout *themesLayout;

    std::vector <Theme *> themes;
    std::vector <QString> themeNames;

    std::vector <Employee *> employees;
};


#endif // CANBAN_H
