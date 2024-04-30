#include "main_core.hpp"
#include <bitset>
#include <iostream>
#include "../utilities/led_controller.hpp"
#include "../utilities/hardcoded_functions.hpp"
#include "common_data/common_variables.hpp"
#include "../utilities/debug_logger.hpp"
#include <cmath>

void MainCore::loop()
{
    dark();
    uint64_t time = 0;
    while (time++ < UINT64_MAX - 1000)
    {
        switch (ALGCODE)
        {
            case ALGNONE:
            {
                break;
            }
            case RESONANCE:
            {
                ALGCODE = ALGNONE;
                scanner.start_frqscan();
                break;
            }
            case APPROACH:
            {
                ALGCODE = ALGNONE;
                blue();
                scanner.approacphm(vector);
                dark();
                break;
            }
            case TESTMOVER:
            {
                ALGCODE = ALGNONE;
                blue();
                scanner.testpiezomover(vector);
                dark();
                break;
            }
            case FREQ_SET:
            {
                ALGCODE = ALGNONE;
                set_Freq((uint16_t) vector[1]);
                break;
            }
            case LID_MOVE_UNTIL_STOP:
            {
                ALGCODE = ALGNONE;
                scanner.positioningXYZ(vector);
                break;
            }
            case MOVE_TOX0Y0:
            {
                ALGCODE = ALGNONE;
                scanner.move_toX0Y0(vector[1], vector[2], vector[3], vector[4]);
                break;
            }
            case LID_MOVE_TOZ0:
            {
                ALGCODE = ALGNONE;
                scanner.LID_move_toZ0(vector[1], vector[2], vector[3], vector[4], vector[5]);
                break;
            }
            case SCANNING:
            {
                ALGCODE = ALGNONE;
                DrawDone = true;
                scanner.scan_update
                    ({
                         static_cast<uint16_t>(vector[1]), static_cast<uint16_t>(vector[2]),
                         static_cast<uint8_t>(vector[3]), static_cast<uint8_t>(vector[4]),
                         static_cast<uint16_t>(vector[5]), static_cast<uint16_t>(vector[6]),
                         static_cast<uint16_t>(vector[7]), static_cast<uint16_t>(vector[8]),
                         static_cast<uint8_t>(vector[9]), static_cast<uint16_t>(vector[10]),
                         static_cast<uint16_t>(vector[11]), static_cast<uint16_t>(vector[12]),
                         static_cast<uint8_t>(vector[13]), static_cast<int16_t>(vector[14]),
                         static_cast<uint8_t>(vector[15]), static_cast<uint8_t>(vector[16]),
                         static_cast<uint16_t>(vector[17]), static_cast<uint16_t>(vector[18]),
                         static_cast<uint8_t>(vector[19]), static_cast<uint8_t>(vector[20]),
                         static_cast<uint16_t>(vector[21]), static_cast<uint16_t>(vector[22]),
                         static_cast<int16_t>(vector[23])
                     }
                    );
                if (!scanner.getHoppingFlg())
                {
                    if (!scanner.getLinearFlg())
                    { scanner.start_scan(vector); }
                    else
                    { scanner.start_scanlin(vector); }
                }
                else
                {
                    if (!scanner.getLinearFlg())
                    { scanner.start_hopingscan(vector); }
                    else
                    { scanner.start_hopingscanlin(vector); }
                }
                DrawDone = true;
                break;
            }
            case SENDDATALIN:
            {
                ALGCODE = ALGNONE;
                scanner.readDATALin();
                break;
            }
            case FASTSCANNING:
            {
                ALGCODE = ALGNONE;
                scanner.start_fastscan(vector);
                break;
            }

            case SET_PID_GAIN:
            {
                ALGCODE = ALGNONE;
                set_GainPID((uint16_t) vector[1]);//240320
                break;
            }
            case SET_AMPLMOD_GAIN: // усиление раскачка зонда
            {
                ALGCODE = ALGNONE;
                set_GainApmlMod((uint8_t) vector[1]);
                break;
            }
            case InitDAC_BIAS_SET_POINT:
            {
                ALGCODE = ALGNONE;
                if (!flgVirtual)
                {
                    init_DACSPB(vector[1]);
                }
                break;
            }
            case InitDAC_Z:
            {
                ALGCODE = ALGNONE;
                if (!flgVirtual)
                { init_DACZ(vector[1]); }
                break;
            }
            case InitDAC_XY:
            {
                ALGCODE = ALGNONE;
                init_DACXY(vector[1]);
                break;
            }
            case SetDACZeroCmd:
            {
                ALGCODE = ALGNONE;
                set_DACZero();
                break;
            }
            case SET_BIAS:
            {
                ALGCODE = ALGNONE;
                set_Bias(vector[1]);
                break;
            }
            case SET_SETPOINT:
            {
                ALGCODE = ALGNONE;
                set_SetPoint(vector[1]);
                break;
            }
            case SET_Z:
            {
                ALGCODE = ALGNONE;
                set_DACZ(vector[1]);
                break;
            }
            case SET_XY:
            {
                ALGCODE = ALGNONE;
                init_SPI(vector[1], vector[2], vector[3], vector[4]);//29, 3, 8, 0, 1, 1, value
                if (vector[5] == 0)
                {
                    move_scannerX(vector[6]);
                }
                else if (vector[5] == 1)
                {
                    move_scannerY(vector[6]);
                }
                break;
            }
            case ADC_READCmd: //TIMER
            {
                ALGCODE = ALGNONE;
                if (ADC_IS_READY_TO_READ)
                {
                    scanner.readADC();
                }
                break;
            }
            case GET_CURRENTX0Y0:
            {
                ALGCODE = ALGNONE;
                scanner.getX0Y0();
                break;
            }
            case SCANNER_RETRACT_PROTRACT:
            {
                ALGCODE = ALGNONE;
                scanner.scanner_retract_protract(vector[1], vector[2]); //int port=6 , int flg
                break;
            }
            case SPECTROSOPY_IV:
            {
                ALGCODE = ALGNONE;
                scanner.spectroscopyIV(vector);
                break;
            }
            case SPECTROSOPY_AIZ:
            {
                ALGCODE = ALGNONE;
                scanner.spectroscopyAIZ(vector);
                break;
            }
            case RetractAlCode:
            {
                ALGCODE = ALGNONE;
                scanner.retract();
                break;
            }
            case VersionCmd:
            {
                ALGCODE = ALGNONE;
                GetSOFTHARDWAREVersion();
                break;
            }
            default:
            {
                activateError();
                break;
            }
        }
    }
}