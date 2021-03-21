//-----------------------------------------------------------------------------
/*

MIDI Functions

*/
//-----------------------------------------------------------------------------

#include <math.h>

#include "ggm.h"

//-----------------------------------------------------------------------------

// k0 = log2f(FrequencyScale) + log2f(440) - (69/12)
#define K0 (q24)(326830450U)

// k1 = 1/12
#define K1 (q24)(1398101U)

// k2 = -3
#define K2 (q24)(-3 * (1U << 24))

//-----------------------------------------------------------------------------
// midi to frequency conversion

// return the 32-bit phase step for the midi note frequency
uint32_t m2f(q24 note) {
	q24 x = K0 + mul_q24(note, K1);
	q30 s = pow2(q24_frac(x) << 7);
	return (uint32_t) (s >> (30 - q24_int(x)));
}

// fast float version of m2f for test purposes.
float m2f_fast(float note) {
	note = clampf(note, 0.f, 127.f);
	uint32_t dx = m2f(float_to_q24(note));
	return (float)dx *(1.f / FrequencyScale);
}

// slow/accurate float version of m2f using math.h functions (~19.8 usecs/call)
float m2f_slow(float note) {
	note = clampf(note, 0.f, 127.f);
	return 440.f * powf(2.f, (note - 69.f) * (1.f / 12.f));
}

//-----------------------------------------------------------------------------
// midi to cv conversion

q24 m2cv(q24 note) {
	// 1 v/octave, 36 = 0V
	// cv = (note/12) - 3
	return K2 + mul_q24(note, K1);
}

//-----------------------------------------------------------------------------
