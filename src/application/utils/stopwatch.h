#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QElapsedTimer>
#include <QTimer>

class Stopwatch : public QObject
{
    Q_OBJECT
public:
    explicit Stopwatch(QObject *parent = nullptr);

    void start();
    void stop();

signals:
    void updated(int elapsed_ms);

private slots:
    void update();

private:
    QTimer m_timer;
    qint64 m_elapsedTime;
    qint64 m_startTime;
};

#endif // STOPWATCH_H
