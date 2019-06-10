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
int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */

	board_init();
	
	
	ioport_set_pin_dir(PIO_PA7_IDX, IOPORT_DIR_OUTPUT);
	configure_twi();
	/* Insert application code here, after the board has been initialized. */
	
	while(1) {
		
		uint32_t data = 0x0;
		
		twi_packet_t rx_packet = {
			.chip = BMP388_ADDR,
			.addr = 0x00,
			.addr_length = 1,
			.buffer = &data,
			.length = 2
			};
			
		int status = twi_master_read(BOARD_TWI, &rx_packet);
		/*
		ioport_set_pin_level(PIO_PA7_IDX, false);
		delay_ms(10);
		ioport_set_pin_level(PIO_PA7_IDX, true);
		delay_ms(10);
		*/
		
		
		status++;
	}
	
	
}
