#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QColorDialog>

#include "canban.h"
#include "addtaskdialog.h"

AddTaskDialog::AddTaskDialog()
{
    setWindowTitle("Добавить задачу");
    QLabel *nameLabel = new QLabel("Название задачи");
    QLabel *descrLabel = new QLabel("Описание задачи");

    taskName = new QLineEdit();
    taskDescr = new QTextEdit();
    taskDescr->setFixedHeight(75);


    QLabel *colorLabel = new QLabel("Цвет задачи");
    taskColor.setNamedColor("orange");
    colorButton = new QPushButton();
    colorButton->setStyleSheet(QStringLiteral("background-color: %1; border-radius: 5px;").arg(taskColor.name()));
    connect(colorButton, SIGNAL(released()), this, SLOT(setColor()));

    addPerfButton = new QPushButton("Добавить исполнителя");
    connect(addPerfButton, SIGNAL(released()), this, SLOT(addFirstPerformer()));


    QGroupBox *dateTimeBox = new QGroupBox("Дата и время дедлайна");

    calendar = new QCalendarWidget();
    calendar->setMinimumDate(QDate::currentDate());
    calendar->setSelectedDate(QDate::currentDate());
    timeEdit = new QTimeEdit();
    timeEdit->setTime(QTime::currentTime().addSecs(60));

    QVBoxLayout *dateTimeLayout = new QVBoxLayout();
    dateTimeLayout->addWidget(calendar);
    dateTimeLayout->addWidget(timeEdit);
    dateTimeBox->setLayout(dateTimeLayout);

    QPushButton* btnOk     = new QPushButton("Ok");
    QPushButton* btnCancel = new QPushButton("Cancel");
    connect(btnOk, SIGNAL(clicked()), SLOT(accept()));
    connect(btnCancel, SIGNAL(clicked()), SLOT(reject()));

    QHBoxLayout *btnsLayout = new QHBoxLayout();
    btnsLayout->addWidget(btnOk);
    btnsLayout->addWidget(btnCancel);

    layout = new QGridLayout();
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(taskName, 0, 1);
    layout->addWidget(descrLabel, 1, 0);
    layout->addWidget(taskDescr, 1, 1);
    layout->addWidget(colorLabel, 2, 0);
    layout->addWidget(colorButton, 2, 1);
    layout->addWidget(addPerfButton, 3, 1);
    layout->addWidget(dateTimeBox, 0, 2, 7, 1);
    layout->addLayout(btnsLayout, 7, 1);

    setLayout(layout);

}

std::vector<Employee *> AddTaskDialog::getPerformers()
{
    return performers;
}



void AddTaskDialog::setColor()
{
    taskColor = QColorDialog::getColor(taskColor, nullptr, "Выберите цвет");
    colorButton->setStyleSheet(QStringLiteral("background-color: %1; border-radius: 5px;").arg(taskColor.name()));
}


QHBoxLayout *AddTaskDialog::setupPerfLayout()
{
    performerNames[numOfPerformers] = new QComboBox(this);
    QStringList names = Canban::inst()->getEmployeeNames();
    names.insert(0, "-");
    performerNames[numOfPerformers]->addItems(names);

    performerPhotos[numOfPerformers] = new QLabel();
    connect(performerNames[numOfPerformers], SIGNAL(currentIndexChanged(QString)), this, SLOT(replacePhoto(QString)));

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(performerNames[numOfPerformers]);
    layout->addWidget(performerPhotos[numOfPerformers]);
    numOfPerformers++;
    return layout;
}


void AddTaskDialog::addFirstPerformer()
{
    layout->addWidget(new QLabel("Исполнитель 1"), 3, 0);
    layout->removeWidget(addPerfButton);
    layout->addLayout(setupPerfLayout(), 3, 1);

    disconnect(addPerfButton, SIGNAL(released()), this, SLOT(addFirstPerformer()));
    connect(addPerfButton, SIGNAL(released()), this, SLOT(addSecondPerformer()));
    layout->addWidget(addPerfButton, 4, 1);
}

void AddTaskDialog::addSecondPerformer()
{
    layout->addWidget(new QLabel("Исполнитель 2"), 4, 0);
    layout->removeWidget(addPerfButton);
    layout->addLayout(setupPerfLayout(), 4, 1);
    disconnect(addPerfButton, SIGNAL(released()), this, SLOT(addSecondPerformer()));
    connect(addPerfButton, SIGNAL(released()), this, SLOT(addThirdPerformer()));
    layout->addWidget(addPerfButton, 5, 1);
}

void AddTaskDialog::addThirdPerformer()
{
    layout->addWidget(new QLabel("Исполнитель 3"), 5, 0);
    layout->removeWidget(addPerfButton);
    layout->addLayout(setupPerfLayout(), 5, 1);
    addPerfButton->hide();
}

void AddTaskDialog::replacePhoto(QString perfName)
{
    for (size_t i = 0; i < numOfPerformers; i++)
        if (performerNames[i]->currentText() == perfName) {
            Employee *emp = Canban::inst()->getEmployee(perfName);
            if (emp != nullptr) {
                performerPhotos[i]->setPixmap(QPixmap::fromImage(emp->getSmallPhoto()));
                if (i == performers.size())
                    performers.push_back(emp);
                else performers[i] = emp;
            }
        }
}
