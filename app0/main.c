//-----------------------------------------------------------------------------

#include <stdio.h>
#include "pico/stdlib.h"

//-----------------------------------------------------------------------------

#define UART uart0

//-----------------------------------------------------------------------------

static void con_putc(char c) {
	uart_putc(UART, c);
}

static void con_puts(const char *s) {
	uart_puts(UART, s);
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

static char *itoa(int x, char *s) {
	unsigned int ux = (x >= 0) ? x : -x;
	int i = 0;
	do {
		s[i++] = '0' + (ux % 10);
		ux /= 10;
	} while (ux != 0);
	if (x < 0) {
		s[i++] = '-';
	}
	s[i--] = 0;
	int j = 0;
	while (j < i) {
		char tmp = s[j];
		s[j++] = s[i];
		s[i--] = tmp;
	}
	return s;
}

static void put_dec(int x) {
	char tmp[32];
	con_puts(itoa(x, tmp));
}

//-----------------------------------------------------------------------------

#define BYTES_PER_LINE 16

static void mem_display8(void *ptr, unsigned n) {
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

static void picoMon(void) {
	puts("pico mon 1.0");

	put_dec(987654321);
	con_puts("\r\n");

	put_dec(-1234);
	con_puts("\r\n");

	mem_display8((void *)0, 1 << 10);
	con_puts("done\r\n");
}

//-----------------------------------------------------------------------------

int main(void) {

	const uint LED_PIN = 25;
	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);

	uart_init(uart0, 115200);
	gpio_set_function(0, GPIO_FUNC_UART);
	gpio_set_function(1, GPIO_FUNC_UART);

	picoMon();

	float x = 1.234f;
	char tmp[20];
	sprintf(tmp, "%f\r\n", x);
	con_puts(tmp);

	while (1) {
		gpio_put(LED_PIN, 1);
		sleep_ms(250);
		gpio_put(LED_PIN, 0);
		sleep_ms(250);
	}

	return 0;
}

//-----------------------------------------------------------------------------
