#include <QPushButton>
#include <QFileDialog>

#include "addemployeedialog.h"
#include "canban.h"

AddEmployeeDialog::AddEmployeeDialog(QString name, QString info, QString spec, QString sal, QImage image)
{
    setWindowTitle("Добавить сотрудника");
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    QLabel *nameLabel = new QLabel("Имя сотрудника");
    QLabel *infoLabel = new QLabel("Заметка");
    QLabel *specLabel = new QLabel("Специализация");
    QLabel *slryLabel = new QLabel("Оплата за час:");

    emplName = new QLineEdit(name);
    emplInfo = new QTextEdit(info);
    emplInfo->setFixedHeight(75);
    specComboBox = new QComboBox();
    specComboBox->addItems(Canban::inst()->specialities);
    specComboBox->setEditable(false);
    specComboBox->setCurrentText(spec);


    salary = new QLineEdit(sal);
    salary->setValidator(new QIntValidator(0, 999, this));
    salary->resize(30, salary->height());
    salary->setFixedWidth(30);
    QHBoxLayout *salaryLayout = new QHBoxLayout();
    salaryLayout->addWidget(new QLabel("$"));
    salaryLayout->addWidget(salary);
    salaryLayout->addStretch();


    photo = new QLabel();
    photo->setPixmap(QPixmap::fromImage(image).scaled(128, 128));
    QDir dir;
    qDebug() << dir.absolutePath();
    photo->setStyleSheet(" border: 2px solid gray;");
    QPushButton *upload = new QPushButton("Загрузить фото");
    connect(upload, SIGNAL(released()), this, SLOT(uploadPhoto()));

    QPushButton* btnOk     = new QPushButton("Сохранить");
    QPushButton* btnCancel = new QPushButton("Отмена");
    connect(btnOk, SIGNAL(clicked()), SLOT(accept()));
    connect(btnCancel, SIGNAL(clicked()), SLOT(reject()));

    QHBoxLayout *btnsLayout = new QHBoxLayout();
    btnsLayout->addWidget(btnOk);
    btnsLayout->addWidget(btnCancel);



    QGridLayout *layout = new QGridLayout();
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(emplName, 0, 1);
    layout->addWidget(infoLabel, 1, 0);
    layout->addWidget(emplInfo, 1, 1);
    layout->addWidget(specLabel, 2, 0);
    layout->addWidget(specComboBox, 2, 1);
    layout->addWidget(slryLabel, 3, 0);
    layout->addLayout(salaryLayout, 3, 1);
    layout->addWidget(photo, 0, 2, 3, 1);
    layout->addWidget(upload, 3, 2);
    layout->addLayout(btnsLayout, 4, 1);
    setLayout(layout);
}

void AddEmployeeDialog::uploadPhoto()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Выберите фотографию", "C:/", "*.png *.jpg *.bmp");

    if (fileName.size() != 0)
        photo->setPixmap(QPixmap(fileName).scaled(128, 128));

}


