#ifndef PLOTTER_H
#define PLOTTER_H

#include "qcustomplot.h"



class Plotter : public QCustomPlot
{
    Q_OBJECT
public:
    explicit Plotter(QWidget* parent = nullptr);
    ~Plotter();

    void plotGraph(QMap<QString, QVector<double>>& position, const QString& header, const QVector<double>& time);

private slots:
    void onMouseMoveEvent(QMouseEvent* event);
    void onXRangeChanged(const QCPRange& range);

private:
    QCPItemTracer* mTracer = nullptr;
    QCPItemText* mLabel = nullptr;
    double mMinValueX = 0;
    double mMaxValueX = 0;

    void addTitle(const QString& title);

    void customizeTracer(const double coordX, const double coordY);
    void customizeLabel(const double coordX, const double coordY, const QString& formattedTime);
};

#endif // PLOTTER_H
