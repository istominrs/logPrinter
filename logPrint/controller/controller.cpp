#include "controller.h"

Controller::Controller(QObject* parent)
    : QObject(parent)
{
    mModel = new Model();
}

Controller::~Controller()
{
    delete mModel;
}


void Controller::onFileSelected(const QString& filePath)
{
    mModel->parseData(filePath);

    mHeaders = mModel->getHeaders();
    mPosition = mModel->getData();
    mTime = mModel->getTimes();

    emit buildPlots();
}


QVector<double> Controller::getTime() const
{
    return mTime;
}


QMap<QString, QVector<double>> Controller::getPosition() const
{
    return mPosition;
}


QStringList Controller::getHeaders() const
{
    return mHeaders;
}

