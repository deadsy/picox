//-----------------------------------------------------------------------------
/*

Pulse Width Modulation Driver

*/
//-----------------------------------------------------------------------------

#ifndef PWM_H
#define PWM_H

//-----------------------------------------------------------------------------

struct pwm_cfg {
	PIO pio;		// pio being used
	unsigned int sm;	// state machine within PIO
	unsigned int pin;	// gpio pin for output
	float frequency;	// frequency of pwm waveform
	float duty;		// initial duty cycle 0..1
};

struct pwm_drv {
	struct pwm_cfg *cfg;
	uint32_t period;	// period counter
};

int pwm_init(struct pwm_drv *pwm, struct pwm_cfg *cfg);
void pwm_set_duty(struct pwm_drv *pwm, float duty);
float pwm_get_frequency(struct pwm_drv *pwm);

//-----------------------------------------------------------------------------

#endif				// PWM_H

//-----------------------------------------------------------------------------
