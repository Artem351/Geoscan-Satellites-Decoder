#ifndef GEOSCAN_CRC_H
#define GEOSCAN_CRC_H

#include <vector>

class CRC{
public:
    static bool check(const std::vector<uint8_t>& packet);
    static uint16_t calculate(const std::vector<uint8_t>& packet, size_t length);
};

#endif