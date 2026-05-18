#ifndef GEOSCAN_PARSER_H
#define GEOSCAN_PARSER_H

#include <cstdint>
#include "geoscanPacketProcessor.h"

constexpr std::size_t IMAGE_PACKET_SIZE = 70;
constexpr std::size_t STANDART_PACKET_SIZE = 74;

constexpr std::size_t AX25_SIZE = 16;
constexpr std::size_t EPS_SIZE = 23;
constexpr std::size_t OBC_SIZE = 16;
constexpr std::size_t COMMU_SIZE = 16;

constexpr std::size_t IMAGE_HEADER_SIZE = 5;
constexpr std::size_t IMAGE_DATA_SIZE = 53;
constexpr uint32_t    IMAGE_SYNC_WORD = 0x316f6b6f;

struct GeoscanacketType1 {
    std::array<uint8_t, AX25_SIZE> ax25;
    uint8_t id = 0;
    std::array<uint8_t, EPS_SIZE> eps;
    std::array<uint8_t, OBC_SIZE> obc;
    std::array<uint8_t, COMMU_SIZE> commu;
    uint16_t crc = 0;
};

struct GeoscanPacketImage {
    std::array<uint8_t, IMAGE_HEADER_SIZE> header;
    uint32_t offset = 0;
    uint16_t fileNumber = 0;
    std::array<uint8_t, IMAGE_DATA_SIZE> data;
    uint16_t crc = 0;
};

class GeoscanParser {
public:
    static bool parseType1(const ProcessedPacket& packet, GeoscanacketType1& out);
    static bool parseImage(const ProcessedPacket& packet, GeoscanPacketImage& out);

private:
    static uint16_t readU16BE(const std::vector<uint8_t>& bytes, size_t offset);
    static uint32_t readU32BE(const std::vector<uint8_t>& bytes, size_t offset);
};

#endif