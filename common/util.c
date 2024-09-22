//-----------------------------------------------------------------------------
/*

Utilities

*/
//-----------------------------------------------------------------------------

#include <string.h>

#include "pico/stdio.h"
#include "util.h"

//-----------------------------------------------------------------------------

static void con_putc(char c) {
	stdio_put_string(&c, 1, false, false);
}

static void con_puts(const char *s) {
	stdio_put_string(s, strlen(s), false, false);
}

//-----------------------------------------------------------------------------

static void put_nybble(int x) {
	x &= 15;
	con_putc((x <= 9) ? ('0' + x) : ('a' + x - 10));
}

static void put_hex8(uint8_t x) {
	put_nybble(x >> 4);
	put_nybble(x);
}

static void put_hex16(uint16_t x) {
	put_hex8(x >> 8);
	put_hex8(x);
}

static void put_hex32(uint32_t x) {
	put_hex16(x >> 16);
	put_hex16(x);
}

//-----------------------------------------------------------------------------

#define BYTES_PER_LINE 16

void mem_display8(void *ptr, unsigned n) {
	char ascii[BYTES_PER_LINE + 1];
	int ofs = 0;

	n = (n + BYTES_PER_LINE - 1) & ~(BYTES_PER_LINE - 1);
	ascii[BYTES_PER_LINE] = 0;

	while (n > 0) {
		put_hex32((uint32_t) ptr + ofs);
		con_puts(": ");
		for (int i = 0; i < BYTES_PER_LINE; i++) {
			char c = ((uint8_t *) ptr)[ofs + i];
			put_hex8(c);
			con_putc(' ');
			// ascii string
			if ((c >= 0x20) && (c <= 0x7e)) {
				ascii[i] = c;
			} else {
				ascii[i] = '.';
			}
			n--;
		}
		con_puts(ascii);
		con_puts("\r\n");
		ofs += BYTES_PER_LINE;
	}
}

//-----------------------------------------------------------------------------

float clamp(float x, float lo, float hi) {
	if (x < lo) {
		return lo;
	}
	if (x > hi) {
		return hi;
	}
	return x;
}

//-----------------------------------------------------------------------------
