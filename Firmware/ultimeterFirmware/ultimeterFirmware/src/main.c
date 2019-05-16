/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include "conf_twi.h"
#include <string.h>

/** USART0 pin RX */
#define PIN_USART0_RXD	  {PIO_PA5A_RXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_RXD_IDX        (PIO_PA5_IDX)
#define PIN_USART0_RXD_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)
/** USART0 pin TX */
#define PIN_USART0_TXD    {PIO_PA6A_TXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_TXD_IDX        (PIO_PA6_IDX)
#define PIN_USART0_TXD_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)
/** USART0 pin CTS */
#define PIN_USART0_CTS    {PIO_PA8A_CTS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_CTS_IDX        (PIO_PA8_IDX)
#define PIN_USART0_CTS_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)

#define BOARD_USART					USART0
#define BOARD_USART_ID				ID_USART0
#define BOARD_USART_BAUDRATE		115200
#define WIFI_USART_HANDLER			USART0_Handler
#define BOARD_USART_IRQn			USART0_IRQn
#define BOARD_USART_CHAR_LENGTH		US_MR_CHRL_8_BIT
#define BOARD_USART_PARITY			US_MR_PAR_NO
#define BOARD_USART_STOP_BITS		US_MR_NBSTOP_1_BIT
#define BOARD_USART_MODE			US_MR_CHMODE_NORMAL


int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	
	
	ioport_set_pin_dir(PIO_PA7_IDX, IOPORT_DIR_OUTPUT);
	configure_twi();
	/* Insert application code here, after the board has been initialized. */
			volatile double angle = 0;
		volatile uint32_t data = 0b1100;
		volatile uint32_t data_1 = 0x00;
		volatile uint32_t data_2 = 0x00;
		twi_packet_t rx_packet = {
			.chip = BNO055_ADDR,
			.addr = 0x3D,
			.addr_length = 1,
			.buffer = &data,
			.length = 1
			};
			
		int status = twi_master_write(BOARD_TWI, &rx_packet);
		
		status = twi_master_read(BOARD_TWI, &rx_packet);
		
			gpio_configure_pin(PIN_USART0_RXD_IDX, PIN_USART0_RXD_FLAGS);
			gpio_configure_pin(PIN_USART0_TXD_IDX, PIN_USART0_TXD_FLAGS);
			gpio_configure_pin(PIN_USART0_CTS_IDX, PIN_USART0_CTS_FLAGS);
			
			static uint32_t ul_sysclk;
			const sam_usart_opt_t usart_console_settings = {
				BOARD_USART_BAUDRATE,
				US_MR_CHRL_8_BIT,
				US_MR_PAR_NO,
				US_MR_NBSTOP_1_BIT,
				US_MR_CHMODE_NORMAL,
				/* This field is only used in IrDA mode. */
				0
			};
			/* Enable peripheral clock. */
			ul_sysclk = sysclk_get_peripheral_hz();
			sysclk_enable_peripheral_clock(BOARD_USART_ID);

			/* Configure USART. */
			usart_init_hw_handshaking(BOARD_USART, &usart_console_settings, ul_sysclk);

			/* Disable all the interrupts. */
			usart_disable_interrupt(BOARD_USART, 0xffffffff);
			
			/* Enable TX & RX function. */
			usart_enable_tx(BOARD_USART);
			usart_enable_rx(BOARD_USART);
			
			usart_enable_interrupt(BOARD_USART, US_IER_RXRDY);

			/* Configure and enable interrupt of USART. */
			NVIC_EnableIRQ(BOARD_USART_IRQn);
			
		
	while(1) {

		
		
		twi_packet_t rx_packet_0 = {
			.chip = BNO055_ADDR,
			.addr = 0x1A,
			.addr_length = 1,
			.buffer = &data,
			.length = 1
		};
		twi_packet_t rx_packet_1 = {
			.chip = BNO055_ADDR,
			.addr = 0x1B,
			.addr_length = 1,
			.buffer = &data_1,
			.length = 1
		};
		data_2 = 0x0001;
		twi_packet_t rx_packet_2 = {
			.chip = BATTRY_ADDR,
			.addr = 0x0000,
			.addr_length = 2,
			.buffer = &data_2,
			.length = 2
		};
			
		status = twi_master_read(BOARD_TWI, &rx_packet_0);
		status = twi_master_read(BOARD_TWI, &rx_packet_1);
		status = twi_master_write(BOARD_TWI, &rx_packet_2);
		status = twi_master_read(BOARD_TWI, &rx_packet_2);
		
		data = ((data_1 << 8) | (data));
		angle = ((double) data) / 16.0;
		data = 0x0;
		data_1 = 0x0;
		
		char charArray[100];
		sprintf(charArray, "%g", angle);
		//usart_write_line(BOARD_USART, &charArray);

		
		//delay_ms(1000);
		/*
		ioport_set_pin_level(PIO_PA7_IDX, false);
		delay_ms(10);
		ioport_set_pin_level(PIO_PA7_IDX, true);
		delay_ms(10);
		*/
		
		
		status++;
	}
	
	
}
