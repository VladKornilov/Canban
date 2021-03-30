
#include <QScrollArea>
#include <QScrollBar>
#include <QMenu>
#include <QMessageBox>

#include "showemployeesdialog.h"

ShowEmployeesDialog::ShowEmployeesDialog()
{
    setWindowTitle("Список сотрудников");
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    layout = new QVBoxLayout();
    for (auto name : Canban::inst()->getEmployeeNames()) {
        EmployeeWidget *empl = new EmployeeWidget(this, name);
        empl->setStyleSheet(".EmployeeWidget { background-color: yellow;"
                                              "border-radius: 10px; "
                                              "border: 4px solid; }");
        layout->addWidget(empl);
    }
    layout->addStretch();

    QScrollArea *scroll = new QScrollArea();
    QWidget *layoutWidget = new QWidget();
    layoutWidget->setLayout(layout);
    scroll->setWidget(layoutWidget);
    scroll->horizontalScrollBar()->setEnabled(false);
    scroll->setMinimumSize(400, 500);
    QVBoxLayout *tmpLayout = new QVBoxLayout();
    tmpLayout->addWidget(scroll);

    setLayout(tmpLayout);
}

void ShowEmployeesDialog::removeEmpl(EmployeeWidget *widget)
{
    layout->removeWidget(widget);
    update();
}


EmployeeWidget::EmployeeWidget(ShowEmployeesDialog *parent, QString name) : QWidget(parent)
{
    this->parent = parent;
    employee = Canban::inst()->getEmployee(name);

    nameLabel = new QLabel("Имя: " + employee->getName());
    infoLabel = new QLabel("Заметка: " + employee->getPersonalInfo());
    infoLabel->setWordWrap(true);
    specLabel = new QLabel("Специализация: " + employee->getSpecialization());
    hourLabel = new QLabel("Оплата за час: $" + QString::number(employee->getHourSalary()));
    overallLabel = new QLabel("Заработано: $" + QString::number(employee->getOverallSalary()));
    photo = new QLabel();
    photo->setPixmap(QPixmap::fromImage(employee->getFullPhoto()));

    burger = new QPushButton();
    burger->setIcon(QIcon("images/burger.png"));
    burger->setStyleSheet("QAbstractButton { background: rgba(255,255,255,0); }");
    burger->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(burger, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));


    layout = new QGridLayout();
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(infoLabel, 1, 0);
    layout->addWidget(specLabel, 2, 0);
    layout->addWidget(hourLabel, 3, 0);
    layout->addWidget(overallLabel, 4, 0);
    layout->addWidget(photo, 0, 1, 5, 1);
    layout->addWidget(burger, 0, 2);

    setLayout(layout);
}

void EmployeeWidget::ShowContextMenu(const QPoint &pos)
{
    QMenu *menu = new QMenu("Menu", this);

    QAction *edit = new QAction("Редактировать");
    connect(edit, SIGNAL(triggered()), this, SLOT(editEmployee()));
    menu->addAction(edit);

    QAction *remove = new QAction("Уволить");
    connect(remove, SIGNAL(triggered()), this, SLOT(fireEmployee()));
    menu->addAction(remove);

    menu->popup(burger->mapToGlobal(pos));
}

void EmployeeWidget::editEmployee()
{
    AddEmployeeDialog *dialog = new AddEmployeeDialog(employee->getName(),
                                                      employee->getPersonalInfo(),
                                                      employee->getSpecialization(),
                                                      QString::number(employee->getHourSalary()),
                                                      employee->getFullPhoto());
    if (dialog->exec() == QDialog::Accepted) {
        QString emplName = dialog->getEmplName();
        if (!emplName.isEmpty() && (Canban::inst()->getEmployee(emplName) == employee || Canban::inst()->getEmployee(emplName) == nullptr)) {
            employee->setName(dialog->getEmplName());
            employee->setPersonalInfo(dialog->getEmplInfo());
            employee->setSpecialization(dialog->getEmplSpec());
            employee->setHourSalary(dialog->getSalary());
            employee->setPhoto(dialog->getPhoto());

            nameLabel->setText("Имя: " + employee->getName());
            infoLabel->setText("Заметка: " + employee->getPersonalInfo());
            specLabel->setText("Специализация: " + employee->getSpecialization());
            hourLabel->setText("Оплата за час: $" + QString::number(employee->getHourSalary()));
            overallLabel->setText("Заработано: $" + QString::number(employee->getOverallSalary()));
            photo->setPixmap(QPixmap::fromImage(employee->getFullPhoto()));
            Canban::inst()->editEmployee(employee);
        }
        else {
            QMessageBox errorBox;
            errorBox.setWindowTitle("Error!");
            errorBox.setIcon(QMessageBox::Critical);
            errorBox.setText("Невозможно изменить информацию о сотруднике");
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

void EmployeeWidget::fireEmployee()
{
    Canban::inst()->fireEmployee(employee);
    parent->removeEmpl(this);
    delete this;
}
