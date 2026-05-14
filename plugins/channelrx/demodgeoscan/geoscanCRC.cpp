#include "geoscanCRC.h"
#include <QDebug>

uint16_t CRC::calculate(const std::vector<uint8_t>& packet, size_t length)
{
    uint16_t crc = 0xFFFF;

    for (size_t i = 0; i < length; i++) {
        crc ^= static_cast<uint16_t>(packet[i]) << 8;

        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = static_cast<uint16_t>((crc << 1) ^ 0x8005);
            } else {
                crc = static_cast<uint16_t>(crc << 1);
            }
        }
    }

    return crc;
}

bool CRC::check(const std::vector<uint8_t>& packet)
{
    const size_t packetSize = packet.size();

    if (packetSize < 2) {
        qDebug() << "CRC Failed: packet too small";
        return false;
    }

    const uint16_t receivedCrc =
        (static_cast<uint16_t>(packet[packetSize - 2]) << 8) |
         static_cast<uint16_t>(packet[packetSize - 1]);

    const uint16_t calculatedCrc = calculate(packet, packetSize - 2);

    if (calculatedCrc != receivedCrc) {
        qDebug() << "CRC Failed";
        qDebug() << "Calculated CRC:" << QString("0x%1").arg(calculatedCrc, 4, 16, QChar('0'));
        qDebug() << "Received CRC:  " << QString("0x%1").arg(receivedCrc, 4, 16, QChar('0'));
        return false;
    }

    qDebug() << "CRC OK";
    return true;
}