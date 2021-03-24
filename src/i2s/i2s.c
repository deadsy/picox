//-----------------------------------------------------------------------------
/*

I2S Driver

*/
//-----------------------------------------------------------------------------

#include <string.h>

#include "hardware/pio.h"

#include "i2s.h"
#include "i2s.pio.h"

//-----------------------------------------------------------------------------

static inline PIO get_pio(struct i2s_cfg *cfg) {
	return cfg->pio ? pio1 : pio0;
}

static inline unsigned int get_gpio_func(struct i2s_cfg *cfg) {
	return cfg->pio ? GPIO_FUNC_PIO1 : GPIO_FUNC_PIO0;
}

//-----------------------------------------------------------------------------

int i2s_init(struct i2s_drv *i2s, struct i2s_cfg *cfg) {

	memset(i2s, 0, sizeof(struct i2s_drv));
	i2s->cfg = *cfg;

	uint func = get_gpio_func(cfg);
	gpio_set_function(cfg->data_pin, func);
	gpio_set_function(cfg->clock_pin_base, func);
	gpio_set_function(cfg->clock_pin_base + 1, func);

	PIO pio = get_pio(cfg);
	pio_sm_claim(pio, cfg->sm);
	uint offset = pio_add_program(pio, &i2s_program);
	i2s_program_init(pio, cfg->sm, offset, cfg->data_pin, cfg->clock_pin_base);

	return 0;
}

//-----------------------------------------------------------------------------
