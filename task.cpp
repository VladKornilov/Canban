#include <QStyleOption>
#include <QPainter>
#include <QPushButton>
#include <QMenu>
#include <QStringList>
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QCoreApplication>
#include <QMessageBox>


#include "task.h"
#include "canban.h"
#include "addtaskdialog.h"


Task::Task(QString taskName, QString taskDescr, QString theme, QColor taskColor,
           std::vector <Employee *> performers, QDateTime deadline, QDateTime created) : QWidget(nullptr)
{
    this->created = created;
    this->deadline = deadline;
    this->performers = performers;
    name = taskName;
    description = taskDescr;
    themeName = theme;
    color = taskColor;

    setMaximumSize(Canban::TASK_WIDTH, 150);
    setMinimumSize(Canban::TASK_WIDTH, 120);

    QHBoxLayout *nameLayout = new QHBoxLayout();

    nameLabel = new QLabel();
    nameLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    nameLabel->setStyleSheet("font: bold 14px;");
    nameLayout->addStretch();
    nameLayout->addWidget(nameLabel);
    nameLayout->addStretch();

    burger = new QPushButton();
    burger->setIcon(QIcon("images/burger.png"));
    burger->setStyleSheet("QAbstractButton { background: rgba(255,255,255,0); }");
    burger->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(burger, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));
    nameLayout->addWidget(burger);

    descriptionLabel = new QLabel();
    descriptionLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    descriptionLabel->setWordWrap(true);

    timeLabel = new QLabel();
    timeLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    timeLabel->setFixedHeight(32);

    perfLayout = new QHBoxLayout();
    perfLayout->addStretch();
    for (int i = 0; i < 3; i++)
        perfLabel[i] = nullptr;

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(nameLayout, 0, 0, 1, 2);
    mainLayout->addWidget(descriptionLabel, 1, 0, 1, 2);
    mainLayout->addWidget(timeLabel, 2, 0);
    mainLayout->addLayout(perfLayout, 2, 1);

    setLayout(mainLayout);
    updateVisual();
    updateTime();
}

void Task::removePerformer(Employee *performer)
{
    for (size_t i = 0; i < performers.size(); i++) {
        if (performers[i] == performer) {
            for (size_t j = i + 1; j < performers.size(); j++) {
                performers[j - 1] = performers[j];
                perfLabel[j - 1] = perfLabel[j];
            }
            perfLabel[performers.size() - 1]->clear();
            performers.pop_back();
            break;
        }
    }
    update();
}

