//-----------------------------------------------------------------------------
/*

Pulse Width Modulation Driver

*/
//-----------------------------------------------------------------------------

#include <string.h>

#include "hardware/pio.h"
#include "hardware/clocks.h"

#include "util.h"
#include "pwm.h"

#define LOG_ENABLE
#include "log.h"

//-----------------------------------------------------------------------------

#define CLOCK_DIVISOR 1.f
#define CLOCKS_PER_CYCLE 3.f

#include "pwm.pio.h"

//-----------------------------------------------------------------------------

// Write `period` to the input shift register
static void pio_pwm_set_period(PIO pio, uint sm, uint32_t period) {
	log_info("set pio%d[%d] to period %d", PIO_NUM(pio), sm, period);
	pio_sm_set_enabled(pio, sm, false);
	pio_sm_put_blocking(pio, sm, period);
	pio_sm_exec(pio, sm, pio_encode_pull(false, false));
	pio_sm_exec(pio, sm, pio_encode_out(pio_isr, 32));
	pio_sm_set_enabled(pio, sm, true);
}

//-----------------------------------------------------------------------------

int pwm_init(struct pwm_drv *pwm, struct pwm_cfg *cfg) {
	log_info("%s() pio%d[%d] freq=%fHz, duty=%f", __func__, PIO_NUM(cfg->pio), cfg->sm, cfg->frequency, cfg->duty);

	// TODO validate config

	memset(pwm, 0, sizeof(struct pwm_drv));
	pwm->cfg = cfg;

	// load the program to the PIO
	unsigned int offset = pio_add_program(cfg->pio, &pwm_program);
	log_info("pwm pio program at ofs %d", offset);

	// initialise the PIO state machine
	pwm_program_init(cfg->pio, cfg->sm, offset, cfg->pin);

	// set the pwm frequency
	pwm->period = (uint32_t) ((float)clock_get_hz(clk_sys) / (cfg->frequency * CLOCKS_PER_CYCLE * CLOCK_DIVISOR));
	pio_pwm_set_period(cfg->pio, cfg->sm, pwm->period);

	// set initial duty cycle
	pwm_set_duty(pwm, cfg->duty);
	return 0;
}

//-----------------------------------------------------------------------------

void pwm_set_duty(struct pwm_drv *pwm, float duty) {
	duty = clamp(duty, 0.f, 1.f);
	uint32_t level = (uint32_t) (duty * (float)pwm->period);
	pio_sm_put_blocking(pwm->cfg->pio, pwm->cfg->sm, level);
}

//-----------------------------------------------------------------------------

float pwm_get_frequency(struct pwm_drv *pwm) {
	return pwm->cfg->frequency;
}

//-----------------------------------------------------------------------------
