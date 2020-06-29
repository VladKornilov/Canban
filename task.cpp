#include <QStyleOption>
#include <QPainter>
#include <QPushButton>

#include "canban.h"
#include "task.h"


Task::Task(QString taskName, QString taskDescr, QColor taskColor,
           std::vector <Employee *> performers, QDateTime deadline, QDateTime created) : QWidget(nullptr)
{
    this->created = created;
    this->deadline = deadline;
    name = taskName;
    description = taskDescr;

    setMaximumSize(Canban::TASK_WIDTH, 150);
    setMinimumSize(Canban::TASK_WIDTH, 120);


    QHBoxLayout *nameLayout = new QHBoxLayout();

    nameLabel = new QLabel(name);
    nameLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    nameLabel->setStyleSheet("font: bold 14px;");
    nameLayout->addStretch();
    nameLayout->addWidget(nameLabel);
    nameLayout->addStretch();

    QPushButton *burger = new QPushButton();
    burger->setIcon(QIcon("images/burger.png"));
    burger->setStyleSheet("QAbstractButton { background: rgba(255,255,255,0); } QAbstractButton:pressed {border: 2px solid black; }");
    connect(burger, SIGNAL(released()), this, SLOT(pressBurger()));
    nameLayout->addWidget(burger);


    descriptionLabel = new QLabel(description, this);
    descriptionLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    descriptionLabel->setWordWrap(true);

    QString timeText = QStringLiteral(u"Создано: %1\nДедлайн: %2")
            .arg(getCreatedString())
            .arg(getDeadlineString());


    timeLabel = new QLabel();

    timeLabel->setToolTip(QStringLiteral(u"Создано: %1\nДедлайн: %2").arg(getCreatedString()).arg(getDeadlineString()));
    timeLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    QHBoxLayout *perfLayout = new QHBoxLayout();
    perfLayout->addStretch();
    for (size_t i = 0; i < performers.size(); i++) {
        perfLabel[i] = new QLabel();
        perfLabel[i]->setPixmap(QPixmap::fromImage(performers[i]->getSmallPhoto()));
        perfLabel[i]->setToolTip(performers[i]->getName());
        perfLayout->addWidget(perfLabel[i]);
    }

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(nameLayout, 0, 0, 1, 2);
    mainLayout->addWidget(descriptionLabel, 1, 0, 1, 2);
    mainLayout->addWidget(timeLabel, 2, 0);
    mainLayout->addLayout(perfLayout, 2, 1);


    //QHBoxLayout *performersLayout = new QHBoxLayout;
    //TODO: performers

    QColor invertColor(255-taskColor.red(), 255-taskColor.green(), 255-taskColor.blue());

    setStyleSheet(QStringLiteral(".Task { background-color: %1;"
                                         "border-radius: 10px; "
                                         "border: 4px solid; }"
                                 ".Task[timeLeft=\"much\"] { border-color: green; }"
                                 ".Task[timeLeft=\"small\"] { border-color: yellow; }"
                                 ".Task[timeLeft=\"none\"] { border-color: red; }"
                                 "QLabel { color: %2; }").arg(taskColor.name()).arg(invertColor.name()));




    setLayout(mainLayout);
    updateTime();

    //show();
}



void Task::setName(QString name)
{
    this->name = name;
}

QString Task::getName()
{
    return this->name;
}



void Task::setDeadline(int y, int mo, int d, int h, int mi, int s)
{
    deadline.setDate(QDate(y,mo,d));
    deadline.setTime(QTime(h,mi,s));
}

QDateTime Task::getCreatedDateTime()
{
    return created;
}

QString Task::getCreatedString()
{
    return created.toString("dd.MM.yy HH:mm");
}

QDateTime Task::getDeadlineDateTime()
{
    return deadline;
}

QString Task::getDeadlineString()
{
    return deadline.toString("dd.MM.yy HH:mm");
}

void Task::updateTime()
{


    qint64 allSeconds = created.secsTo(deadline);
    qint64 leftSeconds = QDateTime::currentDateTime().secsTo(deadline);
    if (leftSeconds < 0) leftSeconds = 0;


    if (leftSeconds * 10 <= allSeconds) setProperty("timeLeft", "none");
    else if (leftSeconds * 2 <= allSeconds) setProperty("timeLeft", "small");
    else setProperty("timeLeft", "much");
    style()->unpolish(this);
    style()->polish(this);


    QString strTime;
    int days = leftSeconds / 3600 / 24;
    QTime time = QTime(0,0).addSecs(leftSeconds);
    if (days > 0)
        strTime.append(QString::number(days)).append(" дн. ");

    strTime.append(time.toString("HH:mm:ss"));

    timeLabel->setText(strTime);
}

void Task::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

#include <iostream>

void Task::pressBurger()
{
    std::cout << "toggled" << std::endl;
}
