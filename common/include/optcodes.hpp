#pragma once

#include <stdint.h>

namespace opt
{
    const uint16_t REQUEST_INFORMATION_FROM_SERVER = 0x0F00;
    const uint16_t NEW_INFO = 0x0F01;
    const uint16_t REQUEST_PARKING  = 0x0E00;
    const uint16_t REQUEST_TICKET = 0x0D00;
    const uint16_t POSITION_INFO = 0x0100;
    const uint16_t TICKET_RESPONSE = 0x0101;
    const uint16_t PARKING_RESPONSE = 0x0102;
    const uint16_t ERROR = 0x0200;
} // namespace opt
