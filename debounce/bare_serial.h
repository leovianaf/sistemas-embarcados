#ifndef BARE_SERIAL_H
#define BARE_SERIAL_H

#define F_CPU 16000000UL

// Registradores UART0
#define UBRR0H (*(volatile unsigned char *)0xC5)
#define UBRR0L (*(volatile unsigned char *)0xC4)
#define UCSR0B (*(volatile unsigned char *)0xC1)
#define UCSR0C (*(volatile unsigned char *)0xC0)
#define UDR0   (*(volatile unsigned char *)0xC6)
#define UCSR0A (*(volatile unsigned char *)0xC0)

#define RXEN0  4
#define TXEN0  3
#define UCSZ01 2
#define UCSZ00 1
#define UDRE0  5

#ifdef __cplusplus
extern "C" {
#endif

void serial_begin(unsigned long baud);
void serial_write_char(char c);
void serial_print(const char *str);
void serial_println(const char *str);
void serial_print_ulong(unsigned long num);
void serial_print_hex(unsigned char num);
void delay_ms(unsigned long ms);

#ifdef __cplusplus
}
#endif

#endif