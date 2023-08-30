#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QMap>
#include <QStringList>
#include <QString>
#include <QVector>
#include <QDateTime>
#include <QList>
#include <QTextStream>
#include <QtDebug>
#include <QMessageBox>

class Model : public QObject
{
    Q_OBJECT
public:
    Model(QObject* parent = nullptr);
    ~Model();

    QStringList getHeaders() const;
    QMap<QString, QVector<double>> getData() const;
    QVector<double> getTimes() const;
    void parseData(const QString& filePath);


private:
    bool openFile(const QString& filePath);
    bool isStringComplete(const QStringList& fields) const;
    QDateTime correctDateTime(const QString& time) const;
    QVector<double> parseNumbers(const QStringList& fields) const;

    void parseHeaders();
    void parsePosition();
    void parseTime();

    QMap<QString, QVector<double>> mPositionData;
    QStringList mHeaders;
    QVector<double> mTimeData;

    QFile mFile;
    QTextStream mStream;
    QStringList mFields;
};


#endif // MODEL_H
