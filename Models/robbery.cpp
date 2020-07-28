#include "robbery.h"
#include "debugoutput.h"

Robbery::Robbery(QObject *parent)
    : QObject(parent)
    , _socket{ new QTcpSocket(this) }
    , _reconnectTimer{ new QTimer(this) }
    , _gameTimeTimer{ new QTimer(this) }
    , _secondsLeft{0}
    , _laserCountCamera0{0}
    , _laserCountCamera1{0}
{
    configureTimers();
    configureSocket();
    reconnectSocket();
}

QString Robbery::time() const {
    QString result;
    if (_secondsLeft > 0) {
        result += QString::number(_secondsLeft/60);
        result += ":";
        if (_secondsLeft%60 < 10) {
            result += "0";
        }
        result += QString::number(_secondsLeft%60);
    }
    else {
        result += "GAME OVER";
    }
    return result;
}

void Robbery::setLaserCountCamera0(int count) {
    _laserCountCamera0 = count;
}

void Robbery::setLaserCountCamera1(int count) {
    _laserCountCamera1 = count;
}

void Robbery::configureTimers() {
    _reconnectTimer->setSingleShot(true);
    _reconnectTimer->setInterval(5000);
    connect(_reconnectTimer, &QTimer::timeout,
            this, &Robbery::reconnectSocket);


    _gameTimeTimer->setSingleShot(false);
    _gameTimeTimer->setInterval(1000);
    connect(_gameTimeTimer, &QTimer::timeout,
            this, &Robbery::decreaseTime);
}

void Robbery::configureSocket() {
    connect(_socket, &QTcpSocket::readyRead, this, &Robbery::readData);
    connect(_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &Robbery::processSocketError);
    connect(_socket, &QTcpSocket::connected,
            [](){dbg << "connected";});
}

void Robbery::processSocketConnectionFailed() {
    dbg << "error occured";
    _reconnectTimer->start();
}

void Robbery::startGame() {
    _secondsLeft = 181;
    decreaseTime();
    _gameTimeTimer->start();
    sendStart();
}

void Robbery::finishGame() {
    _secondsLeft = 0;
    decreaseTime();
    sendStop();
}

void Robbery::sendDotsCount() {
    if (!_socket->isOpen()) {
        return;
    }
    QByteArray response;
    response.append(0x11);
    response.append('\0');
    response.append('\0');
    response.append(char(_laserCountCamera0 + _laserCountCamera1));
    _socket->write(response);
}

void Robbery::sendPing() {
    if (!_socket->isOpen()) {
        return;
    }
    QByteArray response;
    response.append(0x30);
    _socket->write(response);
}

void Robbery::sendStart()
{
    if (!_socket->isOpen()) {
        return;
    }
    QByteArray response;
    response.append(0x21);
    response.append('\0');
    response.append('\0');
    response.append(2);
    _socket->write(response);
}

void Robbery::sendStop()
{
    if (!_socket->isOpen()) {
        return;
    }
    QByteArray response;
    response.append(0x21);
    response.append('\0');
    response.append('\0');
    response.append(1);
    _socket->write(response);
}

void Robbery::readData() {
    QByteArray data = _socket->readAll();
    dbg << "received data" << data;
    if (data.length() == 1) {
        if (data.at(0) == 0x30) {
            sendPing();
        }
        return;
    }
    if (data.length() == 2) {
        if (data.at(0) == 0x11) {
            sendDotsCount();
        }
        return;
    }
    if (data.length() == 4) {
        if (data.at(0) == 0x21) {
            if (data.at(3) == 2) {
                startGame();
            }
            else if (data.at(3) == 1) {
                finishGame();
            }
        }
        return;
    }
}

void Robbery::reconnectSocket() {
    dbg << "connecting...";
    _socket->connectToHost("192.168.66.80", 8686);
}

void Robbery::processSocketError(QAbstractSocket::SocketError socketError) {
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
    case QAbstractSocket::HostNotFoundError:
    case QAbstractSocket::ConnectionRefusedError:
        processSocketConnectionFailed();
        break;
    default:
        break;
    }
    dbg << "error occured" << socketError;
}

void Robbery::decreaseTime() {
    _secondsLeft = _secondsLeft > 0 ? _secondsLeft - 1 : 0;
    if (_secondsLeft == 0) {
        _gameTimeTimer->stop();
    }
    emit timeChanged();
}
