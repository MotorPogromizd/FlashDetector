#include <QApplication>
#include <QQmlApplicationEngine>

#include <QCanBus>

//#include <QNetworkRequest>
//#include <QNetworkReply>
//#include <QNetworkAccessManager>
//#include <QNetworkConfiguration>
//#include <QWebSocket>

//#include <QPluginLoader>
//#include <QtPlugin>

#include <QQmlContext>
#include <QQmlEngine>

#include <QQmlContext>
#include <QQmlFileSelector>
#include <QQuickStyle>

#include <QFontDatabase>

#include <QWindow>

#include "CameraModels/cvcameraviewer.h"

#include "Models/robbery.h"

#include "debugoutput.h"

//Q_IMPORT_PLUGIN(ElepromCanPlugin)

int main(int argc, char *argv[])
{
    QApplication::setApplicationName("PhobiaArcadeRobbery");
    QApplication::setOrganizationName("EngPlus");
    QApplication::setOrganizationDomain("EngPlus.com");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    QFontDatabase fontDatabase;

    if (fontDatabase.addApplicationFont(":/fonts/UpheavalPro.ttf") == -1)
#ifndef QT_NO_DEBUG
        qWarning() << "Failed to load UpheavalPro.ttf";
#else
        return 101;
#endif

    QStringList selectors;
#ifdef QT_EXTRA_FILE_SELECTOR
    selectors += QT_EXTRA_FILE_SELECTOR;
#else
    if (QQuickStyle::name().contains("Material")) {
        selectors += "material";
    }
#endif



    QQmlApplicationEngine engine;
#ifndef QT_NO_DEBUG
    QString cameraAddress0("/home/cvadmin/outcpp.avi");
    QString cameraAddress1("/home/cvadmin/outcpp1.avi");
#else
    QString cameraAddress("rtsp://10.0.6.100:554/live3.sdp");
#endif

    CvCameraViewer *viewer0 = new CvCameraViewer(
                0//cameraAddress0//, 720, 576
                );
    engine.rootContext()->setContextProperty("camera0Viewer", viewer0);
    CvCameraViewer *viewer1 = new CvCameraViewer(
                1//cameraAddress1//, 720, 576
                );
    engine.rootContext()->setContextProperty("camera1Viewer", viewer1);

    Robbery robbery;

    QObject::connect(viewer0->videoThread(), &VideoProcessThread::laserCountChanged,
                     &robbery, &Robbery::setLaserCountCamera0);
    QObject::connect(viewer1->videoThread(), &VideoProcessThread::laserCountChanged,
                     &robbery, &Robbery::setLaserCountCamera1);
    engine.rootContext()->setContextProperty("robbery", &robbery);


    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        return 20;
    }

#ifdef QT_NO_DEBUG
    // Fullscreen in Release mode
    QObject *root = engine.rootObjects().first();
    QWindow *w = qobject_cast<QWindow *>(root);
    if (w != nullptr) {
        w->setVisibility(QWindow::FullScreen);
    }
#endif

    return app.exec();
}
