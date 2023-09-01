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
#include <cmath>

#define RAD_TO_DEG(r) ((r) * 180. / M_PI)

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
    QMap<QString, QVector<double>> mPositionData;
    QStringList mHeaders;
    QVector<double> mTimeData;

    QFile mFile;
    QTextStream mStream;
    QStringList mFields;


    bool openFile(const QString& filePath);
    bool isStringComplete(const QStringList& fields) const;
    inline bool isRadianWordInHeader(const QString& header) const;
    QDateTime correctDateTime(const QString& time) const;
    QVector<double> parseNumbers(const QStringList& fields) const;
    double convertTimeInDecimalSeconds(const QString& time) const;

    void parseHeaders();
    void parsePosition();
    void parseTime();
    void radiansToDegrees();
    QVector<double> convertUnits(const QVector<double>& position) const;
};


#endif // MODEL_H
