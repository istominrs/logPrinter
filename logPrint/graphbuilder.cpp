#include "graphbuilder.h"

GraphBuilder::GraphBuilder(QWidget* parent)
    : QWidget(parent)
{
    _customPlot = new QCustomPlot(this);
    _tracer = new QCPItemTracer(_customPlot);
    _label = new QCPItemText(_customPlot);
    connect(_customPlot, &QCustomPlot::mouseMove, this, &GraphBuilder::handleMouseEnter);
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

    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeSpec(Qt::UTC);
    dateTicker->setDateTimeFormat("hh:mm:ss.zzz");



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

    _customPlot->setInteraction(QCP::iRangeZoom, true);
    _customPlot->setInteraction(QCP::iRangeDrag, true);

    _customPlot->rescaleAxes();
    _customPlot->replot();
}


void GraphBuilder::handleMouseEnter(QMouseEvent *event)
{


    _tracer->setGraph(_customPlot->graph());
    _tracer->setStyle(QCPItemTracer::tsCircle);
    _tracer->setPen(QPen(Qt::red));
    _tracer->position->setType(QCPItemPosition::ptPlotCoords);
    _tracer->setSize(5);
    _tracer->setBrush(Qt::red);
    _tracer->setInterpolating(true);

    double coordMouseX = _customPlot->xAxis->pixelToCoord(event->pos().x());

    _tracer->setGraphKey(coordMouseX);

    _label->position->setType(QCPItemPosition::ptPlotCoords);
    _label->position->setCoords(_tracer->position->key(), _tracer->position->value());
    _label->setPositionAlignment(Qt::AlignRight | Qt::AlignTop);

    QPointF tracerCoords = _tracer->position->coords();
    double xCoord = tracerCoords.x();
    double yCoord = tracerCoords.y();

    QTime time = QTime(0, 0).addMSecs(static_cast<int>(xCoord));
    QString formattedTime = time.toString("hh:mm:ss.zzz");
    _label->setText(QString("Time: %1 | Position: %2").arg(formattedTime).arg(yCoord));
//    int i = 0;
//    if (!_customPlot->rect().contains(event->pos()))
//    {
//        i++;
//        qDebug() << i;
//    }

    _tracer->updatePosition();
    _customPlot->replot();

}

