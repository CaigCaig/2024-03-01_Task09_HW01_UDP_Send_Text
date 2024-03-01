#include "udpworker.h"

UDPworker::UDPworker(QObject *parent) : QObject(parent)
{



}


/*!
 * @brief Метод инициализирует UDP сервер
 */
void UDPworker::InitSocket()
{

    serviceUdpSocket = new QUdpSocket(this);
    /*
     * Соединяем присваиваем адрес и порт серверу и соединяем функцию
     * обраотчик принятых пакетов с сокетом
     */
    serviceUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT);

    connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readDateDatagrams);

    dataUdpSocket = new QUdpSocket(this);
    dataUdpSocket->bind(QHostAddress::LocalHost, DATA_PORT);
    connect(dataUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readDataDatagrams);

}

/*!
 * @brief Метод осуществляет обработку принятой датаграммы
 */
void UDPworker::ReadDatagram(QNetworkDatagram datagram)
{
    QByteArray data;
    QDataStream inStr(&data, QIODevice::ReadOnly);
    QDateTime dateTime;
    QString text;
    switch(datagram.senderPort())
    {
    case BIND_PORT:
        data = datagram.data();
        inStr >> dateTime;

        emit sig_sendTimeToGUI(dateTime);
        break;
    case DATA_PORT:
        text = "Принято сообщение от " + datagram.senderAddress().toString() + ", размер сообщения(байт) " + QString::number(datagram.data().size());
        emit sig_sendDataToGUI(text);
        break;
    }

}

/*!
 * @brief Метод осуществляет опередачу датаграммы
 */
void UDPworker::SendDatagram(QByteArray data, uint16_t port)
{
    /*
     *  Отправляем данные на localhost и задефайненный порт
     */
    switch (port)
    {
    case BIND_PORT:
        serviceUdpSocket->writeDatagram(data, QHostAddress::LocalHost, port);
        break;
    case DATA_PORT:
        dataUdpSocket->writeDatagram(data, QHostAddress::LocalHost, port);
        break;
    }
}

/*!
 * @brief Метод осуществляет чтение датаграм из сокета
 */
void UDPworker::readDateDatagrams( void )
{
    /*
     *  Производим чтение принятых датаграмм
     */
    while(serviceUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
            ReadDatagram(datagram);
    }

}

void UDPworker::readDataDatagrams( void )
{
    /*
     *  Производим чтение принятых датаграмм
     */
    while(dataUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = dataUdpSocket->receiveDatagram();
            ReadDatagram(datagram);
    }

}

