#include <QTimer>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Canban::create(this);

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


void MainWindow::addTask()
{
    AddTaskDialog *dialog = new AddTaskDialog();
    if (dialog->exec() == QDialog::Accepted) {
        Task *task = new Task(dialog->getTaskName(), dialog->getTaskDescr(),
                              dialog->getTaskColor(),  dialog->getPerformers(), dialog->getDateTime());
        canban->addNewTask(task);
    }
}

void MainWindow::addTheme()
{

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



