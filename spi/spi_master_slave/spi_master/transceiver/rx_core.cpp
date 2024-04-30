#include "rx_core.hpp"

#include <iostream>
#include <pico/bootrom.h>
#include "pico/mutex.h"

#include "rx_utils/parser.hpp"
#include "../utilities/base_types/Spi.hpp"
#include "../loop/common_data/common_variables.hpp"

#warning DO NOT SLEEP IN THIS FUNC !!!

void RX_core::comReceiveISR(uint a, uint32_t b)
{
    if (ADC_IS_READY_TO_READ)
    {
        return;
    }
    decoder.activePort(0);
    Spi::setProperties(16, 1, 0);
    spi_read16_blocking(spi_default, 0, spiBuf, 8);
    ADC_IS_READY_TO_READ = true;
}

void RX_core::launchOnCore1()
{
    while (true)
    {
        parse(vector, vupdateparams);
        if (!vector.empty())
        {
            switch (vector[0])
            {
                case VirtualCmd : //флаг симуляции работы микрокотроллера
                    flgVirtual = vector[1];
                    break;
                case DebugLevelCmd: // флаг вывода отладочной инофрмации debug level =2;  =3 запрет вывода!
                    flgDebugLevel = vector[1];
                    break;
                case DebugCmd: // флаг вывода отладочной инофрмации debug level =2;  =3 запрет вывода!
                    flgDebug = vector[1];
                    afc = "code" + std::to_string(DEBUG) + "debug Set Debug " + std::to_string(flgDebug) + '\n';
                    std::cout << afc;
                    afc.clear();
                    sleep_ms(100);
                    break;
//                case SetUseCritialSectAlgCode: // флаг использовать Сritical_section
//                    flgСritical_section = (bool) vector[1];
//                    break;
                case TheadDoneCmd: // mf
                    TheadDone = true;
                    break;
                case DRAWDONECmd: // mf
                    DrawDone = true;
                    break;
                case STOPCmd:
                    STOP = true; //stopAll(); stop the active algorithm
                    break;
                default:
                {
                    if (vector[0] >= 0 && vector[0] < 100)
                    { ALGCODE = (int16_t) vector[0]; }
                    else
                    { ALGCODE = 0; }
                    break;
                }
            }
            continue;
        }
        if (!vupdateparams.empty())
        {
            if (vupdateparams[0] == CONFIG_UPDATECmd)
            {
                CONFIG_UPDATE = true;
            }
        }
    }
}


void RX_core::parse(std::vector<int32_t> &vec, std::vector<int32_t> &vparams)
{
    std::string s;
    getline(std::cin, s);
    Parser parser(s.data());
}

