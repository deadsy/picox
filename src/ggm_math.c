//-----------------------------------------------------------------------------
/*

Picox Math Functions.

*/
//-----------------------------------------------------------------------------

#define LOG_ENABLE

#include <math.h>
#include <float.h>

#include "ggm.h"

//-----------------------------------------------------------------------------
// Fast fixed point cosine using a LUT.
// error: min 0.000000 max 0.000343 rms 0.000170

#define COS_LUT_BITS (7U)
#define COS_LUT_SIZE (1U << COS_LUT_BITS)

static const int16_t COS_LUT_data[COS_LUT_SIZE + 1] = {
	32767, 32728, 32610, 32413, 32138, 31785, 31357, 30852,
	30273, 29621, 28898, 28106, 27245, 26319, 25330, 24279,
	23170, 22005, 20787, 19519, 18204, 16846, 15446, 14010,
	12539, 11039, 9512, 7961, 6392, 4808, 3211, 1607,
	0, -1607, -3211, -4808, -6392, -7961, -9512, -11039,
	-12539, -14010, -15446, -16846, -18204, -19519, -20787, -22005,
	-23170, -24279, -25330, -26319, -27245, -28106, -28898, -29621,
	-30273, -30852, -31357, -31785, -32138, -32413, -32610, -32728,
	-32768, -32728, -32610, -32413, -32138, -31785, -31357, -30852,
	-30273, -29621, -28898, -28106, -27245, -26319, -25330, -24279,
	-23170, -22005, -20787, -19519, -18204, -16846, -15446, -14010,
	-12539, -11039, -9512, -7961, -6392, -4808, -3211, -1607,
	0, 1607, 3211, 4808, 6392, 7961, 9512, 11039,
	12539, 14010, 15446, 16846, 18204, 19519, 20787, 22005,
	23170, 24279, 25330, 26319, 27245, 28106, 28898, 29621,
	30273, 30852, 31357, 31785, 32138, 32413, 32610, 32728,
	32767,
};

#define PHASE_FRAC_BITS (32 - COS_LUT_BITS)

// fixed point cosine: runs at ~0.26 usecs/call
q15 cos_q15(uint32_t x) {
	uint32_t i = x >> PHASE_FRAC_BITS;
	uint32_t frac = (x >> (PHASE_FRAC_BITS - 16)) & 0xffff;
	int32_t y0 = COS_LUT_data[i];
	int32_t y1 = COS_LUT_data[i + 1];
	int32_t dy = ((y1 - y0) * frac) >> 16;
	return (int16_t) (y0 + dy);
}

// cosf equivalent for test purposes
// Note: This runs at ~4.6 usecs/call vs ~5.8 usecs/call for cosf.
float ggm_cosf(float x) {
	uint32_t xi = (uint32_t) (fmodf(x, Tau) * PhaseScale);
	return q15_to_float(cos_q15(xi));
}

//-----------------------------------------------------------------------------
// midi to frequency conversion

#define M2F_LUT_BITS (7U)
#define M2F_LUT_SIZE (1U << M2F_LUT_BITS)

static const uint32_t M2F_LUT_data[M2F_LUT_SIZE + 1] = {
	0x000b29a6, 0x000bd392, 0x000c879a, 0x000d4656, 0x000e1069, 0x000ee680, 0x000fc953, 0x0010b9a2,
	0x0011b83c, 0x0012c5f9, 0x0013e3c0, 0x00151285, 0x0016534c, 0x0017a725, 0x00190f34, 0x001a8cac,
	0x001c20d2, 0x001dcd01, 0x001f92a6, 0x00217345, 0x00237078, 0x00258bf2, 0x0027c780, 0x002a250b,
	0x002ca698, 0x002f4e4b, 0x00321e68, 0x00351958, 0x003841a5, 0x003b9a03, 0x003f254d, 0x0042e68a,
	0x0046e0f0, 0x004b17e4, 0x004f8f01, 0x00544a17, 0x00594d30, 0x005e9c96, 0x00643cd1, 0x006a32b0,
	0x0070834b, 0x00773407, 0x007e4a9b, 0x0085cd15, 0x008dc1e0, 0x00962fc9, 0x009f1e02, 0x00a8942e,
	0x00b29a61, 0x00bd392c, 0x00c879a3, 0x00d46561, 0x00e10697, 0x00ee680e, 0x00fc9536, 0x010b9a2a,
	0x011b83c1, 0x012c5f92, 0x013e3c05, 0x0151285c, 0x016534c3, 0x017a7259, 0x0190f346, 0x01a8cac3,
	0x01c20d2e, 0x01dcd01d, 0x01f92a6c, 0x02173455, 0x02370783, 0x0258bf25, 0x027c780b, 0x02a250b9,
	0x02ca6986, 0x02f4e4b3, 0x0321e68d, 0x03519586, 0x03841a5d, 0x03b9a03a, 0x03f254d9, 0x042e68ab,
	0x046e0f06, 0x04b17e4b, 0x04f8f016, 0x0544a173, 0x0594d30d, 0x05e9c967, 0x0643cd1a, 0x06a32b0c,
	0x070834ba, 0x07734074, 0x07e4a9b2, 0x085cd157, 0x08dc1e0d, 0x0962fc96, 0x09f1e02d, 0x0a8942e6,
	0x0b29a61a, 0x0bd392cf, 0x0c879a35, 0x0d465619, 0x0e106974, 0x0ee680e9, 0x0fc95364, 0x10b9a2ae,
	0x11b83c1a, 0x12c5f92c, 0x13e3c05a, 0x151285cd, 0x16534c34, 0x17a7259f, 0x190f346a, 0x1a8cac33,
	0x1c20d2e8, 0x1dcd01d2, 0x1f92a6c8, 0x2173455d, 0x23707834, 0x258bf258, 0x27c780b4, 0x2a250b9b,
	0x2ca69869, 0x2f4e4b3f, 0x321e68d4, 0x35195867, 0x3841a5d0, 0x3b9a03a5, 0x3f254d90, 0x42e68abb,
	0x46e0f069,
};

