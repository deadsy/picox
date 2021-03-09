//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

#include "pico/stdio.h"
#include "pico/stdlib.h"

#define LOG_ENABLE
#include "log.h"

//-----------------------------------------------------------------------------

int main(void) {

	stdio_init_all();

	log_trace("trace int %d", 1);
	log_debug("debug float %f", 1.f);
	log_info("info %s", "string");
	log_warn("warn");
	log_error("error");
	log_fatal("fatal");

	for (int i = 0; i < 100; i++) {
		log_info("i %d", i);
		sleep_ms(500);
	}

	return 0;
}

//-----------------------------------------------------------------------------
