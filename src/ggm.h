//-----------------------------------------------------------------------------
/*

GooGooMuck Includes

*/
//-----------------------------------------------------------------------------

#ifndef GGM_H
#define GGM_H

//-----------------------------------------------------------------------------

#include "log.h"
#include "qfmt.h"
#include "const.h"

//-----------------------------------------------------------------------------
// math

// cosine
q15 cos_q15(uint32_t x);
float ggm_cosf(float x);

// midi to frequency
uint32_t m2f(q24 note);
float m2f_slow(float note);
float m2f_fast(float note);

static inline float clamp_hi(float x, float hi) {
	return (x > hi) ? hi : x;
}

static inline float clamp_lo(float x, float lo) {
	return (x < lo) ? lo : x;
}

static inline float clamp(float x, float lo, float hi) {
	return clamp_lo(clamp_hi(x, hi), lo);
}

// test functions
void rms_error(const char *name, float x0, float x1, float dx, float (*f0)(float), float (*f1)(float));
void benchmark_f0(const char *name, int16_t(*f0) (uint32_t), unsigned int n);
void benchmark_f1(const char *name, float (*f1)(float), unsigned int n);
void benchmark_f2(const char *name, uint32_t(*f2) (q24), unsigned int n);

//-----------------------------------------------------------------------------
// midi

float midi_to_frequency(float note);

//-----------------------------------------------------------------------------

#endif				// GGM_H

//-----------------------------------------------------------------------------