uint32_t m2f(q24 note) {
	uint32_t i = (note >> 24) & 127;
	uint32_t frac = (note >> 8) & 0xffff;
	uint32_t s0 = M2F_LUT_data[i];
	uint32_t s1 = M2F_LUT_data[i + 1];
	uint32_t ds = ((s1 - s0) * frac) >> 16;
	return s0 + ds;
}

// fast float version of m2f for test purposes.
float m2f_fast(float note) {
	note = clamp(note, 0.f, 127.f);
	uint32_t dx = m2f(float_to_q24(note));
	return (float)dx *(1.f / FrequencyScale);
}

// slow/accurate float version of m2f using math.h functions (~19.8 usecs/call)
float m2f_slow(float note) {
	note = clamp(note, 0.f, 127.f);
	return 440.f * powf(2.f, (note - 69.f) * (1.f / 12.f));
}

//-----------------------------------------------------------------------------
// Error Analysis
// Compare a "fast" function against the known-accurate "slow" function
// Log the error stats to see how good the fast function is.

void rms_error(const char *name, float x0, float x1, float dx, float (*f0)(float), float (*f1)(float)) {
	float x = x0;
	float max_err = FLT_MIN;
	float min_err = FLT_MAX;
	float sum_err2 = 0.f;
	int n = 0;
	while (x < x1) {
		float y0 = f0(x);
		float y1 = f1(x);
		float err = fabsf(y1 - y0);
		//log_debug("x %f y0 %f y1 %f err %f", x, y0, y1, err);
		sum_err2 += err * err;
		if (err < min_err) {
			min_err = err;
		}
		if (err > max_err) {
			max_err = err;
		}
		x += dx;
		n++;
	}
	float rms_err = sqrtf(sum_err2 / (float)n);
	log_info("%s: min %f max %f rms %f", name, min_err, max_err, rms_err);
}

//-----------------------------------------------------------------------------
// function benchmarks

// benchmark int16_t func(uint32_t x)
void benchmark_f0(const char *name, int16_t(*f0) (uint32_t), unsigned int n) {
	uint32_t x = 0;
	unsigned int i = 0;
	absolute_time_t base = get_absolute_time();
	while (i < n) {
		f0(x);
		x += 17;
		i++;
	}
	int64_t usecs = absolute_time_diff_us(base, get_absolute_time());
	float usecs_per = (float)usecs / (float)n;
	log_info("%s: %d calls, %" PRId64 " usecs (%f usecs/call)", name, n, usecs, usecs_per);
}

// benchmark float func(float x)
void benchmark_f1(const char *name, float (*f1)(float), unsigned int n) {
	float x = 0.f;
	unsigned int i = 0;
	absolute_time_t base = get_absolute_time();
	while (i < n) {
		f1(x);
		x += 0.05f;
		i++;
	}
	int64_t usecs = absolute_time_diff_us(base, get_absolute_time());
	float usecs_per = (float)usecs / (float)n;
	log_info("%s: %d calls, %" PRId64 " usecs (%f usecs/call)", name, n, usecs, usecs_per);
}

// benchmark uint32_t func(q24  x)
void benchmark_f2(const char *name, uint32_t(*f2) (q24), unsigned int n) {
	q24 x = 0;
	unsigned int i = 0;
	absolute_time_t base = get_absolute_time();
	while (i < n) {
		f2(x);
		x += (1U << 23);	// 0.5
		i++;
	}
	int64_t usecs = absolute_time_diff_us(base, get_absolute_time());
	float usecs_per = (float)usecs / (float)n;
	log_info("%s: %d calls, %" PRId64 " usecs (%f usecs/call)", name, n, usecs, usecs_per);
}

//-----------------------------------------------------------------------------
