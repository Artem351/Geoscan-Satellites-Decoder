#include "geoscanPacketProcessor.h"
#include "geoscanPN9.h"
#include "geoscanCRC.h"
#include <vector>
#include <QDebug>

int PacketProccessor::process(const std::vector<uint8_t>& packet){
    ProcessedPacket result;

    if (packet.size() < PACKET_IMAGE_SIZE) {
        qDebug() << "Packet dropped: too short, size:"
                 << static_cast<int>(scrambledPacket.size());
        return result;
    }

    // Пробуем image 70 байт
    if (tryCandidate(scrambledPacket, PACKET_IMAGE_SIZE, PacketType::ImagePacket, result)) {
        return result;
    }

    // Пробуем обычный пакет 74 байта
    if (tryCandidate(scrambledPacket, PACKET_IMAGE_SIZE, PacketType::StandartPacket, result)) {
        return result;
    }

    qDebug() << "Packet dropped: CRC failed for 70 and 74";
    return result;
}


bool PacketProcessor::tryCandidate(const std::vector<uint8_t>& scrambledPacket,
                                   int candidateSize,
                                   PacketType type,
                                   ProcessedPacket& out){
                                    
    if (scrambledPacket.size() < static_cast<size_t>(candidateSize)) {
        return false;
    }

    std::vector<uint8_t> candidate(
        scrambledPacket.begin(),
        scrambledPacket.begin() + candidateSize
    );

    Scrambler::descramblePN9(candidate);

    if (!CRC::check(candidate)) {
        return false;
    }

    out.type = type;
    out.bytes = std::move(candidate);

    if (type == PacketType::ImagePacket) {
        qDebug() << "Valid IMAGE packet, size" << candidateSize;
    } else if (type == PacketType::StandardPacket) {
        qDebug() << "Valid STANDARD packet, size" << candidateSize;
    }

    return true;
}