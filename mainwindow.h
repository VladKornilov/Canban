#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>

#include "ui_mainwindow.h"
#include "task.h"
#include "canban.h"
#include "addtaskdialog.h"
#include "addemployeedialog.h"

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

    QMenu *fileMenu;
    void setupFileMenu();
    QMenu *addMenu;
    void setupAddMenu();

    Canban *canban;

private slots:
    void addTask();
    void addTheme();
    void addEmployee();
    void updateTime();
};
#endif // MAINWINDOW_H
