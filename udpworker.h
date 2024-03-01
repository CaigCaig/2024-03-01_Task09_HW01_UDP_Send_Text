#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>


#define BIND_PORT 12345
#define DATA_PORT 12346

class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSocket( void );
    void ReadDatagram( QNetworkDatagram datagram);
    void SendDatagram(QByteArray data, uint16_t port);


private slots:
    void readDateDatagrams(void);
    void readDataDatagrams(void);

private:
    QUdpSocket* serviceUdpSocket;
    QUdpSocket* dataUdpSocket;

signals:
    void sig_sendTimeToGUI(QDateTime data);
    void sig_sendDataToGUI(QString data);

};

#endif // UDPWORKER_H
