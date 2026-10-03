#ifndef WAITFOR_H
#define WAITFOR_H

#include <QEventLoop>
#include <QFuture>
#include <QFutureWatcher>

template <typename T>
T waitFor(QFuture<T> future)
{
    QFutureWatcher<T> watcher;
    QEventLoop loop;
    QObject::connect(&watcher, &QFutureWatcher<T>::finished, &loop, &QEventLoop::quit);
    watcher.setFuture(future);
    if(!future.isFinished())
        loop.exec();
    return future.resultCount() > 0 ? future.result() : T();
}

#endif // WAITFOR_H
