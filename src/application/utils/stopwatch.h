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
    qint64 m_elapsedTime;
    QElapsedTimer m_elapsedTimer;
    QTimer m_timer;
};

#endif // STOPWATCH_H
