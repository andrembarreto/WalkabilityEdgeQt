#include "stopwatch.h"

#include <chrono>

namespace {

using namespace std::chrono;

const steady_clock::time_point startTime = steady_clock::now();

uint64_t timeNow()
{
    return duration_cast<milliseconds>(steady_clock::now() - startTime).count();
}

}

Stopwatch::Stopwatch(QObject *parent)
    : QObject{parent}
    , m_elapsedTime(0)
    , m_startTime(0)
{
    m_timer.setTimerType(Qt::PreciseTimer);
    m_timer.setSingleShot(false);
    m_timer.setInterval(500);
    m_timer.callOnTimeout(this, &Stopwatch::update);
}

void Stopwatch::update()
{
    m_elapsedTime = timeNow() - m_startTime;
    emit updated(m_elapsedTime);
}

void Stopwatch::start()
{
    m_elapsedTime = 0;
    m_startTime = timeNow();
    m_timer.start();
}

void Stopwatch::stop()
{
    m_timer.stop();
    update();
}