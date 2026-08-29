#include "stopwatch.h"

Stopwatch::Stopwatch(QObject *parent)
    : QObject{parent}
    , m_elapsedTime(0)
{
    connect(&m_timer, &QTimer::timeout, this, &Stopwatch::update);
}

void Stopwatch::update()
{
    m_elapsedTime += m_elapsedTimer.elapsed();
    emit updated(m_elapsedTime);
}

void Stopwatch::start()
{
    m_elapsedTime = 0;
    m_elapsedTimer.start();
    m_timer.start(50);
}

void Stopwatch::stop()
{
    m_timer.stop();
    update();
}