//-----------------------------------------------------------------------------
/*


*/
//-----------------------------------------------------------------------------

#include "ggm.h"

//-----------------------------------------------------------------------------

struct audio_buffer_pool *audio_init(void) {

	struct audio_buffer_pool *producer_pool = NULL;

	static audio_format_t audio_format = {
		.format = AUDIO_BUFFER_FORMAT_PCM_S16,
		.sample_freq = AudioSampleFrequency,
		.channel_count = 1,
	};

	static struct audio_buffer_format producer_format = {
		.format = &audio_format,
		.sample_stride = 2
	};

	producer_pool = audio_new_producer_pool(&producer_format, 3, AudioBufferSize);
	if (producer_pool == NULL) {
		log_error("");
		return NULL;
	}

	struct audio_i2s_config config = {
		.data_pin = 9,
		.clock_pin_base = 10,
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

//-----------------------------------------------------------------------------
