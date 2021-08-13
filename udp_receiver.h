#ifndef UDP_RECEIVER_H
#define UDP_RECEIVER_H

#include <QMainWindow>
#include <qudpsocket.h>
#include <QThread>
#include <cinttypes>

class UdpReceiver : public QObject {
    Q_OBJECT

private:
    QUdpSocket *udp_Socket;
    uint16_t port;

    QByteArray data_bytes;
    QByteArray data_file;
    QString data_str;

private slots:

    void test();

    void init_thread();
    void listen_Port();

    //    TODO

signals:
    void to_Chat(QByteArray datagram);

public:
    UdpReceiver(uint16_t from_port, QObject* parent = 0);
    ~UdpReceiver();

};

#endif // UDP_RECEIVER_H
