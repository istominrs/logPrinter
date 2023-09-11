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
    void buildPlots();

public:
    explicit Controller(QObject* parent = nullptr);
    ~Controller();

    QVector<double> getTime() const;
    QMap<QString, QVector<double> > getPosition() const;
    QStringList getHeaders() const;

private:
    Model* mModel = nullptr;
    QVector<double> mTime;
    QMap<QString, QVector<double>> mPosition;
    QStringList mHeaders;
};


#endif // CONTROLLER_H
