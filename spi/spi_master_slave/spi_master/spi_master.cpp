#include <iostream>
#include "loop/common_data/common_variables.hpp"
#include "loop/main_core.hpp"
#include "utilities/peripheral_functions.hpp"
#include "hardware/uart.h"
#include "hardware/dma.h"

int start_app() {
    critical_section_init(&criticalSection);
    if (!critical_section_is_initialized(&criticalSection)) {
        activateError();
    }
    int i = 0;
    while (i++ < 3)
    {
        activateGreen();
        sleep_ms(100);
        activateBlue();
        sleep_ms(100);
        activateRed();
        sleep_ms(100);
        activateDark();
        sleep_ms(100);
    }
    setDefaultSettings();
    MainCore mainCore;
    mainCore.loop();
    return 0;
}

//#define UART_TX_PIN 8
//#define UART_RX_PIN 9
//
//void testUart() {
//    // TODO ADD TO PROTOCOL AND SPI TOO
//    uart_init(uart1, 135200);
//    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
//    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
//    while (1) {
//
//        uart_puts(uart1, "String for uart");
////    sleep_ms(10000);
//        std::cout << "str for all\n";
//        sleep_ms(10000);
//    }
//}




int main() {

    return start_app();
}
