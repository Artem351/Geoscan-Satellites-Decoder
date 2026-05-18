#ifndef PACKET_PROCESSOR_H
#define PACKET_PROCESSOR_H

#include <vector>
#include <cstdint>
#include "geoscanCorrelator.h"

enum PacketType {StandartPacket, ImagePacket, UnknownPacket};

struct ProcessedPacket {
    PacketType type = PacketType::UnknownPacket;
    std::vector<uint8_t> bytes;
};


class PacketProccessor{
public:
    /// @brief Метод обрабатывающий пакет. Определяется тип пакета, затем он дескремблируется и проверяется CRC 
    /// @param packet необработанный пакет
    /// @return Структура данных хранящая сам пакет и его тип
    static ProcessedPacket process(const std::vector<uint8_t> &packet);

private:
    static bool tryCandidate(const std::vector<uint8_t>& scrambledPacket,
                             int candidateSize,
                             PacketType type,
                             ProcessedPacket& out);
};;

#endif