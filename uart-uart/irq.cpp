#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "stdbool.h"
#include "stdint.h"
#include "hardware/regs/dreq.h"
#include "CircularQue.h"
/// \tag::uart_advanced[]

#define UART_ID uart0
#define BAUD_RATE 19200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

static int chars_rxed = 0;

// RX interrupt handler
 void on_uart_rx()
 {
 	io_rw_32 dr,ris;
 	// get the interrupt bits
	ris = uart_get_hw(uart0)->ris;
	// see if Rx Interrupt
	if(ris & UART_UARTRIS_RXRIS_BITS)
	{
	// get the character from hardware
	  dr = uart_get_hw(UART_ID)->dr;
	  uart_get_hw(UART_ID)->dr = dr;
	}
	// see if Tx interrupt
	if(ris & UART_UARTRIS_TXRIS_BITS)
	{
	   while (!isQueueEmpty())
          {
            if (uart_is_writable(UART_ID))
            {
               uart_get_hw(UART_ID)->dr = deque();
            }
	  }
        //uart_get_hw(uart0)->imsc |= UART_UARTIMSC_TXIM_BITS;
        //uart_set_irq_enables(UART_ID, true, false);
          uart_get_hw(UART_ID)->icr |= UART_UARTICR_TXIC_BITS;
	}
 }

int main()
 {
    // Set up our UART with a basic baud rate.
    uart_init(UART_ID, 2400);

	for (uint16_t i = 0; i <= 2; i++)
	{
  	 enque('a');
	}
	
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Actually, we want a different speed
    // The call will return the actual baud rate selected, which will be as close as
    // possible to that requested
    uart_set_baudrate(UART_ID, BAUD_RATE);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(UART_ID, false);

    // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, true);

    // OK, all set up.
    // Lets send a basic string out, and then run a loop and wait for RX interrupts
    // The handler will count them, but also reflect the incoming data back with a slight change!
    uart_puts(UART_ID, "eu");
    uint32_t dc = 0;
    bool toggle_lev = false;
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    while (1)
    {
        dc++;
        if (dc>= 50000000)
	{
  	 dc = 0;
         for (uint16_t i = 0; i <= 2; i++)
         {
          enque('a');
         }
 	 toggle_lev = ! toggle_lev;
	 gpio_put(PICO_DEFAULT_LED_PIN, toggle_lev);
         //uart_get_hw(UART_ID)->icr &= ~UART_UARTICR_TXIC_BITS;
         uart_set_irq_enables(UART_ID, true, true);
         uart_putc(UART_ID,'m');
	}
    }
}

// End of Main.c
