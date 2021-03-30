#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QColorDialog>
#include <algorithm>

#include "addthemedialog.h"

AddThemeDialog::AddThemeDialog()
{
    setWindowTitle("Добавить колонку");
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    QLabel *nameLabel = new QLabel("Название колонки");

    themeName = new QLineEdit();

    QLabel *colorLabel = new QLabel("Цвет колонки");
    themeColor.setNamedColor("green");
    colorButton = new QPushButton();
    colorButton->setStyleSheet(QStringLiteral("background-color: %1; border-radius: 5px;").arg(themeColor.name()));
    connect(colorButton, SIGNAL(released()), this, SLOT(setColor()));


    QPushButton* btnOk     = new QPushButton("Создать");
    QPushButton* btnCancel = new QPushButton("Отмена");
    connect(btnOk, SIGNAL(clicked()), SLOT(accept()));
    connect(btnCancel, SIGNAL(clicked()), SLOT(reject()));

    QHBoxLayout *btnsLayout = new QHBoxLayout();
    btnsLayout->addWidget(btnOk);
    btnsLayout->addWidget(btnCancel);

    layout = new QGridLayout();
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(themeName, 0, 1);
    layout->addWidget(colorLabel, 2, 0);
    layout->addWidget(colorButton, 2, 1);
    layout->addLayout(btnsLayout, 3, 0, 1, 2);

    setLayout(layout);
}

void AddThemeDialog::setColor()
{
    themeColor = QColorDialog::getColor(themeColor, nullptr, "Выберите цвет");
    colorButton->setStyleSheet(QStringLiteral("background-color: %1; border-radius: 5px;").arg(themeColor.name()));
}
