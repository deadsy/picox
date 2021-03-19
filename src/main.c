//-----------------------------------------------------------------------------
/*

Entry Point

*/
//-----------------------------------------------------------------------------

#define LOG_ENABLE

#include <math.h>
#include "pico/stdio.h"
#include "ggm.h"

//-----------------------------------------------------------------------------

int main(void) {

	stdio_init_all();
	log_init(LOG_TRACE, true);

	log_info("sine wave demo");

	rms_error("cosf/cosf_fast", 0.f, 4.f * Tau, 0.01f, cosf, cosf_fast);
	benchmark_f0("cos_q15", cos_q15, 2048);
	benchmark_f1("cosf_fast", cosf_fast, 2048);
	benchmark_f1("cosf", cosf, 2048);

	rms_error("pow2_slow/pow2_fast", 0.f, 1.f, 0.01f, pow2_slow, pow2_fast);
	benchmark_f1("pow2_slow", pow2_slow, 2048);
	benchmark_f1("pow2_fast", pow2_fast, 2048);

	rms_error("m2f_slow/m2f_fast", 0.f, 127.f, 0.3f, m2f_slow, m2f_fast);
	benchmark_f1("m2f_slow", m2f_slow, 2048);
	benchmark_f1("m2f_fast", m2f_fast, 2048);
	benchmark_f2("m2f", m2f, 2048);

	while (1) {
	}

	return 0;
}

//-----------------------------------------------------------------------------
