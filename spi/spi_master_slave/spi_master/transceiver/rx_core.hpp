#ifndef PICO_EXAMPLES_RX_CORE_HPP
#define PICO_EXAMPLES_RX_CORE_HPP

#include <ctime>
#include <vector>
#include "pico/util/queue.h"

class RX_core
{
public:
    static void parse(std::vector<int32_t> &vec, std::vector<int32_t> &vparams);
    static void comReceiveISR(uint a, uint32_t b);
    static void launchOnCore1();
};


#endif
