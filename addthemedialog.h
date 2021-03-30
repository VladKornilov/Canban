#ifndef ADDTHEMEDIALOG_H
#define ADDTHEMEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QGridLayout>

class AddThemeDialog : public QDialog
{
    Q_OBJECT
public:
    AddThemeDialog();

    QString getThemeName() { return themeName->text(); }
    QColor getThemeColor() { return themeColor; }

private:
    QLineEdit *themeName;
    QPushButton *colorButton;
    QColor themeColor;
    QGridLayout *layout;

private slots:
    void setColor();
};

#endif // ADDTHEMEDIALOG_H
