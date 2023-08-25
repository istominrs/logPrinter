#include "controller.h"

Controller::Controller(QObject* parent)
    : QObject(parent)
{
    _model = new Model();
}

Controller::~Controller()
{
    delete _model;
}


void Controller::onFileSelected(const QString& filePath)
{
    _model->parseHeaders(filePath);
    _headers = _model->getHeaders();

    _model->parseData(filePath);
    _position = _model->getData();

    _model->parseTime(filePath);
    _time = _model->getTimes();

    emit readyForBuildGraphics();
}



QVector<double> Controller::getTime() const
{
    return this->_time;
}

QMap<QString, QVector<double>> Controller::getPosition() const
{
    return this->_position;
}

QStringList Controller::getHeaders() const
{
    return this->_headers;
}

