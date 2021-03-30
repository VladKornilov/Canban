#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>

#include "ui_mainwindow.h"
#include "task.h"
#include "canban.h"
#include "addtaskdialog.h"
#include "addemployeedialog.h"
#include "addthemedialog.h"
#include "showemployeesdialog.h"
#include "files.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    //QMenu *fileMenu;
    QMenu *setupFileMenu();
    QMenu *setupAddMenu();

    Canban *canban;
    Files *files;

private slots:
    void loadTasks();
    void loadEmployees();
    void saveTasks();
    void saveEmployees();
    void reportTasks();
    void reportEmployees();

    void addTask();
    void addTheme();
    void addEmployee();
    void showEmployees();
};
#endif // MAINWINDOW_H
