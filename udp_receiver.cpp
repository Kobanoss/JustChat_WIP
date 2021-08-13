#include "udp_receiver.h"

UdpReceiver::UdpReceiver(uint16_t from_port, QObject* parent) : QObject(parent) {
    qDebug() << ">Creating thread:" << QThread::currentThreadId();
    port = from_port;
}

UdpReceiver::~UdpReceiver() {
}


void UdpReceiver:: test() {
    qDebug() << ">>Start test";
    // init_thread();
    qDebug() << ">>End test";
}


void UdpReceiver::init_thread() {
    udp_Socket = new QUdpSocket;
    listen_Port();
    emit to_Chat("Hello");
    connect(udp_Socket, &QUdpSocket::readyRead, [&](){
       if (udp_Socket->hasPendingDatagrams()) {
           QByteArray datagram;
           datagram.resize(udp_Socket->pendingDatagramSize());
           udp_Socket->readDatagram(datagram.data(), datagram.size());
           qDebug() << QString(datagram);
           emit to_Chat(datagram);
       }


    });
}
void UdpReceiver::listen_Port() {
    udp_Socket->bind(port,QUdpSocket::ShareAddress);
}


