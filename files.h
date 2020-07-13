#ifndef FILES_H
#define FILES_H

#include <QObject>
#include <QWidget>

#include "canban.h"
#include "task.h"
#include "employee.h"

class Files
{
public:
    static void create() { instance = new Files(); }
    static Files *inst() { return instance; }

    std::vector <Task *> loadTasks(QString fileName);
    bool saveTasks(QString fileName);

    std::vector <Employee *> loadEmployees(QString fileName);
    bool saveEmployees(QString fileName);

    bool generateTasksReport(QString fileName);
    bool generateEmployeesReport(QString fileName);

private:
    static Files *instance;

    Files(){};

};

#endif // FILES_H
