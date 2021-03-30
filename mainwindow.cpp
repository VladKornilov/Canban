#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QTextStream>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Canban Board");
    Canban::create(this);
    Files::create();

    canban = Canban::inst();
    std::vector <Employee *> employees = Files::inst()->loadEmployees("database/savedEmpl.json");
    std::vector <Task *> tasks = Files::inst()->loadTasks("database/savedTasks.json");
    if (!employees.empty() || !tasks.empty()) {
        QMessageBox loadBox;
       loadBox.setWindowTitle("Загрузка");
       loadBox.setIcon(QMessageBox::Question);
       loadBox.setText("Вы хотите загрузить последнее состояние доски?");
       QPushButton *accept = loadBox.addButton(QString("Да"), QMessageBox::AcceptRole);
       loadBox.addButton(QString("Нет"), QMessageBox::RejectRole);
       loadBox.exec();
       if (loadBox.clickedButton() == accept) {
           for (Employee *empl : employees)
               canban->addNewEmployee(empl);
           for (Task *task : tasks)
               canban->addExistingTask(task);
       }
    }


    menuBar()->addMenu(setupFileMenu());
    menuBar()->addMenu(setupAddMenu());
    QAction *emplList = new QAction("Список сотрудников");
    menuBar()->addAction(emplList);
    connect(emplList, SIGNAL(triggered()), this, SLOT(showEmployees()));


//    QHBoxLayout *mainLayout = new QHBoxLayout();
//    mainLayout->addWidget(task);

//    QWidget *central = new QWidget();
//    central->setLayout(mainLayout);
    setCentralWidget(canban);
    show();
}

MainWindow::~MainWindow()
{
    Files::inst()->saveEmployees("database/savedEmpl.json");
    Files::inst()->saveTasks("database/savedTasks.json");

    delete ui;
}


QMenu *MainWindow::setupFileMenu() {
    QMenu *fileMenu = new QMenu("Файл");
    QAction *loadTasks = new QAction("Импорт задач");
    QAction *loadEmployees = new QAction("Импорт сотрудников");
    QAction *saveTasks = new QAction("Экспорт задач");
    QAction *saveEmployees = new QAction("Экспорт сотрудников");
    QAction *reportTasks = new QAction("Создать отчет о задачах");
    QAction *reportEmployees = new QAction("Создать отчет по сотрудникам");

    fileMenu->addAction(loadTasks);
    fileMenu->addAction(loadEmployees);
    fileMenu->addSeparator();
    fileMenu->addAction(saveTasks);
    fileMenu->addAction(saveEmployees);
    fileMenu->addSeparator();
    fileMenu->addAction(reportTasks);
    fileMenu->addAction(reportEmployees);
    connect(loadTasks, SIGNAL(triggered()), SLOT(loadTasks()));
    connect(loadEmployees, SIGNAL(triggered()), SLOT(loadEmployees()));
    connect(saveTasks, SIGNAL(triggered()), SLOT(saveTasks()));
    connect(saveEmployees, SIGNAL(triggered()), SLOT(saveEmployees()));
    connect(reportTasks, SIGNAL(triggered()), SLOT(reportTasks()));
    connect(reportEmployees, SIGNAL(triggered()), SLOT(reportEmployees()));
    return fileMenu;
}

QMenu *MainWindow::setupAddMenu() {
    QMenu *addMenu = new QMenu("Добавить");
    QAction *addTask = new QAction("Добавить задачу", this);
    QAction *addTheme = new QAction("Добавить колонку");
    QAction *addEmployee = new QAction("Добавить сотрудника");
    addMenu->addAction(addTask);
    addMenu->addAction(addTheme);
    addMenu->addAction(addEmployee);
    connect(addTask, SIGNAL(triggered()), SLOT(addTask()));
    connect(addTheme, SIGNAL(triggered()), SLOT(addTheme()));
    connect(addEmployee, SIGNAL(triggered()), SLOT(addEmployee()));
    return addMenu;
}

void MainWindow::loadTasks()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Выберите файл с сохраненными задачами", "", "JSON (*.json)");
    if (!fileName.isEmpty()) {
        std::vector <Task *> tasks = Files::inst()->loadTasks(fileName);
        for (Task *task : tasks) {
            canban->addExistingTask(task);
        }
    }
}

void MainWindow::loadEmployees()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Выберите файл с сохраненными сотрудниками", "", "JSON (*.json)");
    if (!fileName.isEmpty()) {
        std::vector <Employee *> employees = Files::inst()->loadEmployees(fileName);
        for (Employee *empl : employees) {
            canban->addNewEmployee(empl);
        }
    }
}

