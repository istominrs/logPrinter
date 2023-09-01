#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    mController = new Controller();
    connect(this, &MainWindow::fileSelected, mController, &Controller::onFileSelected);
    connect(mController, &Controller::buildPlots, this, &MainWindow::printPlot);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mController;
}


void MainWindow::printPlot()
{
    QMap<QString, QVector<double>> position = mController->getPosition();
    QStringList headers = mController->getHeaders();
    QVector<double> time = mController->getTime();

    for (int i = 1; i < headers.size(); ++i)
    {
       std::unique_ptr<Plotter> plotter(new Plotter());
       plotter->plotGraph(position, headers[i], time);
       ui->gridLayout->addWidget(plotter.release());
    }

    ui->scrollAreaWidgetContents->setMinimumHeight(400 * headers.size());
}

void MainWindow::on_chooseFileAction_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Выберите файл", QDir::homePath(), "*.csv");

    emit fileSelected(filePath);
}


void MainWindow::on_aboutProgramAction_triggered()
{
    QMessageBox::information(this, "О программе",
    "<b>Использование: </b><br>"
    "<br>"
    "<b>1.</b> Запустите программу.<br>"
    "<br>"
    "<b>2.</b> Выберите файл с данными, нажав на кнопку 'Выбрать файл'.<br>"
    "<br>"
    "<b>3.</b> Откроется диалоговое окно выбора файла. Укажите путь к файлу и нажмите 'Открыть' .<br>"
    "<br>"
    "<b>4.</b> Программа загрузит данные из выбранного файла и автоматически строит графики на основе этих данных.<br>"
    "<br>"
    "<b>5.</b> Используйте элементы управления графиками, чтобы настроить и адаптировать графики в соответствии с вашими потребностями.<br>"
    "<br>"
    "<b>6.</b> Исследуйте данные, анализируйте тренды и корреляции, исследуйте различные аспекты ваших данных с помощью графиков.");
}

