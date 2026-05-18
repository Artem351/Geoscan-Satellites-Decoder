#include "geoscanPacketProcessor.h"
#include "geoscanPN9.h"
#include "geoscanCRC.h"
#include <vector>
#include <QDebug>

ProcessedPacket PacketProccessor::process(const std::vector<uint8_t>& packet){
    ProcessedPacket result;

    if (packet.size() < PACKET_IMAGE_SIZE) {
        qDebug() << "Packet dropped: too short, size:"
                 << static_cast<int>(packet.size());
        return result;
    }

    // Пробуем image 70 байт
    if (tryCandidate(packet, PACKET_IMAGE_SIZE, PacketType::ImagePacket, result)) {
        return result;
    }

    // Пробуем обычный пакет 74 байта
    if (tryCandidate(packet, PACKET_IMAGE_SIZE, PacketType::StandartPacket, result)) {
        return result;
    }

    qDebug() << "Packet dropped: CRC failed for 70 and 74";
    return result;
}


bool PacketProccessor::tryCandidate(const std::vector<uint8_t>& packet,
                                   int candidateSize,
                                   PacketType type,
                                   ProcessedPacket& out){
                                    
    if (packet.size() < static_cast<size_t>(candidateSize)) {
        return false;
    }

    std::vector<uint8_t> candidate(packet.begin(), packet.begin() + candidateSize);

    Scrambler::descramblePN9(candidate);

    if (!CRC::check(candidate)) {
        return false;
    }

    out.type = type;
    out.bytes = std::move(candidate);

    if (type == PacketType::ImagePacket) {
        qDebug() << "Valid IMAGE packet, size" << candidateSize;
    } else if (type == PacketType::StandartPacket) {
        qDebug() << "Valid STANDARD packet, size" << candidateSize;
    }

    return true;
}