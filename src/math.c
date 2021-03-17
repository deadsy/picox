//-----------------------------------------------------------------------------
/*

Math Functions for 32-bit float values.

*/
//-----------------------------------------------------------------------------

#include "picox.h"

//-----------------------------------------------------------------------------
// 32-bit float helper macros

/* A union which permits us to convert between a float and a 32 bit int.  */
typedef union {
	float value;
	uint32_t word;
} ieee_float_shape_type;

/* Get a 32 bit int from a float.  */
#define GET_FLOAT_WORD(i, d)		    \
	do {				    \
		ieee_float_shape_type gf_u; \
		gf_u.value = (d);	    \
		(i) = gf_u.word;	    \
	} while (0)

/* Set a float from a 32 bit int.  */
#define SET_FLOAT_WORD(d, i)		    \
	do {				    \
		ieee_float_shape_type sf_u; \
		sf_u.word = (i);	    \
		(d) = sf_u.value;	    \
	} while (0)

#define FLT_UWORD_IS_NAN(x) ((x) > 0x7f800000L)
#define FLT_UWORD_IS_INFINITE(x) ((x) == 0x7f800000L)

//-----------------------------------------------------------------------------
// 32-bit float truncation.

float truncf(float x) {
	int32_t signbit, w, exponent_less_127;

	GET_FLOAT_WORD(w, x);

	/* Extract sign bit. */
	signbit = w & 0x80000000;

	/* Extract exponent field. */
	exponent_less_127 = ((w & 0x7f800000) >> 23) - 127;

	if (exponent_less_127 < 23) {
		if (exponent_less_127 < 0) {
			/* -1 < x < 1, so result is +0 or -0. */
			SET_FLOAT_WORD(x, signbit);
		} else {
			SET_FLOAT_WORD(x, signbit | (w & ~(0x007fffff >> exponent_less_127)));
		}
	} else {
		if (exponent_less_127 == 255) {
			/* x is NaN or infinite. */
			return x + x;
		}

		/* All bits in the fraction field are relevant. */
	}
	return x;
}

//-----------------------------------------------------------------------------
// 32-bit float absolute values

float fabsf(float x) {
	union {
		unsigned int ui;
		float f;
	} val;
	val.f = x;
	val.ui = (val.ui << 1) >> 1;
	return val.f;
}

//-----------------------------------------------------------------------------
// is not-a-number

int isnanf(float x) {
	int32_t ix;
	GET_FLOAT_WORD(ix, x);
	ix &= 0x7fffffff;
	return FLT_UWORD_IS_NAN(ix);
}

//-----------------------------------------------------------------------------
// is infinite

int isinff(float x) {
	int32_t ix;
	GET_FLOAT_WORD(ix, x);
	ix &= 0x7fffffff;
	return FLT_UWORD_IS_INFINITE(ix);
}

//-----------------------------------------------------------------------------
// 32-bit float trigonometry functions
// These are LUT based and are less accurate but faster than typical libm functions.

// convert x radians to the range 0..Tau radians
static inline float normalize_radians(float x) {
	while (x < 0) {
		x += Tau;
	}
	while (x >= Tau) {
		x -= Tau;
	}
	return x;
}

float cosf(float x) {
	x = normalize_radians(x);
	uint32_t xi = (uint32_t) (x * PhaseScale);
	return q31_to_float(cos_lut(xi));
}

float sinf(float x) {
	x = normalize_radians(x);
	uint32_t xi = QuarterCycle - (uint32_t) (x * PhaseScale);
	return q31_to_float(cos_lut(xi));
}

//-----------------------------------------------------------------------------

float pow2(float x) {
	return q30_to_float(pow2_lut(float_to_q31(x)));
}

//-----------------------------------------------------------------------------
// 32-bit float power function
// powe returns powf(e, x)

#if 0
#define LOG_E2 (1.4426950408889634f)	/* 1.0 / math.log(2.0) */

float powe(float x) {
	return pow2(LOG_E2 * x);
}
#endif

//-----------------------------------------------------------------------------
