//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

#include "pico/stdio.h"

#define LOG_ENABLE
#include "picox.h"

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

	x = 0.f;
	while (x < 128.f) {
		log_info("midi_to_frequency(%f) = %f", x, midi_to_frequency(x));
		x += 1.f;
	}

	x = 0.f;
	while (x < 1.f) {
		log_info("pow2_frac(%f) = %f", x, pow2(x));
		x += 0.01f;
	}

	while (1) {
	}

	return 0;
}

//-----------------------------------------------------------------------------
