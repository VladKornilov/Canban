#ifndef CANBAN_H
#define CANBAN_H

#include <QGraphicsView>
#include <vector>

#include "theme.h"
#include "employee.h"



class Canban : public QWidget
{
    Q_OBJECT
public:
    static void create(QWidget *parent) {
        instance = new Canban(parent);
    }
    static Canban *inst() {
        return instance;
    }
    void addNewTheme(Theme *theme);
    void addNewTask(Task *task);
    void addExistingTask(Task *task);
    void moveTask(Task *task, Theme *toTheme);
    void moveTheme(Theme *theme, int to);
    Task *getTask(QString taskName);
    void removeTask(QString taskName);
    void addNewEmployee(Employee *empl) { employees.push_back(empl); }
    void editEmployee(Employee *empl);
    void fireEmployee(Employee *empl);
    QStringList getThemeNames();
    Theme *getTheme(QString name);
    QStringList getEmployeeNames();
    Employee *getEmployee(QString name);

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
    QStringList themeNames;

    std::vector <Employee *> employees;

private slots:
    void updateTime();
};


#endif // CANBAN_H
