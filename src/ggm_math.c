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

// fixed point cosine
q15 cos_q15(uint32_t x) {
	uint32_t i = x >> PHASE_FRAC_BITS;
	uint32_t frac = (x >> (PHASE_FRAC_BITS - 16)) & 0xffff;
	int32_t y0 = COS_LUT_data[i];
	int32_t y1 = COS_LUT_data[i + 1];
	int32_t dy = ((y1 - y0) * frac) >> 16;
	return (int16_t) (y0 + dy);
}

// fast float version of cosf for test purposes
float cosf_fast(float x) {
	uint32_t xi = (uint32_t) (fmodf(x, Tau) * PhaseScale);
	return q15_to_float(cos_q15(xi));
}

//-----------------------------------------------------------------------------
// fast pow2(x) where x = [0,1)

static const uint16_t exp0_table[128] = {
	0x8000, 0x80b2, 0x8165, 0x8219, 0x82ce, 0x8383, 0x843a, 0x84f2, 0x85ab, 0x8665, 0x871f, 0x87db, 0x8898, 0x8956, 0x8a15, 0x8ad5,
	0x8b96, 0x8c58, 0x8d1b, 0x8ddf, 0x8ea4, 0x8f6b, 0x9032, 0x90fa, 0x91c4, 0x928e, 0x935a, 0x9427, 0x94f5, 0x95c4, 0x9694, 0x9765,
	0x9838, 0x990c, 0x99e0, 0x9ab6, 0x9b8d, 0x9c65, 0x9d3f, 0x9e19, 0x9ef5, 0x9fd2, 0xa0b0, 0xa190, 0xa270, 0xa352, 0xa435, 0xa519,
	0xa5ff, 0xa6e6, 0xa7ce, 0xa8b7, 0xa9a1, 0xaa8d, 0xab7a, 0xac69, 0xad58, 0xae49, 0xaf3b, 0xb02f, 0xb124, 0xb21a, 0xb312, 0xb40b,
	0xb505, 0xb601, 0xb6fe, 0xb7fc, 0xb8fc, 0xb9fd, 0xbaff, 0xbc03, 0xbd09, 0xbe0f, 0xbf18, 0xc021, 0xc12c, 0xc239, 0xc347, 0xc456,
	0xc567, 0xc67a, 0xc78d, 0xc8a3, 0xc9ba, 0xcad2, 0xcbec, 0xcd08, 0xce25, 0xcf43, 0xd063, 0xd185, 0xd2a8, 0xd3cd, 0xd4f3, 0xd61b,
	0xd745, 0xd870, 0xd99d, 0xdacc, 0xdbfc, 0xdd2e, 0xde61, 0xdf96, 0xe0cd, 0xe205, 0xe340, 0xe47b, 0xe5b9, 0xe6f8, 0xe839, 0xe97c,
	0xeac1, 0xec07, 0xed4f, 0xee99, 0xefe5, 0xf132, 0xf281, 0xf3d3, 0xf525, 0xf67a, 0xf7d1, 0xf929, 0xfa84, 0xfbe0, 0xfd3e, 0xfe9e,
};

static const uint16_t exp1_table[128] = {
	0x8000, 0x8001, 0x8003, 0x8004, 0x8006, 0x8007, 0x8008, 0x800a, 0x800b, 0x800c, 0x800e, 0x800f, 0x8011, 0x8012, 0x8013, 0x8015,
	0x8016, 0x8018, 0x8019, 0x801a, 0x801c, 0x801d, 0x801f, 0x8020, 0x8021, 0x8023, 0x8024, 0x8025, 0x8027, 0x8028, 0x802a, 0x802b,
	0x802c, 0x802e, 0x802f, 0x8031, 0x8032, 0x8033, 0x8035, 0x8036, 0x8037, 0x8039, 0x803a, 0x803c, 0x803d, 0x803e, 0x8040, 0x8041,
	0x8043, 0x8044, 0x8045, 0x8047, 0x8048, 0x804a, 0x804b, 0x804c, 0x804e, 0x804f, 0x8051, 0x8052, 0x8053, 0x8055, 0x8056, 0x8057,
	0x8059, 0x805a, 0x805c, 0x805d, 0x805e, 0x8060, 0x8061, 0x8063, 0x8064, 0x8065, 0x8067, 0x8068, 0x806a, 0x806b, 0x806c, 0x806e,
	0x806f, 0x8070, 0x8072, 0x8073, 0x8075, 0x8076, 0x8077, 0x8079, 0x807a, 0x807c, 0x807d, 0x807e, 0x8080, 0x8081, 0x8083, 0x8084,
	0x8085, 0x8087, 0x8088, 0x808a, 0x808b, 0x808c, 0x808e, 0x808f, 0x8090, 0x8092, 0x8093, 0x8095, 0x8096, 0x8097, 0x8099, 0x809a,
	0x809c, 0x809d, 0x809e, 0x80a0, 0x80a1, 0x80a3, 0x80a4, 0x80a5, 0x80a7, 0x80a8, 0x80aa, 0x80ab, 0x80ac, 0x80ae, 0x80af, 0x80b1,
};

q30 pow2(q31 x) {
	uint32_t n = x >> 17;
	uint32_t y0 = exp0_table[(n >> 7) & 127];
	uint32_t y1 = exp1_table[n & 127];
	return (int32_t) (y0 * y1);
}

// fast float version of pow2 for test purposes.
float pow2_fast(float x) {
	x = clamp(x, 0.f, 1.f);
	return q30_to_float(pow2(float_to_q31(x)));
}

// slow/accurate version of pow2 using math.h functions
float pow2_slow(float x) {
	x = clamp(x, 0.f, 1.f);
	return powf(2.f, x);
}

//-----------------------------------------------------------------------------
// midi to frequency conversion

// k0 = log2f(FrequencyScale) + log2f(440) - (69/12)
// k1 = 1/12
#define K0 (326830450U)		// q24
#define K1 (1398101U)		// q24

// return the 32-bit phase step for the notes frequency
uint32_t m2f(q24 note) {
	q24 x = K0 + mul_q24(note, K1);
	q30 s = pow2(q24_frac(x) << 7);
	return (uint32_t) (s >> (30 - q24_int(x)));
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
