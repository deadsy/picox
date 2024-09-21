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

static int setup_pio(const pio_program_t * program, PIO * pio, uint * sm, uint * offset, uint pin, uint16_t divisor) {
	log_info("%s()", __func__);

	// load the program on a free pio and state machine
	if (!pio_claim_free_sm_and_add_program(program, pio, sm, offset)) {
		return -1;
	}
	log_info("using pio%d[%d] @ offset %d", PIO_NUM(*pio), *sm, *offset);

	// Set this pin's GPIO function (connect PIO to the pad)
	pio_gpio_init(*pio, pin);

	// Set the pin direction to output with the PIO
	pio_sm_set_consecutive_pindirs(*pio, *sm, pin, 1, true);

	pio_sm_config c = blinker_program_get_default_config(*offset);
	sm_config_set_set_pins(&c, pin, 1);

	// Load our configuration, and jump to the start of the program
	pio_sm_init(*pio, *sm, *offset, &c);

	// set the pio divisor
	pio_sm_set_clkdiv(*pio, *sm, divisor);
	return 0;
}

//-----------------------------------------------------------------------------

#define GPIO_PIN0 16
#define CLOCK_DIVISOR 65535	// Clock divisor to use, as slow as possible

static int app_main(void) {
	log_info("%s()", __func__);
	int rc = 0;

	PIO pio[3];
	uint sm[3];
	uint offset[3];

	rc = setup_pio(&blinker_program, &pio[0], &sm[0], &offset[0], GPIO_PIN0, CLOCK_DIVISOR);
	if (rc != 0) {
		goto exit;
	}

	pio_sm_set_enabled(pio[0], sm[0], true);

 exit:
	return rc;
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
	log_info("exit rc=%d", rc);
	while (1) {
		sleep_ms(250);
	}
	return 0;
}

//-----------------------------------------------------------------------------
