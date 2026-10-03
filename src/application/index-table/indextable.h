#ifndef INDEXTABLE_H
#define INDEXTABLE_H

#include <QObject>
#include <QFuture>
#include <QVector>

#include "tableitems.h"
#include "iindexapi.h"

class IndexTable : public QObject
{
    Q_OBJECT
public:
    static IndexTable& instance();

    void setApi(IIndexAPI* api);
    QFuture<QVector<DimensionItem>> get();

private:
    explicit IndexTable(QObject *parent = nullptr);

    IIndexAPI* m_api = nullptr;
};

#endif // INDEXTABLE_H
