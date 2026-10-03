#ifndef IBACKGROUNDTRACKINGSERVICE_H
#define IBACKGROUNDTRACKINGSERVICE_H

class IBackgroundTrackingService
{
public:
    virtual ~IBackgroundTrackingService() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
};

#endif // IBACKGROUNDTRACKINGSERVICE_H
