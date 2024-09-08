//-----------------------------------------------------------------------------
/*


*/
//-----------------------------------------------------------------------------

#include <inttypes.h>
#include <math.h>

#include "pico/stdlib.h"
#include "pico/audio_i2s.h"

//-----------------------------------------------------------------------------

#define UART uart0

#define SINE_WAVE_TABLE_LEN 2048
static int16_t sine_wave_table[SINE_WAVE_TABLE_LEN];

#define SAMPLES_PER_BUFFER 256

//-----------------------------------------------------------------------------

static void con_puts(const char *s) {
	uart_puts(UART, s);
}

static void con_putc(char c) {
	uart_putc(UART, c);
}

static void put_nybble(int x) {
	x &= 15;
	con_putc((x <= 9) ? ('0' + x) : ('a' + x - 10));
}

static void put_hex8(uint8_t x) {
	put_nybble(x >> 4);
	put_nybble(x);
}

static void put_hex16(uint16_t x) {
	put_hex8(x >> 8);
	put_hex8(x);
}

static void put_hex32(uint32_t x) {
	put_hex16(x >> 16);
	put_hex16(x);
}

//-----------------------------------------------------------------------------

struct audio_buffer_pool *init_audio(void) {

	struct audio_buffer_pool *producer_pool = NULL;

	static audio_format_t audio_format = {
		.format = AUDIO_BUFFER_FORMAT_PCM_S16,
		.sample_freq = 24000,
		.channel_count = 1,
	};

	static struct audio_buffer_format producer_format = {
		.format = &audio_format,
		.sample_stride = 2
	};

	producer_pool = audio_new_producer_pool(&producer_format, 3, SAMPLES_PER_BUFFER);
	if (producer_pool == NULL) {
		con_puts("producer_poll == NULL\r\n");
		return NULL;
	}

	struct audio_i2s_config config = {
		.data_pin = 9,	//PICO_AUDIO_I2S_DATA_PIN,
		.clock_pin_base = 10,	//PICO_AUDIO_I2S_CLOCK_PIN_BASE,
		.dma_channel = 0,
		.pio_sm = 0,
	};

	const struct audio_format *output_format = audio_i2s_setup(&audio_format, &config);
	if (output_format == NULL) {
		con_puts("unable to open audio device\r\n");
		return NULL;
	}

	bool ok = audio_i2s_connect(producer_pool);
	if (!ok) {
		con_puts("audio_i2s_connect failed\r\n");
		return NULL;
	}

	audio_i2s_set_enabled(true);

	return producer_pool;
}

//-----------------------------------------------------------------------------

#define LED_PIN 25

static void initIO(void) {
	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);

	uart_init(UART, 115200);
	gpio_set_function(0, GPIO_FUNC_UART);
	gpio_set_function(1, GPIO_FUNC_UART);
}

//-----------------------------------------------------------------------------

int main(void) {

	initIO();

	con_puts("sine wave demo\r\n");

	for (int i = 0; i < SINE_WAVE_TABLE_LEN; i++) {
		sine_wave_table[i] = 32767 * cosf(i * 2 * (float)(M_PI / SINE_WAVE_TABLE_LEN));
	}

	struct audio_buffer_pool *ap = init_audio();

	con_puts("ap = 0x");
	put_hex32((uint32_t) ap);
	con_puts("\r\n");

	uint32_t step = 0x200000;
	uint32_t pos = 0;
	uint32_t pos_max = 0x10000 * SINE_WAVE_TABLE_LEN;
	uint vol = 128;

	while (true) {
		struct audio_buffer *buffer = take_audio_buffer(ap, true);

		con_puts("buffer = 0x");
		put_hex32((uint32_t) buffer);
		con_puts("\r\n");

		int16_t *samples = (int16_t *) buffer->buffer->bytes;
		for (uint i = 0; i < buffer->max_sample_count; i++) {
			samples[i] = (vol * sine_wave_table[pos >> 16u]) >> 8u;
			pos += step;
			if (pos >= pos_max)
				pos -= pos_max;
		}
		buffer->sample_count = buffer->max_sample_count;
		give_audio_buffer(ap, buffer);
	}

	con_puts("done\r\n");

	return 0;
}

//-----------------------------------------------------------------------------