void Task::setDeadline(int y, int mo, int d, int h, int mi, int s)
{
    deadline.setDate(QDate(y,mo,d));
    deadline.setTime(QTime(h,mi,s));
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


QString fillString(QString symbols, int count) {
    QString str;
    while (count--) str += symbols;
    return str;
}


QString Task::generateReport()
{
    int reportWidth = 70;
    QString report = fillString("=", reportWidth) + "\n";
    report += "|" + fillString(" ", reportWidth - 2) + "|\n";
    report += "| " + QString("Название задачи: ") + name + fillString(" ", reportWidth - 3 - 17 - name.length()) + "|\n";
    report += "|" + fillString(" ", reportWidth - 2) + "|\n";
    report += "| " + QString("Описание: ") + fillString(" ", reportWidth - 3 - 10) + "|\n";

    int descPos = 0;
    while (descPos < description.length()) {
        QString line = description.mid(descPos, reportWidth - 4);
        descPos += line.length();
        report += "| " + line + fillString(" ", reportWidth - 3 - line.length()) + "|\n";
    }
    report += "|" + fillString(" ", reportWidth - 2) + "|\n";

    report += "| " + QString("Исполнители: ");
    int perfLength = 13;
    for (auto perf : performers) {
        QString perfName = perf->getName();
        report += perfName + "; ";
        perfLength += perfName.length() + 2;
    }
    report += fillString(" ", reportWidth - 3 - perfLength) + "|\n";

    report += "|" + fillString(" ", reportWidth - 2) + "|\n";
    report += "| " + QString("Дата создания: ") + getCreatedString() + fillString(" ", reportWidth - 3 - 15 - getCreatedString().length()) + "|\n";

    report += "|" + fillString(" ", reportWidth - 2) + "|\n";
    report += "| " + QString("Дедлайн: ") + getDeadlineString() + fillString(" ", reportWidth - 3 - 9 - getDeadlineString().length()) + "|\n";

    report += "|" + fillString(" ", reportWidth - 2) + "|\n";
    report += fillString("=", reportWidth) + "\n";
    return report;
}


void Task::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


void Task::updateVisual()
{
    nameLabel->setText(name);
    descriptionLabel->setText(description);
    timeLabel->setToolTip(QStringLiteral(u"Создано: %1\nДедлайн: %2").arg(getCreatedString()).arg(getDeadlineString()));

    for (size_t i = 0; i < 3; i++) {
        perfLayout->removeWidget(perfLabel[i]);
        if (perfLabel[i] != nullptr) {
            delete perfLabel[i];
            perfLabel[i] = nullptr;
        }
    }

    perfLayout->addStretch();
    for (size_t i = 0; i < performers.size(); i++) {

        perfLabel[i] = new QLabel();
        perfLabel[i]->setPixmap(QPixmap::fromImage(performers[i]->getSmallPhoto()));
        perfLabel[i]->setToolTip(performers[i]->getName());
        perfLayout->addWidget(perfLabel[i]);
    }

    QColor invertColor(255-color.red(), 255-color.green(), 255-color.blue());
    setStyleSheet(QStringLiteral(".Task { background-color: %1;"
                                         "border-radius: 10px; "
                                         "border: 4px solid; }"
                                 ".Task[timeLeft=\"much\"] { border-color: green; }"
                                 ".Task[timeLeft=\"small\"] { border-color: yellow; }"
                                 ".Task[timeLeft=\"none\"] { border-color: red; }"
                                 "QLabel { color: %2; }").arg(color.name()).arg(invertColor.name()));
    update();
}


void Task::ShowContextMenu(const QPoint& pos)
{
    QMenu *menu = new QMenu("Menu", this);
    QMenu *move = new QMenu("Переместить", menu);

    QStringList themeNames = Canban::inst()->getThemeNames();
    for (int i = 0; i < themeNames.size(); i++) {
        QAction *themeAction = new QAction(themeNames[i]);
        Theme *theme = Canban::inst()->getTheme(themeNames[i]);
        if (theme->containsTask(this)) {
            themeAction->setEnabled(false);
        }
        move->addAction(themeAction);
    }
    connect(move, SIGNAL(triggered(QAction *)), this, SLOT(changeTheme(QAction *)));

    menu->addMenu(move);

    QAction *edit = new QAction("Редактировать");
    connect(edit, SIGNAL(triggered()), this, SLOT(editTask()));
    menu->addAction(edit);

    QAction *remove = new QAction("Удалить");
    connect(remove, SIGNAL(triggered()), this, SLOT(removeTask()));
    menu->addAction(remove);

    menu->popup(burger->mapToGlobal(pos));
}

void Task::changeTheme(QAction *themeName)
{
    Canban::inst()->moveTask(this, Canban::inst()->getTheme(themeName->text()));
}

void Task::editTask()
{
    AddTaskDialog *dialog = new AddTaskDialog(name, description, color, performers, deadline);
    if (dialog->exec() == QDialog::Accepted) {
        QString taskName = dialog->getTaskName();
        if (!taskName.isEmpty() && (Canban::inst()->getTask(taskName) == this || Canban::inst()->getTask(taskName) == nullptr)) {
            name = dialog->getTaskName();
            description = dialog->getTaskDescr();
            color = dialog->getTaskColor();
            performers = dialog->getPerformers();
            deadline = dialog->getDateTime();
            updateVisual();
        }
        else {
            QMessageBox errorBox;
            errorBox.setWindowTitle("Error!");
            errorBox.setIcon(QMessageBox::Critical);
            errorBox.setText("Невозможно изменить название задачи");
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

void Task::removeTask()
{
    Canban::inst()->getTheme(themeName)->removeTask(this);
    delete this;
}
