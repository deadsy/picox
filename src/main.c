//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"

#define LOG_ENABLE
#include "log.h"

//-----------------------------------------------------------------------------

static void thread(void) {

	for (int i = 0; i < 100; i++) {
		log_info("cpu1 %d", i);
		sleep_ms(300);
	}

	while (1) ;
}

//-----------------------------------------------------------------------------

int main(void) {

	stdio_init_all();

	log_init(LOG_TRACE, true);

	log_trace("trace int %d", 1);
	log_debug("debug float %f", 1.f);
	log_info("info %s", "string");
	log_warn("warn");
	log_error("error");
	log_fatal("fatal");

	multicore_launch_core1(thread);

	for (int i = 0; i < 100; i++) {
		log_info("cpu0 %d", i);
		sleep_ms(500);
	}

	while (1) ;

	return 0;
}

//-----------------------------------------------------------------------------
