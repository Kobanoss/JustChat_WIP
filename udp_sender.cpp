#include "udp_sender.h"

UdpSender::UdpSender(uint64_t max_size,uint64_t freq, uint16_t from_port,QString message,QByteArray file, QObject* parent) : QObject(parent) {
    qDebug() << ">Creating thread:" << QThread::currentThreadId();
    package_size = max_size;
    package_freq = freq;
    port = from_port;
    data_str = message;
    data_file = file;
}

UdpSender::~UdpSender() {

}


void UdpSender:: test() {
    qDebug() << ">>Start test";
    init_thread();
    qDebug() << ">>End test";
}


void UdpSender::init_thread() {
    udp_Socket = new QUdpSocket;
    auto datagramma = data_str.toLatin1();
    udp_Socket->writeDatagram(datagramma, QHostAddress::Broadcast, port);
    // sender_socket->connect() TODO: Connect slot to send
    qDebug() << ">Starting Broadcast from port " << port << "\n";
}
