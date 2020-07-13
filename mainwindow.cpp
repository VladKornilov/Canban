#include <QTimer>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Canban Board");
    Canban::create(this);
    Files::create();

    setupFileMenu();
    setupAddMenu();

    canban = Canban::inst();


    QTimer *timeTimer = new QTimer();
    timeTimer->setInterval(1000);
    connect(timeTimer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timeTimer->start();


//    QHBoxLayout *mainLayout = new QHBoxLayout();
//    mainLayout->addWidget(task);

//    QWidget *central = new QWidget();
//    central->setLayout(mainLayout);
    setCentralWidget(canban);
    show();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setupFileMenu() {
    fileMenu = menuBar()->addMenu("Файл");
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
}

void MainWindow::setupAddMenu() {
    addMenu = menuBar()->addMenu("Добавить");
    QAction *addTask = new QAction("Добавить задачу", this);
    QAction *addTheme = new QAction("Добавить колонку");
    QAction *addEmployee = new QAction("Добавить сотрудника");
    addMenu->addAction(addTask);
    addMenu->addAction(addTheme);
    addMenu->addAction(addEmployee);
    connect(addTask, SIGNAL(triggered()), SLOT(addTask()));
    connect(addTheme, SIGNAL(triggered()), SLOT(addTheme()));
    connect(addEmployee, SIGNAL(triggered()), SLOT(addEmployee()));
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
        if (!taskName.isEmpty() && Canban::inst()->getTask(taskName) == nullptr) {
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
    bool ok;
    QString themeName = QInputDialog::getText(this, "Добавить тему", "Название темы: ", QLineEdit::Normal, "", &ok);
    if (ok) {
        if (!themeName.isEmpty() && Canban::inst()->getTheme(themeName) == nullptr) {
            Canban::inst()->addNewTheme(new Theme(nullptr, themeName, new QColor("white")));
        }
        else {
            QMessageBox errorBox;
            errorBox.setWindowTitle("Error!");
            errorBox.setIcon(QMessageBox::Critical);
            errorBox.setText("Невозможно создать тему с таким названием");
            if (themeName.isEmpty()) {
                errorBox.setDetailedText("Пустое название темы");
            }
            else {
                errorBox.setDetailedText(QStringLiteral("Тема с названием \"%1\" уже существует").arg(themeName));
            }
            errorBox.exec();
        }
    }

}

void MainWindow::addEmployee()
{
    AddEmployeeDialog *dialog = new AddEmployeeDialog();
    if (dialog->exec() == QDialog::Accepted) {
        Employee *empl = new Employee(dialog->getEmplName(), dialog->getEmplSpec(),
                                      dialog->getEmplInfo(), dialog->getSalary(), dialog->getPhoto());
        canban->addNewEmployee(empl);
    }
}

void MainWindow::updateTime()
{
    canban->updateTime();
}



