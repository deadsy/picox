//-----------------------------------------------------------------------------
/*

PIO Code Testing and Experiments

*/
//-----------------------------------------------------------------------------

#include "pico/stdio.h"
#include "hardware/pio.h"

#include "pwm.h"

#define LOG_ENABLE
#include "log.h"

//-----------------------------------------------------------------------------

static struct pwm_cfg pwm0_cfg = { pio0, 0, 16, 50.f, 0.1 };

static struct pwm_drv pwm0;

static int app_main(void) {
	log_info("%s()", __func__);
	int rc = 0;

	rc = pwm_init(&pwm0, &pwm0_cfg);
	if (rc != 0) {
		goto exit;
	}

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
	log_info("exit rc %d", rc);
	while (1) {
		sleep_ms(250);
	}
	return 0;
}

//-----------------------------------------------------------------------------
