#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

static bool print_int(int data) {
	if (data < 0) {
		if (putchar('-') == EOF) {
			return false;
		}
		data = -data;
	}

	if (data / 10) {
		print_int(data / 10);
	}

	if (putchar((data % 10) + 0x30) == EOF) {
		return false;
	}

	return true;
}

static bool print_hex(int data) {
	char hex_num[INT_HEX_NUM_DIGITS];
	int i, j, k, temp;
	j = 1;
	k = temp = 0;

	for (i = 0; i < INT_HEX_NUM_DIGITS; i++) {
		temp = data % 16;

		if (temp < 10) {
			temp += 48;

		} else {
			temp = temp + 55;
		}

		hex_num[j++] = temp;
		data = data / 16;
	}

	printf("0x");
	for (k = j - 1; k > 0; k--) {
		putchar(hex_num[k]);
	}
}

int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;

		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;

		} else if (*format == 'd' || *format == 'i') {
			format++;
			int i = va_arg(parameters, int);
			if (!maxrem) {
				return -1;
			}
			if (!print_int(i)) {
				return -1;
			}

			written++;

		} else if (*format == 'x') {
			format++;
			int i = va_arg(parameters, int);
			if (!maxrem) {
				return -1;
			}
			if (!print_hex(i)) {
				return -1;
			}

			written++;
			
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}
