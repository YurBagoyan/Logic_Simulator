#ifndef FIRSTDIALOGWINDOW_H
#define FIRSTDIALOGWINDOW_H

#include <QDialog>

namespace Ui {
class FirstDialogWindow;
}

class FirstDialogWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FirstDialogWindow(QWidget *parent = nullptr);
    ~FirstDialogWindow();

    bool isRejected() const;
    QString getProjectName() const;
    QString getProjectPath() const;


public slots:
    void on_newProjectButton_clicked();

    void on_openProjectButton_clicked();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();



private:
    void closeEvent(QCloseEvent *event);

private:
    Ui::FirstDialogWindow *ui;

    bool closed{false};

    QString projectName{"Unnamed"};
    QString projectPath{""};
};

#endif // FIRSTDIALOGWINDOW_H
