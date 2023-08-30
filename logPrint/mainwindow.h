#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "controller.h"
#include "plotter.h"

#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void fileSelected(const QString& filePath);

private slots:
    void on_chooseFileAction_triggered();

    void on_aboutProgramAction_triggered();

public slots:
    void printPlot();

private:
    Ui::MainWindow *ui;

    Controller* mController = nullptr;
};
#endif // MAINWINDOW_H
