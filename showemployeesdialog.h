#ifndef SHOWEMPLOYEESDIALOG_H
#define SHOWEMPLOYEESDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>

#include "canban.h"
#include "addemployeedialog.h"

class EmployeeWidget;

class ShowEmployeesDialog : public QDialog
{
    Q_OBJECT
public:
    ShowEmployeesDialog();
    void removeEmpl(EmployeeWidget *widget);

private:
    QVBoxLayout *layout;


};

class EmployeeWidget : public QWidget {
    Q_OBJECT
public:
    EmployeeWidget(ShowEmployeesDialog *parent, QString name);

private:
    ShowEmployeesDialog *parent;
    Employee *employee;

    QLabel *nameLabel;
    QLabel *infoLabel;
    QLabel *specLabel;
    QLabel *hourLabel;
    QLabel *overallLabel;
    QLabel *photo;
    QPushButton *burger;
    QGridLayout *layout;

private slots:
    void ShowContextMenu(const QPoint& pos);
    void editEmployee();
    void fireEmployee();
};

#endif // SHOWEMPLOYEESDIALOG_H
