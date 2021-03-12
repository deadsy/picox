//-----------------------------------------------------------------------------
/*

Look Up Tables

*/
//-----------------------------------------------------------------------------

#include "lut.h"

//-----------------------------------------------------------------------------
// LUT based cosine function - generated by ./tools/lut.py

#define COS_LUT_BITS (7U)
#define COS_LUT_SIZE (1U << COS_LUT_BITS)

static const int32_t COS_LUT_data[COS_LUT_SIZE << 1] = {
	2147483647, -2586738, 2144896909, -7753982, 2137142927, -12902547, 2124240380, -18020029,
	2106220351, -23094097, 2083126254, -28112531, 2055013723, -33063240, 2021950483, -37934295,
	1984016188, -42713964, 1941302224, -47390730, 1893911494, -51953330, 1841958164, -56390768,
	1785567396, -60692357, 1724875039, -64847731, 1660027308, -68846883, 1591180425, -72680176,
	1518500249, -76338375, 1442161874, -79812670, 1362349204, -83094689, 1279254515, -86176525,
	1193077990, -89050754, 1104027236, -91710452, 1012316784, -94149213, 918167571, -96361158,
	821806413, -98340962, 723465451, -100083854, 623381597, -101585634, 521795963, -102842687,
	418953276, -103851982, 315101294, -104611088, 210490206, -105118178, 105372028, -105372028,
	0, -105372028, -105372028, -105118178, -210490206, -104611088, -315101294, -103851982,
	-418953276, -102842687, -521795963, -101585634, -623381597, -100083854, -723465451, -98340962,
	-821806413, -96361158, -918167571, -94149213, -1012316784, -91710452, -1104027236, -89050754,
	-1193077990, -86176525, -1279254515, -83094689, -1362349204, -79812670, -1442161874, -76338375,
	-1518500249, -72680176, -1591180425, -68846883, -1660027308, -64847731, -1724875039, -60692357,
	-1785567396, -56390768, -1841958164, -51953330, -1893911494, -47390730, -1941302224, -42713964,
	-1984016188, -37934295, -2021950483, -33063240, -2055013723, -28112531, -2083126254, -23094097,
	-2106220351, -18020029, -2124240380, -12902547, -2137142927, -7753982, -2144896909, -2586739,
	-2147483648, 2586739, -2144896909, 7753982, -2137142927, 12902547, -2124240380, 18020029,
	-2106220351, 23094097, -2083126254, 28112531, -2055013723, 33063240, -2021950483, 37934295,
	-1984016188, 42713964, -1941302224, 47390730, -1893911494, 51953330, -1841958164, 56390768,
	-1785567396, 60692357, -1724875039, 64847731, -1660027308, 68846883, -1591180425, 72680176,
	-1518500249, 76338375, -1442161874, 79812670, -1362349204, 83094689, -1279254515, 86176525,
	-1193077990, 89050754, -1104027236, 91710452, -1012316784, 94149213, -918167571, 96361158,
	-821806413, 98340962, -723465451, 100083854, -623381597, 101585634, -521795963, 102842687,
	-418953276, 103851982, -315101294, 104611088, -210490206, 105118178, -105372028, 105372028,
	0, 105372028, 105372028, 105118178, 210490206, 104611088, 315101294, 103851982,
	418953276, 102842687, 521795963, 101585634, 623381597, 100083854, 723465451, 98340962,
	821806413, 96361158, 918167571, 94149213, 1012316784, 91710452, 1104027236, 89050754,
	1193077990, 86176525, 1279254515, 83094689, 1362349204, 79812670, 1442161874, 76338375,
	1518500249, 72680176, 1591180425, 68846883, 1660027308, 64847731, 1724875039, 60692357,
	1785567396, 56390768, 1841958164, 51953330, 1893911494, 47390730, 1941302224, 42713964,
	1984016188, 37934295, 2021950483, 33063240, 2055013723, 28112531, 2083126254, 23094097,
	2106220351, 18020029, 2124240380, 12902547, 2137142927, 7753982, 2144896909, 2586738,
};

int32_t cos_lookup(uint32_t x) {
	return lerp(COS_LUT_data, x, 32 - COS_LUT_BITS, 21);
}

//-----------------------------------------------------------------------------
