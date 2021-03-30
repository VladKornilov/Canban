#ifndef ADDEMPLOYEEDIALOG_H
#define ADDEMPLOYEEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QLabel>

class AddEmployeeDialog : public QDialog
{
    Q_OBJECT
public:
    AddEmployeeDialog(QString name = "John Doe",
                      QString info = "",
                      QString spec = "",
                      QString sal = "10",
                      QImage image = QImage("../images/employee_dummy.png"));

    QString getEmplName() { return emplName->text(); }
    QString getEmplInfo() { return emplInfo->toPlainText(); }
    QString getEmplSpec() { return specComboBox->currentText(); }
    int getSalary() { return salary->text().toInt(); }
    QImage getPhoto() { return photo->pixmap().toImage(); }

private:
    QLineEdit *emplName;
    QTextEdit *emplInfo;
    QComboBox *specComboBox;
    QLineEdit *salary;
    QLabel *photo;


private slots:
    void uploadPhoto();
};

#endif // ADDEMPLOYEEDIALOG_H
