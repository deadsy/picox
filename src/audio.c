//-----------------------------------------------------------------------------
/*


*/
//-----------------------------------------------------------------------------

#if 0

#define WAVE_TABLE_LEN 2048
static int16_t wave_table[WAVE_TABLE_LEN];

#define SAMPLES_PER_BUFFER 256

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
		log_error("");
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
		log_error("");
		return NULL;
	}

	bool ok = audio_i2s_connect(producer_pool);
	if (!ok) {
		log_error("");
		return NULL;
	}

	audio_i2s_set_enabled(true);

	return producer_pool;
}

#endif

#if 0

for (int i = 0; i < WAVE_TABLE_LEN; i++) {
	wave_table[i] = (int16_t) (32767.f * cosf(i * 2.f * (float)(M_PI / WAVE_TABLE_LEN)));
}

struct audio_buffer_pool *ap = init_audio();

uint32_t step = 0x200000;
uint32_t pos = 0;
uint32_t pos_max = 0x10000 * WAVE_TABLE_LEN;
uint vol = 128;

while (true) {
	struct audio_buffer *buffer = take_audio_buffer(ap, true);

	int16_t *samples = (int16_t *) buffer->buffer->bytes;
	for (uint i = 0; i < buffer->max_sample_count; i++) {
		samples[i] = (vol * wave_table[pos >> 16u]) >> 8u;
		pos += step;
		if (pos >= pos_max)
			pos -= pos_max;
	}
	buffer->sample_count = buffer->max_sample_count;
	give_audio_buffer(ap, buffer);
}

#endif

//-----------------------------------------------------------------------------
