#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    controller = new Controller();
    plot = new QCustomPlot();
    connect(this, &MainWindow::fileSelected, controller, &Controller::onFileSelected);
    connect(controller, &Controller::readyForBuildGraphics, this, &MainWindow::printGraph);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete controller;
    delete plot;
}


void MainWindow::printGraph()
{
    QMap<QString, QVector<double>> position = controller->getPosition();
    QStringList headers = controller->getHeaders();
    QVector<double> time = controller->getTime();


    for (int i = 1; i < headers.size(); ++i)
    {
        GraphBuilder* builder = new GraphBuilder();
        plot = builder->getCustomPlot();
        builder->build(position, headers[i], time);
        ui->gridLayout->addWidget(plot);
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

