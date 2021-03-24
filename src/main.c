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

#define VOL_INC 0.01f

int main(void) {

	stdio_init_all();
	log_init(LOG_TRACE, true);

	log_info("sine wave demo");

	log_info("%d samples/sec", AudioSampleFrequency);
	log_info("%d bytes/buffer", AudioBufferSize);
	log_info("%.2f ms/buffer", SecsPerAudioBuffer * 1000.f);

	struct audio_buffer_pool *ap = audio_init_old();
	if (ap == NULL) {
		log_error("");
		goto exit;
	}

	uint32_t phase = 0;

	float vol = 0.1f;
	q15 voli = float_to_q15(vol);

	int note = 69;
	uint32_t step = m2f(int_to_q24(note));

	while (true) {

		int c = getchar_timeout_us(0);
		if (c >= 0) {
			switch (c) {
			case '-':
				vol = clampf(vol - VOL_INC, 0, 1.f);
				break;
			case '+':
				vol = clampf(vol + VOL_INC, 0, 1.f);
				break;
			case '[':
				note = clampi(note - 1, 0, 127);
				break;
			case ']':
				note = clampi(note + 1, 0, 127);
				break;
			default:
				break;
			}
			voli = float_to_q15(vol);
			step = m2f(int_to_q24(note));

			log_info("vol %.3f note %d frequency %.2f Hz", vol, note, m2f_fast(note));
		}

		struct audio_buffer *buffer = take_audio_buffer(ap, true);
		int16_t *samples = (int16_t *) buffer->buffer->bytes;
		for (unsigned int i = 0; i < buffer->max_sample_count; i++) {
			samples[i] = mul_q15(voli, cos_q15(phase));
			phase += step;
		}
		buffer->sample_count = buffer->max_sample_count;
		give_audio_buffer(ap, buffer);
	}

#if 0

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

#endif

 exit:
	while (1) {
	}
	return 0;
}

//-----------------------------------------------------------------------------
