#include "model.h"

Model::Model(QObject* parent)
    : QObject(parent) {}

Model::~Model() = default;


bool Model::openFile(const QString &filePath, QFile &file, QTextStream &stream) const
{
    file.setFileName(filePath);

    if (file.size() == 0)
    {
        QMessageBox::critical(nullptr, "Ошибка", "Размер файла нулевой!");
        return false;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось открыть файл!");
        return false;
    }

    stream.setDevice(&file);
    return true;
}


bool Model::isStringComplete(const QStringList &fields) const
{
    bool isFull = true;
    for (const QString& field : fields)
    {
        if (field.isEmpty())
        {
            isFull = false;
            break;
        }
    }

    return isFull;
}


QVector<double> Model::parseNumbers(const QStringList &fields) const
{
    QVector<double> column;
    for (const QString& field : fields)
    {
        bool ok = false;
        double number = field.trimmed().toDouble(&ok);

        if (ok) { column.append(number); }
    }

    return column;
}


void Model::parseHeaders(const QString& filePath)
{
    if (!openFile(filePath, file, stream)) { return; }

    headers = stream.readLine().split(",");
    file.close();
}


void Model::parseData(const QString &filePath)
{
    if (!openFile(filePath, file, stream)) { return; }

    while (!stream.atEnd())
    {
        QStringList fields = stream.readLine().split(",");

        if (!isStringComplete(fields) || headers.size() != fields.size()) { continue; }

        QVector<double> column = parseNumbers(fields);
        for (size_t i = 0; i < column.size(); ++i)
        {
            positionData[headers[i + 1]].append(column[i]);
        }

    }
    file.close();
}


void Model::parseTime(const QString& filePath)
{
    if (!openFile(filePath, file, stream)) { return; }

    QStringList times;
    while (!stream.atEnd())
    {
        QStringList fields = stream.readLine().split(",");

        if (!isStringComplete(fields) || headers.size() != fields.size()) { continue; }

//        QStringList times;
        times.append(fields.first());

//        for (const QString& time : times)
//        {
//            QDateTime dateTime = QDateTime::fromString(time.chopped(3), "yyyy-MM-dd hh:mm:ss.zzz");

//            if (dateTime.isValid())
//            {
//                QTime t = dateTime.time();
//                timeData.append(t.msecsSinceStartOfDay());
//            }
//        }
    }

    times.removeAt(0);
    for (const QString& time : times)
    {
        int lastIndex = time.lastIndexOf(".");
        QString trimmed = time.left(lastIndex + 4);
        QDateTime dateTime = QDateTime::fromString(trimmed, "yyyy-MM-dd hh:mm:ss.zzz");

        if (dateTime.isValid())
        {
            QTime t = dateTime.time();
            timeData.append(t.msecsSinceStartOfDay());

        }
    }
    file.close();

}


QStringList Model::getHeaders() const
{
    return this->headers;
}

QMap<QString, QVector<double>> Model::getData() const
{
    return this->positionData;
}

QVector<double> Model::getTimes() const
{
    return this->timeData;
}
