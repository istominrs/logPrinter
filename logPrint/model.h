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
    void parseHeaders(const QString& filePath);
    void parseData(const QString& filePath);
    void parseTime(const QString& filePath);

private:
    bool openFile(const QString& filePath, QFile& file, QTextStream& stream) const;
    bool isStringComplete(const QStringList& fields) const;
    QDateTime correctDateTime(const QString& time) const;
    QVector<double> parseNumbers(const QStringList& fields) const;

    QMap<QString, QVector<double>> positionData;
    QStringList headers;
    QVector<double> timeData;

    QFile file;
    QTextStream stream;
};


#endif // MODEL_H
