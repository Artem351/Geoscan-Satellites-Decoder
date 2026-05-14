#ifndef PACKET_PROCESSOR_H
#define PACKET_PROCESSOR_H

#include <vector>
#include <cstdint>

enum PacketType {StandartPacket, ImagePacket, UnknownPacket};

struct ProcessedPacket {
    PacketType type = PacketType::Unknown;
    std::vector<uint8_t> bytes;
};

constexpr int PACKET_SIZE = 74;
constexpr int PACKET_IMAGE_SIZE = 70;

class PacketProccessor{
public:
    static ProcessedPacket process(const std::vector<uint8_t> &packet);

private:
    static bool tryCandidate(const std::vector<uint8_t>& scrambledPacket,
                             int candidateSize,
                             PacketType type,
                             ProcessedPacket& out);
};;

#endif