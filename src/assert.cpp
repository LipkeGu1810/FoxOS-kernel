#include <assert.h>
#include <interrupts/panic.h>
#include <driver/serial.h>
#include <stdio.h>

void __assert_fail(const char* __assertion, const char* __file, unsigned int __line) {
	char panic_buffer[1024];
	sprintf(panic_buffer, "Assertion failed: \"%s\" in file %s at line %d\n", __assertion, __file, __line);

	interrupts::Panic p = interrupts::Panic(panic_buffer);
	p.do_it(NULL);
}

void __assert_fail_nopanic(const char* __assertion, const char* __file, unsigned int __line) {
	char panic_buffer[1024];
	sprintf(panic_buffer, "Assertion failed: \"%s\" in file %s at line %d\n", __assertion, __file, __line);

	driver::global_serial_driver->puts(panic_buffer);
}