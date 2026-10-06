#include "backgroundtrackingservicefactory.h"

#include <QtGlobal>

#ifdef Q_OS_ANDROID
#include <QCoreApplication>
#include <QJniObject>
#endif

namespace {

#ifdef Q_OS_ANDROID
    // Inicia e para o JourneyTrackingService (android/src/.../JourneyTrackingService.java)
    class BackgroundTrackingService final : public IBackgroundTrackingService
    {
    public:
        void start() override {
            if(!m_notificationPermissionRequested)
            {
                callService("requestNotificationPermission");
                m_notificationPermissionRequested = true;
            }
            callService("start");
        }

        void stop() override {
            callService("stop");
        }

    private:
        bool m_notificationPermissionRequested = false;

        static void callService(const char* method) {
            QJniObject::callStaticMethod<void>(
                "br/usp/esalq/caminhabilidade/qt/JourneyTrackingService",
                method,
                "(Landroid/content/Context;)V",
                QNativeInterface::QAndroidApplication::context().object()
            );
        }
    };
#else
    class BackgroundTrackingService final : public IBackgroundTrackingService
    {
    public:
        void start() override {}
        void stop() override {}
    };
#endif

}

std::unique_ptr<IBackgroundTrackingService> createBackgroundTrackingService() {
    return std::make_unique<BackgroundTrackingService>();
}
