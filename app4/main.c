//-----------------------------------------------------------------------------
/*

PIO Code Testing and Experiments

*/
//-----------------------------------------------------------------------------

#include "pico/stdio.h"
#include "hardware/pio.h"

#define LOG_ENABLE
#include "log.h"

#include "pwm.pio.h"

//-----------------------------------------------------------------------------

// Write `period` to the input shift register
static void pio_pwm_set_period(PIO pio, uint sm, uint32_t period) {
	pio_sm_set_enabled(pio, sm, false);
	pio_sm_put_blocking(pio, sm, period);
	pio_sm_exec(pio, sm, pio_encode_pull(false, false));
	pio_sm_exec(pio, sm, pio_encode_out(pio_isr, 32));
	pio_sm_set_enabled(pio, sm, true);
}

// Write `level` to TX FIFO. State machine will copy this into X.
static void pio_pwm_set_level(PIO pio, uint sm, uint32_t level) {
	pio_sm_put_blocking(pio, sm, level);
}

//-----------------------------------------------------------------------------

#define PWM_PIN 16

static int app_main(void) {
	log_info("%s()", __func__);
	int rc = 0;

	PIO pio = pio0;
	int sm = 0;
	uint offset = pio_add_program(pio, &pwm_program);
	log_info("loaded program at %d", offset);

	pwm_program_init(pio, sm, offset, PWM_PIN);
	pio_pwm_set_period(pio, sm, (1u << 16) - 1);

	int level = 0;
	while (true) {
		log_info("level %d", level);
		pio_pwm_set_level(pio, sm, level * level);
		level = (level + 1) % 256;
		sleep_ms(20);
	}

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
