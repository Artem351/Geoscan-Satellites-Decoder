#include "geoscanparser.h"
#include <stdlib.h>


static bool parseType1(const ProcessedPacket& packet, GeoscanacketType1& out){
    if (packet.bytes.size() != STANDART_PACKET_SIZE) {
            return false;
    }
    std::size_t pos = 0;

        std::copy(packet.bytes.begin() + pos,
                  packet.bytes.begin() + pos + AX25_SIZE,
                  out.ax25.begin());
        pos += AX25_SIZE;

        out.id = packet.bytes[pos];
        pos += 1;

        std::copy(packet.bytes.begin() + pos,
                  packet.bytes.begin() + pos + EPS_SIZE,
                  out.eps.begin());
        pos += EPS_SIZE;

        std::copy(packet.bytes.begin() + pos,
                  packet.bytes.begin() + pos + OBC_SIZE,
                  out.obc.begin());
        pos += OBC_SIZE;

        std::copy(packet.bytes.begin() + pos,
                  packet.bytes.begin() + pos + COMMU_SIZE,
                  out.commu.begin());
        pos += COMMU_SIZE;

        pos += 2;//это для crc

        return pos == STANDART_PACKET_SIZE;
}

static bool parseImage(const ProcessedPacket& packet, GeoscanPacketImage& out) {
    if (packet.bytes.size() != IMAGE_PACKET_SIZE) {
        return false;
    }
    
    std::size_t pos = 0;

    std::copy(packet.bytes.begin() + pos,
              packet.bytes.begin() + pos + IMAGE_HEADER_SIZE,
              out.header.begin());
    pos += IMAGE_HEADER_SIZE;

    uint32_t imageSync =
        (static_cast<uint32_t>(packet.bytes[pos]) << 24) |
        (static_cast<uint32_t>(packet.bytes[pos + 1]) << 16) |
        (static_cast<uint32_t>(packet.bytes[pos + 2]) << 8) |
         static_cast<uint32_t>(packet.bytes[pos + 3]);
    pos += 4;

    if (imageSync != IMAGE_SYNC_WORD) {
        return false;
    }

    out.offset =
        (static_cast<uint32_t>(packet.bytes[pos]) << 24) |
        (static_cast<uint32_t>(packet.bytes[pos + 1]) << 16) |
        (static_cast<uint32_t>(packet.bytes[pos + 2]) << 8) |
         static_cast<uint32_t>(packet.bytes[pos + 3]);
    pos += 4;

    out.fileNumber =
        (static_cast<uint16_t>(packet.bytes[pos]) << 8) |
         static_cast<uint16_t>(packet.bytes[pos + 1]);
    pos += 2;

    std::copy(packet.bytes.begin() + pos,
              packet.bytes.begin() + pos + IMAGE_DATA_SIZE,
              out.data.begin());
    pos += IMAGE_DATA_SIZE;

    pos += 2;//это для crc

    return pos == IMAGE_PACKET_SIZE;
}
