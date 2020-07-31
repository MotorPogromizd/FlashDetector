#include <QApplication>
#include <QQmlApplicationEngine>



#include <QQmlContext>
#include <QQmlFileSelector>
#include <QQuickStyle>

#include <QWindow>

#include <opencv2/opencv.hpp>

#include "OpenCvQtViewer/cvcameraviewer.h"

#include "Models/robbery.h"

#include "QtFonts/fontconfig.h"
#include "QtDebugPrint/debugoutput.h"

const int threshold = 190;//208;//190;//174;
const int blur = 3;//5;//3;
const double scale = 2.0;


cv::Scalar randomColor(cv::RNG& rng) {
    return cv::Scalar(rng.uniform(0.0,255.0), rng.uniform(0.0,255.0), rng.uniform(0.0,255.0));
}

int main(int argc, char *argv[])
{
    QApplication::setApplicationName("FlashDetector");
    QApplication::setOrganizationName("MotorSoft");
    QApplication::setOrganizationDomain("MotorSoft");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    setDebugMessagePattern();


    QStringList selectors;
#ifdef QT_EXTRA_FILE_SELECTOR
    selectors += QT_EXTRA_FILE_SELECTOR;
#else
    if (QQuickStyle::name().contains("Material")) {
        selectors += "material";
    }
#endif



    QQmlApplicationEngine engine;

    Font::init(engine);

#ifndef QT_NO_DEBUG
    QString cameraAddress0("/home/cvadmin/outcpp.avi");
    QString cameraAddress1("/home/cvadmin/outcpp1.avi");
#else
    QString cameraAddress("rtsp://10.0.6.100:554/live3.sdp");
#endif
    cv::Mat blured, binarizedFrame, processed;
    auto task = [&blured, &binarizedFrame, &processed](cv::Mat input, cv::Mat& output) {
        cv::medianBlur(input, blured, blur);
        cv::cvtColor(blured, binarizedFrame, cv::COLOR_RGB2GRAY );
        cv::threshold(binarizedFrame, binarizedFrame, threshold, 255.0, cv::THRESH_BINARY);
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(binarizedFrame, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
        processed = input.clone();
        int contoursSize = int(contours.size());
        cv::putText(processed, std::to_string(contoursSize),
                    cv::Point(0,100), cv::FONT_HERSHEY_SIMPLEX, 1.4, cv::Scalar(0,0,255), 3);
        cv::RNG rng(2);
        for (int i = 0; i < contoursSize; ++i) {
            cv::drawContours(processed, contours, i, cv::Scalar(0,0,255),4);
        } // emit laserCountChanged(contoursSize);
        cv::cvtColor(processed, output, cv::COLOR_RGB2RGBA);
    };

    cv::Mat blured2, binarizedFrame2, processed2;
    auto task2 = [&blured2, &binarizedFrame2, &processed2](cv::Mat input, cv::Mat& output) {
        cv::medianBlur(input, blured2, blur);
        cv::cvtColor(blured2, binarizedFrame2, cv::COLOR_RGB2GRAY );
        cv::threshold(binarizedFrame2, binarizedFrame2, threshold, 255.0, cv::THRESH_BINARY);
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(binarizedFrame2, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
        processed2 = input.clone();
        int contoursSize = int(contours.size());
        cv::putText(processed2, std::to_string(contoursSize),
                    cv::Point(0,100), cv::FONT_HERSHEY_SIMPLEX, 1.4, cv::Scalar(0,0,255), 3);
        cv::RNG rng(2);
        for (int i = 0; i < contoursSize; ++i) {
            cv::drawContours(processed2, contours, i, cv::Scalar(0,0,255),4);
        } // emit laserCountChanged(contoursSize);
        cv::cvtColor(processed2, output, cv::COLOR_RGB2RGBA);
    };
    CvCameraViewer* viewer0 = new CvCameraViewer(
//                0
                cameraAddress0,
                task
                );
    engine.rootContext()->setContextProperty("camera0Viewer", viewer0);
    CvCameraViewer* viewer1 = new CvCameraViewer(
//                1
                cameraAddress1,
                task2
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
