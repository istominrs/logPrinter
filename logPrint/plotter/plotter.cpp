#include "plotter.h"

Plotter::Plotter(QWidget* parent)
    : QCustomPlot(parent)
{
    mTracer = new QCPItemTracer(this);
    mLabel = new QCPItemText(this);

    this->setMouseTracking(true);

    connect(this, &QCustomPlot::mouseMove, this, &Plotter::onMouseMoveEvent);
    connect(xAxis, static_cast<void(QCPAxis::*)(const QCPRange&)>(&QCPAxis::rangeChanged), this, &Plotter::onXRangeChanged);
}

Plotter::~Plotter()
{
    QCPTextElement* headline = qobject_cast<QCPTextElement*>(plotLayout()->element(0, 0));
    delete headline;
    delete mTracer;
    delete mLabel;
}


void Plotter::plotGraph(QMap<QString, QVector<double>> &position, const QString &header, const QVector<double> &time)
{
    addGraph();
    mMinValueX = *std::min_element(time.begin(), time.end());
    mMaxValueX = *std::max_element(time.begin(), time.end());


    QSharedPointer<QCPAxisTickerTime> dateTicker(new QCPAxisTickerTime);
    dateTicker->setTimeFormat("%h:%m:%s:%z");


    graph()->setData(time, position[header]);
    graph()->setPen(QPen(QColor("green")));


    xAxis->setTicker(dateTicker);
    xAxis->setLabel("Time");
    yAxis->setLabel("Position");
    xAxis->setRange(mMinValueX, mMaxValueX);


    addTitle(header);

    setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);


    rescaleAxes();
    replot();
}


void Plotter::addTitle(const QString &title)
{
    QCPTextElement* headline = new QCPTextElement(this);
    headline->setText(title);
    headline->setFont(QFont("Arial", 14, QFont::Bold));
    plotLayout()->insertRow(0);
    plotLayout()->addElement(0, 0, headline);
}


void Plotter::onMouseMoveEvent(QMouseEvent* event)
{
    QPointF mousePos = event->pos();
    QCPAbstractPlottable* plottable = plottableAt(mousePos);
    if (plottable != nullptr)
    {
        double coordX = xAxis->pixelToCoord(event->pos().x());
        double coordY = yAxis->pixelToCoord(event->pos().y());

        customizeTracer(coordX, coordY);
        QPointF tracerCoords = mTracer->position->coords();
        double xTracerCoord = tracerCoords.x();
        double yTracerCoord = tracerCoords.y();

        QDateTime dateTime = QDateTime::currentDateTime();
        dateTime.setTime(QTime(0, 0).addMSecs(qRound(xTracerCoord * 1000)));

        QString formattedTime = dateTime.toString("hh:mm:ss.zzz");
        customizeLabel(xTracerCoord, yTracerCoord, formattedTime);

        mTracer->setVisible(true);
        mLabel->setVisible(true);
    }
    else
    {
        mTracer->setVisible(false);
        mLabel->setVisible(false);
    }


    replot();
}


void Plotter::customizeTracer(const double coordX, const double coordY)
{
    mTracer->setGraph(graph());
    mTracer->setStyle(QCPItemTracer::tsCircle);
    mTracer->position->setType(QCPItemPosition::ptPlotCoords);
    mTracer->position->setCoords(coordX, coordY);
    mTracer->setGraphKey(coordX);
    mTracer->setPen(QPen(QColor("red")));
    mTracer->setSize(5);
    mTracer->setBrush(QColor("red"));
    mTracer->setInterpolating(true);
    mTracer->updatePosition();
}


void Plotter::customizeLabel(const double coordX, const double coordY, const QString& formattedTime)
{
    mLabel->setPositionAlignment(Qt::AlignLeading | Qt::AlignBottom);
    mLabel->position->setCoords(coordX, coordY);
    mLabel->setText(QString("%1 | %2").arg(formattedTime).arg(coordY));
}


void Plotter::onXRangeChanged(const QCPRange& range)
{
    QCPRange boundedRange = range;
    double lowerRangeBound = mMinValueX;
    double upperRangeBound = mMaxValueX;

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

    xAxis->setRange(boundedRange);
}
