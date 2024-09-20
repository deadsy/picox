//-----------------------------------------------------------------------------
/*

PIO Code Testing and Experiments

*/
//-----------------------------------------------------------------------------

#include "pico/stdio.h"
#include "hardware/pio.h"

#define LOG_ENABLE
#include "log.h"

#include "blinker.pio.h"

//-----------------------------------------------------------------------------

#if 0

static int appx_main(void) {
	log_info("%s()", __func__);
	int rc = 0;

	PIO pio = pio0;

	rc = pio_add_program(pio, &blinker_program);
	if (rc != 0) {
		goto exit;
	}
	// setup the clock divider
	pio->sm[0].clkdiv = (uint32_t) (2.5f * (1 << 16));

	// setup the pin mapping
	pio->sm[0].pinctrl = (1 << PIO_SM0_PINCTRL_SET_COUNT_LSB) | (0 << PIO_SM0_PINCTRL_SET_BASE_LSB);
	gpio_set_function(0, pio_get_funcsel(pio));

	// start the state machine
	hw_set_bits(&pio->ctrl, 1 << (PIO_CTRL_SM_ENABLE_LSB + 0));

 exit:
	return rc;
}

#endif

//-----------------------------------------------------------------------------

static int app_main(void) {
	log_info("%s()", __func__);

	PIO pio = pio0;

	// load the program
	for (uint i = 0; i < count_of(blinker_program_instructions); i++) {
		pio->instr_mem[i] = blinker_program_instructions[i];
	}

	// setup the clock divider
	pio->sm[0].clkdiv = (uint32_t) (2.5f * (1 << 16));

	// setup the pin mapping
	pio->sm[0].pinctrl = (1 << PIO_SM0_PINCTRL_SET_COUNT_LSB) | (0 << PIO_SM0_PINCTRL_SET_BASE_LSB);
	gpio_set_function(0, pio_get_funcsel(pio));

	// start the state machine
	hw_set_bits(&pio->ctrl, 1 << (PIO_CTRL_SM_ENABLE_LSB + 0));

	return 0;
}

//-----------------------------------------------------------------------------

int main(void) {
	int rc = 0;

	stdio_init_all();
	log_init(LOG_TRACE, true);
	log_info("%s()", __func__);

	rc = app_main();
	if (rc != 0) {
		goto exit;
	}

 exit:
	if (rc != 0) {
		log_error("rc %d", rc);
	}
	while (1) {
	}
	return 0;
}

//-----------------------------------------------------------------------------
