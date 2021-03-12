//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

#include "pico/stdio.h"

#define LOG_ENABLE
#include "log.h"

#include "const.h"
#include "math.h"

//-----------------------------------------------------------------------------

int main(void) {

	stdio_init_all();
	log_init(LOG_TRACE, true);

	log_info("sine wave demo");

	float x = 0.f;
	while (x < 4.f * Pi) {
		log_info("cosf(%f) = %f", x, cosf(x));
		x += 0.05f;
	}

	while (1) {
	}
	return 0;
}

//-----------------------------------------------------------------------------
