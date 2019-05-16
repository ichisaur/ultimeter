/*
 * conf_twi.c
 *
 * Created: 5/6/2019 8:59:21 PM
 *  Author: ichis
 */ 

#include "conf_twi.h"

void configure_twi(void)
{
	
	gpio_configure_pin(TWI0_DATA_GPIO, TWI0_DATA_FLAGS);
	gpio_configure_pin(TWI0_CLK_GPIO, TWI0_CLK_FLAGS);
	
	twi_options_t opt;
	
	/* Enable TWI peripheral */
	pmc_enable_periph_clk(ID_BOARD_TWI);

	/* Init TWI peripheral */
	opt.master_clk = sysclk_get_cpu_hz();
	opt.speed      = TWI_CLK;
	//opt.chip		= BNO055_ADDR;
	twi_master_init(BOARD_TWI, &opt);

	/* Configure TWI interrupts */
	NVIC_DisableIRQ(BOARD_TWI_IRQn);
	NVIC_ClearPendingIRQ(BOARD_TWI_IRQn);
	NVIC_SetPriority(BOARD_TWI_IRQn, 0);
	NVIC_EnableIRQ(BOARD_TWI_IRQn);
}

uint32_t read8(uint8_t chip, uint8_t addr) {

	uint32_t data; 

	twi_packet_t txPacket = {
		.chip = chip,
		.addr = addr,
		.addr_length = 1,
		.buffer = &data,
		.length = 1
	};

	twi_master_read(BOARD_TWI, &txPacket);

	return data;
	
}

uint32_t write8(uint8_t chip, uint8_t addr, uint8_t data) {
	uint8_t txdata = data;
	uint32_t rxdata;

	twi_packet_t txPacket = {
		.chip = chip,
		.addr = addr,
		.addr_length = 1,
		.buffer = &tddata,
		.length = 1
	}

	twi_packet_t rxPacket = {
		.chip = chip,
		.addr = addr,
		.addr_length = 1,
		.buffer = &rxdata,
		.length = 1
	}

	twi_master_write(BOARD_TWI, &txPacket);
	twi_master_read(BOARD_TWI, &rxPacket);

	return rxdata;

}