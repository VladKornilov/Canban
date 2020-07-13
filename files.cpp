#include <QFileDialog>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTextStream>

#include "files.h"

Files *Files::instance = 0;


std::vector<Task *> Files::loadTasks(QString fileName)
{
    std::vector<Task *> tasks;
    QFile jsonFile(fileName);
    if (!jsonFile.open(QIODevice::ReadOnly)) return tasks;

    QJsonDocument jsonDocument(QJsonDocument::fromJson(jsonFile.readAll()));
    QJsonArray jsonTasks = jsonDocument.array();
    for (auto i : jsonTasks) {
        QJsonObject jsonTask = i.toObject();

        std::vector <Employee *> customers;
        QJsonArray jsonEmpls = jsonTask["performers"].toArray();
        for (auto i : jsonEmpls) {
            QString customer = i.toString();
            Employee *empl = Canban::inst()->getEmployee(customer);
            if (empl != nullptr) {
                customers.push_back(empl);
            }
        }

        Task *task = new Task(jsonTask["name"].toString(),
                              jsonTask["description"].toString(),
                              jsonTask["theme"].toString(),
                              QColor(jsonTask["color"].toString()),
                              customers,
                              QDateTime::fromTime_t(jsonTask["deadline"].toInt()),
                              QDateTime::fromTime_t(jsonTask["created"].toInt()));
        tasks.push_back(task);
    }
    return tasks;
}

bool Files::saveTasks(QString fileName)
{
    QFile jsonFile(fileName);
    if (!jsonFile.open(QIODevice::WriteOnly)) return false;

    QJsonArray jsonTasks;
    for (QString themeName : Canban::inst()->getThemeNames()) {
        Theme *theme = Canban::inst()->getTheme(themeName);
        for (Task *task : theme->getTasks()) {
            QJsonObject jsonTask;
            jsonTask.insert("name", task->getName());
            jsonTask.insert("description", task->getDescription());
            jsonTask.insert("theme", task->getThemeName());
            jsonTask.insert("color", task->getColor());
            jsonTask.insert("created", (int)task->getCreatedDateTime().toTime_t());
            jsonTask.insert("deadline", (int)task->getDeadlineDateTime().toTime_t());

            QJsonArray jsonEmpls;
            for (Employee *empl : task->getPerformers()) {
                jsonEmpls.append(empl->getName());
            }
            jsonTask.insert("performers", jsonEmpls);

            jsonTasks.append(jsonTask);
        }
    }
    jsonFile.write(QJsonDocument(jsonTasks).toJson(QJsonDocument::Compact));
    jsonFile.close();
    return true;
}


std::vector<Employee *> Files::loadEmployees(QString fileName)
{
    std::vector<Employee *> employees;
    QFile jsonFile(fileName);
    if (!jsonFile.open(QIODevice::ReadOnly)) return employees;

    QJsonDocument jsonDocument(QJsonDocument::fromJson(jsonFile.readAll()));
    QJsonArray jsonEmployees = jsonDocument.array();
    for (auto i : jsonEmployees) {
        QJsonObject jsonEmpl = i.toObject();
        QImage photo(jsonEmpl["photo"].toString());
        Employee *empl = new Employee(jsonEmpl["name"].toString(), jsonEmpl["spec"].toString(),
                                      jsonEmpl["info"].toString(), jsonEmpl["hour"].toInt(), photo);
        empl->setOverallSalary(jsonEmpl["overall"].toInt());

        employees.push_back(empl);
    }
    return employees;
}

bool Files::saveEmployees(QString fileName)
{
    QFileInfo info(fileName);
    QDir fileDir = info.dir();
    QFile jsonFile(fileName);

    if (!jsonFile.open(QIODevice::WriteOnly)) return false;

    QJsonArray jsonEmployees;
    for (QString emplName : Canban::inst()->getEmployeeNames()) {
        Employee *empl = Canban::inst()->getEmployee(emplName);

        QString imageFile = fileDir.path() + "/" + emplName + ".png";
        bool saved = empl->getFullPhoto().save(imageFile);
        if (!saved) return false;

        QJsonObject jsonEmpl;
        jsonEmpl.insert("name", empl->getName());
        jsonEmpl.insert("spec", empl->getSpecialization());
        jsonEmpl.insert("info", empl->getPersonalInfo());
        jsonEmpl.insert("hour", empl->getHourSalary());
        jsonEmpl.insert("overall", empl->getOverallSalary());
        jsonEmpl.insert("photo", imageFile);
        jsonEmployees.append(jsonEmpl);
    }

    jsonFile.write(QJsonDocument(jsonEmployees).toJson(QJsonDocument::Compact));
    jsonFile.close();

    return true;
}

bool Files::generateTasksReport(QString fileName)
{
    QFile reportFile(fileName);
    if (!reportFile.open(QIODevice::WriteOnly)) return false;

    QTextStream out(&reportFile);

    for (QString themeName : Canban::inst()->getThemeNames()) {
        out << "**********" + themeName + "**********\n\n";

        Theme *theme = Canban::inst()->getTheme(themeName);
        for (auto tasks : theme->getTasks())
            out << tasks->generateReport() << "\n";
    }
    reportFile.close();
    return true;
}

bool Files::generateEmployeesReport(QString fileName)
{
    QFile reportFile(fileName);
    if (!reportFile.open(QIODevice::WriteOnly)) return false;

    QTextStream out(&reportFile);

    for (QString emplName : Canban::inst()->getEmployeeNames()) {

        Employee *empl = Canban::inst()->getEmployee(emplName);
        out << empl->generateReport() << "\n";
    }
    reportFile.close();
    return true;
}
