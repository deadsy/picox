//-----------------------------------------------------------------------------
/*

Fixed Point Q-Format Code

*/
//-----------------------------------------------------------------------------

#ifndef QFMT_H
#define QFMT_H

//-----------------------------------------------------------------------------

#include <inttypes.h>

//-----------------------------------------------------------------------------

typedef int32_t q31;		// Q1.31
typedef int32_t q30;		// Q2.30
typedef int32_t q24;		// Q8.24
typedef int32_t q17;		// Q15.17
typedef int16_t q15;		// Q1.15

//-----------------------------------------------------------------------------
// Q1.31: -1 .. 0.999

static inline float q31_to_float(q31 x) {
	return (float)x *(1.f / (float)(1UL << 31));
}

static inline q31 float_to_q31(float x) {
	return (q24) (x * (float)(1UL << 31));
}

static inline q15 q31_to_q15(q31 x) {
	return (int16_t) (x >> 16);
}

//-----------------------------------------------------------------------------
// Q2.30: -2 .. 1.999

static inline float q30_to_float(q30 x) {
	return (float)x *(1.f / (float)(1UL << 30));
}

//-----------------------------------------------------------------------------
// Q8.24: -128 .. 127.999

static inline float q24_to_float(q24 x) {
	return (float)x *(1.f / (float)(1UL << 24));
}

static inline q24 float_to_q24(float x) {
	return (q24) (x * (float)(1UL << 24));
}

//-----------------------------------------------------------------------------
// Q15.17: -16384 .. 16383.999

static inline float q17_to_float(q17 x) {
	return (float)x *(1.f / (float)(1UL << 17));
}

//-----------------------------------------------------------------------------
// Q1.15: -1 .. 0.999

static inline float q15_to_float(q15 x) {
	return (float)x *(1.f / (float)(1UL << 15));
}

//-----------------------------------------------------------------------------

#endif				// QFMT_H

//-----------------------------------------------------------------------------