void MainWindow::saveTasks()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Выберите файл для сохранения", "tasks", "JSON (*.json)");
    if (!fileName.isEmpty()) {
        Files::inst()->saveTasks(fileName);
    }
}

void MainWindow::saveEmployees()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Выберите файл для сохранения", "employees", "JSON (*.json)");
    if (!fileName.isEmpty()) {
        bool ok = Files::inst()->saveEmployees(fileName);
        if (!ok) {
            QMessageBox errorBox;
            errorBox.setWindowTitle("Error!");
            errorBox.setIcon(QMessageBox::Critical);
            errorBox.setText("Ошибка при сохранении сотрудников");
            errorBox.exec();
        }
    }
}

void MainWindow::reportTasks()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Выберите файл для сохранения отчета", "tasks_report", "TXT (*.txt)");
    if (!fileName.isEmpty()) {
        Files::inst()->generateTasksReport(fileName);
    }
}

void MainWindow::reportEmployees()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Выберите файл для сохранения отчета", "employees_report", "TXT (*.txt)");
    if (!fileName.isEmpty()) {
        Files::inst()->generateEmployeesReport(fileName);
    }
}


void MainWindow::addTask()
{
    AddTaskDialog *dialog = new AddTaskDialog();
    if (dialog->exec() == QDialog::Accepted) {
        QString taskName = dialog->getTaskName();
        if (!taskName.isEmpty() && canban->getTask(taskName) == nullptr) {
            Task *task = new Task(taskName, dialog->getTaskDescr(), "",
                                  dialog->getTaskColor(),  dialog->getPerformers(), dialog->getDateTime());
            canban->addNewTask(task);
        }
        else {
            QMessageBox errorBox;
            errorBox.setWindowTitle("Error!");
            errorBox.setIcon(QMessageBox::Critical);
            errorBox.setText("Невозможно создать задачу с таким названием");
            if (taskName.isEmpty()) {
                errorBox.setDetailedText("Пустое название задачи");
            }
            else {
                errorBox.setDetailedText(QStringLiteral("Задача с названием \"%1\" уже существует").arg(taskName));
            }
            errorBox.exec();
        }
    }
}

void MainWindow::addTheme()
{
    AddThemeDialog *dialog = new AddThemeDialog();
    if (dialog->exec() == QDialog::Accepted) {
        QString themeName = dialog->getThemeName();
        if (!themeName.isEmpty() && canban->getTheme(themeName) == nullptr) {
            Theme *theme = new Theme(nullptr, dialog->getThemeName(), dialog->getThemeColor());
            canban->addNewTheme(theme);
        }
        else {
            QMessageBox errorBox;
            errorBox.setWindowTitle("Error!");
            errorBox.setIcon(QMessageBox::Critical);
            errorBox.setText("Невозможно создать колонку с таким названием");
            if (themeName.isEmpty()) {
                errorBox.setDetailedText("Пустое название колонки");
            }
            else {
                errorBox.setDetailedText(QStringLiteral("Колонка с названием \"%1\" уже существует").arg(themeName));
            }
            errorBox.exec();
        }
    }
}

void MainWindow::addEmployee()
{
    AddEmployeeDialog *dialog = new AddEmployeeDialog();
    if (dialog->exec() == QDialog::Accepted) {
        QString emplName = dialog->getEmplName();
        if (!emplName.isEmpty() && canban->getEmployee(emplName) == nullptr) {
            Employee *empl = new Employee(dialog->getEmplName(), dialog->getEmplSpec(),
                                        dialog->getEmplInfo(), dialog->getSalary(), dialog->getPhoto());
            canban->addNewEmployee(empl);
        }
        else {
            QMessageBox errorBox;
            errorBox.setWindowTitle("Error!");
            errorBox.setIcon(QMessageBox::Critical);
            errorBox.setText("Невозможно создать сотрудника");
            if (emplName.isEmpty()) {
                errorBox.setDetailedText("Задано пустое имя сотрудника");
            }
            else {
                errorBox.setDetailedText(QStringLiteral("Сотрудник по имени \"%1\" уже существует").arg(emplName));
            }
            errorBox.exec();
        }
    }
}

void MainWindow::showEmployees()
{
    ShowEmployeesDialog *dialog = new ShowEmployeesDialog();
    dialog->exec();
}





