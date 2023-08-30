#include "model.h"

Model::Model(QObject* parent)
    : QObject(parent) {}

Model::~Model() = default;


bool Model::openFile(const QString &filePath)
{
    mFile.setFileName(filePath);

    if (!mFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось открыть файл!");
        return false;
    }

    if (mFile.size() == 0)
    {
        QMessageBox::critical(nullptr, "Ошибка", "Размер файла нулевой!");
        return false;
    }

    mStream.setDevice(&mFile);
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

        if (ok)
        {
            column.append(number);
        }
    }

    return column;
}


QDateTime Model::correctDateTime(const QString &time) const
{
    int lastIndex = time.lastIndexOf(".");
    QString trimmed = time.left(lastIndex + 4);
    return QDateTime::fromString(trimmed, "yyyy-MM-dd HH:mm:ss.zzz");
}


void Model::parseData(const QString &filePath)
{
    if (!openFile(filePath))
    {
        return;
    }

    while (!mStream.atEnd())
    {
        mFields.append(mStream.readLine());
    }

    parseHeaders();
    parsePosition();
    parseTime();

    mFile.close();
}


void Model::parseHeaders()
{
    mHeaders = mFields[0].split(",");
}


void Model::parsePosition()
{
    for (QStringList::iterator it = mFields.begin() + 1; it != mFields.end(); ++it)
    {
        QStringList values = it->split(",");

        if (!isStringComplete(values) || mHeaders.size() != values.size())
        {
            continue;
        }

        QVector<double> column = parseNumbers(values);
        for (size_t i = 0; i < column.size(); i++)
        {
            mPositionData[mHeaders[i + 1]].append(std::move(column[i]));
        }
    }
}


void Model::parseTime()
{
    QStringList times;
    for (QStringList::iterator it = mFields.begin() + 1; it != mFields.end(); ++it)
    {
        QStringList values = it->split(",");

        if (!isStringComplete(values) || mHeaders.size() != values.size())
        {
            continue;
        }
        times.append(std::move(values.first()));
    }

    for (const QString& time : times)
    {
        QDateTime dateTime = correctDateTime(time);
        if (dateTime.isValid())
        {
            qint64 milliseconds = dateTime.time().msecsSinceStartOfDay();
            mTimeData.append(milliseconds);
        }
    }
}


QStringList Model::getHeaders() const
{
    return this->mHeaders;
}

QMap<QString, QVector<double>> Model::getData() const
{
    return this->mPositionData;
}

QVector<double> Model::getTimes() const
{
    return this->mTimeData;
}
