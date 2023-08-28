#include "graphbuilder.h"

GraphBuilder::GraphBuilder(QWidget* parent)
    : QWidget(parent)
{
    _customPlot = new QCustomPlot(this);
    _tracer = new QCPItemTracer(_customPlot);
    _label = new QCPItemText(_customPlot);

    connect(_customPlot, &QCustomPlot::mouseMove, this, &GraphBuilder::handleMouseEnter);


    connect(_customPlot->xAxis, static_cast<void(QCPAxis::*)(const QCPRange&)>(&QCPAxis::rangeChanged), this, &GraphBuilder::onXRangeChanged);
}

GraphBuilder::~GraphBuilder()
{
    delete _customPlot;
    delete _tracer;
    delete _label;
}


QCustomPlot* GraphBuilder::getCustomPlot()
{
    return this->_customPlot;
}


void GraphBuilder::build(QMap<QString, QVector<double>>& position, const QString& header, const QVector<double>& time)
{
    minValueTime = *std::min_element(time.begin(), time.end());
    maxValueTime = *std::max_element(time.begin(), time.end());

    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeSpec(Qt::UTC);
    dateTicker->setDateTimeFormat("HH:mm:ss.zzz");


    _customPlot->addGraph();
    _customPlot->graph()->setData(time, position[header]);
    _customPlot->graph()->setPen(QPen(QColor("green")));


    _customPlot->xAxis->setTicker(dateTicker);
    _customPlot->xAxis->setLabel("Time");
    _customPlot->yAxis->setLabel("Position");


    QCPTextElement *title = new QCPTextElement(_customPlot);
    title->setText(header);
    title->setFont(QFont("sans", 8, QFont::Bold));
    _customPlot->plotLayout()->insertRow(0);
    _customPlot->plotLayout()->addElement(0, 0, title);


    _customPlot->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);


    _customPlot->rescaleAxes();
    _customPlot->replot();
}


void GraphBuilder::handleMouseEnter(QMouseEvent *event)
{

//    double coordMouseX = _customPlot->xAxis->pixelToCoord(event->pos().x());

    QPoint mousePos = event->pos();
    if (_customPlot->rect().contains(mousePos))
    {
        // Создание и настройка трейсера
        _tracer->setGraph(_customPlot->graph());
        _tracer->setStyle(QCPItemTracer::tsCircle);
        _tracer->setPen(QPen(Qt::red));
        _tracer->position->setType(QCPItemPosition::ptPlotCoords);
        _tracer->setSize(5);
        _tracer->setBrush(Qt::red);
        _tracer->setInterpolating(true);
        _tracer->setVisible(true);

        double coordMouseX = _customPlot->xAxis->pixelToCoord(event->pos().x());
        double coordMouseY = _customPlot->yAxis->pixelToCoord(event->pos().y());

        QPointF tracerCoords = _tracer->position->coords();
        double xCoord = tracerCoords.x();
        double yCoord = tracerCoords.y();


        QTime time = QTime(0, 0).addMSecs(static_cast<int>(xCoord));
        QString formattedTime = time.toString("HH:mm:ss.zzz");
        _label->setText(QString("Time: %1 | Position: %2").arg(formattedTime).arg(yCoord));

        _tracer->position->setCoords(coordMouseX, coordMouseY);
        _tracer->setGraphKey(coordMouseX);
        _label->position->setType(QCPItemPosition::ptPlotCoords);
        _label->position->setCoords(_tracer->position->key(), _tracer->position->value());
        _label->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
        _tracer->updatePosition();
        _customPlot->replot();
    }
    else
    {
        _customPlot->removeItem(_tracer);
        delete _tracer;
        _tracer = nullptr;
        _customPlot->removeItem(_label);
        _customPlot->replot();
    }

}



void GraphBuilder::onXRangeChanged(const QCPRange& range)
{
    QCPRange boundedRange = range;
    double lowerRangeBound = minValueTime;
    double upperRangeBound = maxValueTime;

    if (boundedRange.lower < lowerRangeBound)
    {
        boundedRange.lower = lowerRangeBound;
        boundedRange.upper = lowerRangeBound + boundedRange.size();
    }

    if (boundedRange.upper > upperRangeBound)
    {
        boundedRange.upper = upperRangeBound;
        boundedRange.lower = upperRangeBound - boundedRange.size();
    }

    _customPlot->xAxis->setRange(boundedRange);
}

