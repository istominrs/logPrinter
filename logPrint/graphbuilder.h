#ifndef GRAPHBUILDER_H
#define GRAPHBUILDER_H

#include "qcustomplot.h"



class GraphBuilder : public QWidget
{
    Q_OBJECT
public:
    explicit GraphBuilder(QWidget* parent = nullptr);
    ~GraphBuilder();

    QCustomPlot* getCustomPlot();
    void build(QMap<QString, QVector<double>>& position, const QString& header, const QVector<double>& time);


private:
    QCustomPlot* _customPlot = nullptr;
    QCPItemTracer* _tracer = nullptr;
    QCPItemText* _label = nullptr;

    double minValueTime = 0;
    double maxValueTime = 0;


private slots:
    void handleMouseEnter(QMouseEvent* event);

    void onXRangeChanged(const QCPRange& range);
};

#endif // GRAPHBUILDER_H
