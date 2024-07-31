#include <stdio.h>
#include "pico/stdlib.h" 
#include "hardware/uart.h"
#include <cstring>

// big-endian  little-endian!!!
//#include "tusb.h"
/*
#include "hardware/uart.h"

/// \tag::hello_uart[]

#define UART_ID uart0
#define BAUD_RATE 115200

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1
/
int main() {
    // Set up our UART with the required speed.
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Use some the various UART functions to send out data
    // In a default system, printf will also output via the default UART

    // Send out a character without any conversions
    uart_putc_raw(UART_ID, 'A');

    // Send out a character but do CR/LF conversions
    uart_putc(UART_ID, 'B');

    // Send out a string, with CR/LF conversions
    uart_puts(UART_ID, " Hello, UART!\n");
}
  */   
#define FPGAUART_TX_PIN 8 //!
#define FPGAUART_RX_PIN 9 //!
#define FPGA_UART_ID    uart1
#define FPGA_BAUD_RATE  115200//400000 
   
   int main()
   {
      size_t sz=12;  
      uint8_t inbuffer[sz];     
      uint8_t outbuffer[sz];      
      stdio_init_all();  
      uart_init(FPGA_UART_ID , FPGA_BAUD_RATE); //add  240627
      gpio_set_function(FPGAUART_TX_PIN, GPIO_FUNC_UART);
      gpio_set_function(FPGAUART_RX_PIN, GPIO_FUNC_UART);

    // Enable UART
      uart_set_hw_flow(FPGA_UART_ID, false, false);
      uart_set_format(FPGA_UART_ID, 8, 1, UART_PARITY_NONE);
      uart_set_fifo_enabled(FPGA_UART_ID, true);//true);
   //   uint8_t buf[1];
    while (true)         
    { 
    //  for (size_t i = 0; i < sz; i++)
      {
      if(uart_is_readable(FPGA_UART_ID ))      
      // while (!uart_is_readable(FPGA_UART_ID )) {sleep_ms(10);}     
          uart_read_blocking(FPGA_UART_ID, inbuffer,sz);
      //     uart_read_blocking(FPGA_UART_ID, buf,1);
        //  inbuffer[i]=buf[0];
      //    sleep_ms(20);
      }
    //    uart_read_blocking(FPGA_UART_ID , inbuffer,sz);
        //uart_write_blocking(uart_inst_t *uart, const uint8_t *src, size_t len)
   //     sleep_ms(400);
        for (size_t i = 0; i < sz; i++)
        {
         outbuffer[i]=inbuffer[i]; 
        }
  //   for (size_t i = 0; i < sz; i++)
     {
     //  if (uart_is_writable(FPGA_UART_ID ))       
       while (!uart_is_writable(FPGA_UART_ID )) { sleep_ms(10);}
      
        //     const char* message = "Hello, UART!";
       //    size_t len = strlen(message);
       //     uart_write_blocking(FPGA_UART_ID, (const uint8_t*)message, len);
        uart_write_blocking(FPGA_UART_ID,outbuffer,sz);
      //  buf[0]=outbuffer[i];
      //   uart_write_blocking(FPGA_UART_ID, buf,1);
     //    sleep_ms(20);             
     //    uart_write_blocking(FPGA_UART_ID, &outbuffer[i],1);
     }
    //   uart_write_blocking(FPGA_UART_ID , outbuffer,sz);
        //uart_write_blocking(uart_inst_t *uart, const uint8_t *src, size_t len)
    }
       sleep_ms(100);
 }     

