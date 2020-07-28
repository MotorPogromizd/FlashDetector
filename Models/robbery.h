#ifndef ROBBERY_H
#define ROBBERY_H

#include <QObject>
#include <QTimer>
#include <QTcpSocket>


class Robbery : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString time READ time NOTIFY timeChanged)
public:
    explicit Robbery(QObject* parent = nullptr);

public slots:
    QString time() const;
    void setLaserCountCamera0(int count);
    void setLaserCountCamera1(int count);

signals:
    void timeChanged();

private:
    void configureTimers();
    void configureSocket();
    void processSocketConnectionFailed();
    void startGame();
    void finishGame();
    void sendDotsCount();
    void sendPing();
    void sendStart();
    void sendStop();

private slots:
    void readData();
    void reconnectSocket();
    void processSocketError(QAbstractSocket::SocketError socketError);
    void decreaseTime();

private:
    QTcpSocket* _socket;
    QTimer* _reconnectTimer;
    QTimer* _gameTimeTimer;
    int _secondsLeft;
    int _laserCountCamera0;
    int _laserCountCamera1;
};

#endif // ROBBERY_H
