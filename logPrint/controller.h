#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "model.h"

#include <QObject>
#include <QDebug>

class Controller : public QObject
{
    Q_OBJECT

public slots:
    void onFileSelected(const QString& filePath);

signals:
    void readyForBuildGraphics();

public:
    explicit Controller(QObject* parent = nullptr);
    ~Controller();

    QVector<double> getTime() const;
    QMap<QString, QVector<double> > getPosition() const;
    QStringList getHeaders() const;

private:
    Model* _model = nullptr;

    QVector<double> _time;
    QMap<QString, QVector<double>> _position;
    QStringList _headers;
};


#endif // CONTROLLER_H
