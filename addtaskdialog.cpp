#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QColorDialog>
#include <algorithm>

#include "canban.h"
#include "addtaskdialog.h"


AddTaskDialog::AddTaskDialog(QString name, QString descr, QColor color, std::vector <Employee *> taskPerformers, QDateTime deadline)
{
    if (name.isEmpty())
        setWindowTitle("Добавить задачу");
    else
        setWindowTitle("Редактировать задачу");
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    QLabel *nameLabel = new QLabel("Название задачи");
    QLabel *descrLabel = new QLabel("Описание задачи");

    taskName = new QLineEdit(name);
    taskDescr = new QTextEdit(descr);
    taskDescr->setFixedHeight(75);


    QLabel *colorLabel = new QLabel("Цвет задачи");
    taskColor = color;
    colorButton = new QPushButton();
    colorButton->setStyleSheet(QStringLiteral("background-color: %1; border-radius: 5px;").arg(taskColor.name()));
    connect(colorButton, SIGNAL(released()), this, SLOT(setColor()));

    QGroupBox *dateTimeBox = new QGroupBox("Дата и время дедлайна");

    calendar = new QCalendarWidget();
    calendar->setMinimumDate(QDate::currentDate());
    calendar->setSelectedDate(deadline.date());
    timeEdit = new QTimeEdit();
    timeEdit->setTime(deadline.time());

    QVBoxLayout *dateTimeLayout = new QVBoxLayout();
    dateTimeLayout->addWidget(calendar);
    dateTimeLayout->addWidget(timeEdit);
    dateTimeBox->setLayout(dateTimeLayout);

    QPushButton* btnOk     = new QPushButton("Сохранить");
    QPushButton* btnCancel = new QPushButton("Отмена");
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

    performers = taskPerformers;
    for (size_t i = 0; i < performers.size(); i++) {
        layout->addWidget(new QLabel("Исполнитель " + QString::number(i + 1)), 3 + i, 0);
        layout->addLayout(setupPerfLayout(i, performers[i]->getName()), 3 + i, 1);
    }
    if (performers.size() < 3) {
        addPerfButton = new QPushButton("Добавить исполнителя");

        switch (performers.size()) {
        case 0:
            connect(addPerfButton, SIGNAL(released()), this, SLOT(addFirstPerformer()));
            layout->addWidget(addPerfButton, 3, 1);
            break;
        case 1:
            connect(addPerfButton, SIGNAL(released()), this, SLOT(addSecondPerformer()));
            layout->addWidget(addPerfButton, 4, 1);
            break;
        case 2:
            connect(addPerfButton, SIGNAL(released()), this, SLOT(addThirdPerformer()));
            layout->addWidget(addPerfButton, 5, 1);
            break;
        }
    }
    layout->addWidget(dateTimeBox, 0, 2, 7, 1);
    layout->addLayout(btnsLayout, 7, 1);

    setLayout(layout);
}

std::vector<Employee *> AddTaskDialog::getPerformers()
{
    for (size_t i = 0; i < performers.size(); i++) {
        for (size_t j = i + 1; j < performers.size(); j++)
            if (performers[i] == performers[j]) performers[j] = nullptr;
    }

    auto i = std::find(performers.begin(), performers.end(), nullptr);
    while (i != performers.end()) {
        performers.erase(i);
        i = std::find(performers.begin(), performers.end(), nullptr);
    }


    return performers;
}



void AddTaskDialog::setColor()
{
    taskColor = QColorDialog::getColor(taskColor, nullptr, "Выберите цвет");
    colorButton->setStyleSheet(QStringLiteral("background-color: %1; border-radius: 5px;").arg(taskColor.name()));
}


QHBoxLayout *AddTaskDialog::setupPerfLayout(int id, QString selected)
{
    if ((size_t)id + 1 > numOfPerformers)
        numOfPerformers = id + 1;

    performerNames[id] = new QComboBox(this);
    QStringList names = Canban::inst()->getEmployeeNames();
    names.insert(0, "-");
    performerNames[id]->addItems(names);

    performerPhotos[id] = new QLabel();
    connect(performerNames[id], SIGNAL(currentIndexChanged(QString)), this, SLOT(replacePhoto(QString)));
    if (!selected.isEmpty()) {
        performerNames[id]->setCurrentIndex(names.indexOf(selected));
        replacePhoto(selected);
    }

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(performerNames[id]);
    layout->addWidget(performerPhotos[id]);

    return layout;
}


void AddTaskDialog::addFirstPerformer()
{
    layout->addWidget(new QLabel("Исполнитель 1"), 3, 0);
    performers.push_back(nullptr);
    layout->removeWidget(addPerfButton);
    layout->addLayout(setupPerfLayout(0), 3, 1);

    disconnect(addPerfButton, SIGNAL(released()), this, SLOT(addFirstPerformer()));
    connect(addPerfButton, SIGNAL(released()), this, SLOT(addSecondPerformer()));
    layout->addWidget(addPerfButton, 4, 1);
}

void AddTaskDialog::addSecondPerformer()
{
    layout->addWidget(new QLabel("Исполнитель 2"), 4, 0);
    performers.push_back(nullptr);
    layout->removeWidget(addPerfButton);
    layout->addLayout(setupPerfLayout(1), 4, 1);
    disconnect(addPerfButton, SIGNAL(released()), this, SLOT(addSecondPerformer()));
    connect(addPerfButton, SIGNAL(released()), this, SLOT(addThirdPerformer()));
    layout->addWidget(addPerfButton, 5, 1);
}

void AddTaskDialog::addThirdPerformer()
{
    layout->addWidget(new QLabel("Исполнитель 3"), 5, 0);
    performers.push_back(nullptr);
    layout->removeWidget(addPerfButton);
    layout->addLayout(setupPerfLayout(2), 5, 1);
    addPerfButton->hide();
}

void AddTaskDialog::replacePhoto(QString perfName)
{
    for (size_t i = 0; i < numOfPerformers; i++)
        if (performerNames[i]->currentText() == perfName) {
            Employee *emp = Canban::inst()->getEmployee(perfName);
            if (emp != nullptr) {
                performers[i] = emp;
                performerPhotos[i]->setPixmap(QPixmap::fromImage(emp->getSmallPhoto()));
            }
            else {
                performers[i] = nullptr;
                performerPhotos[i]->clear();
            }
        }
}
