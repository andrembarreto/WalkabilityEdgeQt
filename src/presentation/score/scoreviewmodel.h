#ifndef SCOREVIEWMODEL_H
#define SCOREVIEWMODEL_H

#include <qqml.h>

struct scoreViewModel
{
    Q_GADGET
    QML_ELEMENT

    Q_PROPERTY(float global MEMBER global CONSTANT)

public:
    float global;
};

#endif // SCOREVIEWMODEL_H
