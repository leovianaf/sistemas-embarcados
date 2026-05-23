/*
 * =========================================================================
 * INTERPRETADOR DE COMANDOS USART (VERSÃO 3)
 * =========================================================================
 * Mapeamento: Comunicação Serial UART0 via biblioteca "bare_serial.h".
 * * COMO UTILIZAR OS COMANDOS NO TERMINAL (Sempre termine com ENTER / '\r'):
 * * 1. ENVIAR DADOS: Digite qualquer texto comum para armazená-lo no buffer.
 * Exemplo: "UFRPE Engenharia" -> Retorna: "Texto recebido e armazenado..."
 * * 2. COMANDOS DISPONÍVEIS:
 * i.   "print"    -> Imprime o último texto que foi armazenado no buffer.
 * ii.  "reverse"  -> Imprime o texto armazenado de trás para frente.
 * iii. "clear"    -> Limpa os buffers e o histórico de texto salvo.
 * iv.  "baud X"   -> Altera o Baud-rate. Ex: "baud 115200" ou "baud 9600".
 * (Lembre-se de mudar a velocidade no monitor serial também!)
 * v.   "bits X"   -> Altera o tamanho do frame. Ex: "bits 8", "bits 7", etc.
 * vi.  "pointers" -> Mostra as posições atuais de Head/Tail dos buffers circulares.
 * =========================================================================
 */

#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "bare_serial.h"

#define BUF_SIZE 64

volatile char rx_buf[BUF_SIZE];
volatile uint8_t rx_head = 0, rx_tail = 0;
volatile char tx_buf[BUF_SIZE];
volatile uint8_t tx_head = 0, tx_tail = 0;
volatile bool cmd_ready = false;

static char ultimo_conteudo[BUF_SIZE] = "Nenhum dado salvo";

void enviar_caractere(char c) {
  uint8_t next_head = (tx_head + 1) % BUF_SIZE;
  while (next_head == tx_tail);
  tx_buf[tx_head] = c;
  tx_head = next_head;
  UCSR0B |= (1 << UDRIE0);
}

void enviar_string(const char* str) {
  while (*str) enviar_caractere(*str++);
}

void setup() {
  cli();

  serial_begin(9600);
  UCSR0B |= (1 << RXCIE0);

  sei();
  enviar_string("V3: Interpretador de Comandos Pronto\r\n");
}

void loop() {
  if (cmd_ready) {
    cli();
    char cmd[BUF_SIZE];
    uint8_t i = 0;

    while (rx_tail != rx_head && i < BUF_SIZE - 1) {
      cmd[i++] = rx_buf[rx_tail];
      rx_tail = (rx_tail + 1) % BUF_SIZE;
    }
    cmd[i] = '\0';
    cmd_ready = false;
    sei();

    if (i > 0) {
      if (strcmp(cmd, "print") == 0) {
        enviar_string("Conteudo atual do buffer salvo: ");
        enviar_string(ultimo_conteudo);
        enviar_string("\r\n");
      }
      else if (strcmp(cmd, "reverse") == 0) {
        enviar_string("Inverso do buffer salvo: ");
        int len = strlen(ultimo_conteudo);
        for (int j = len - 1; j >= 0; j--) {
          enviar_caractere(ultimo_conteudo[j]);
        }
        enviar_string("\r\n");
      }
      else if (strcmp(cmd, "clear") == 0) {
        cli();
        rx_head = 0;
        rx_tail = 0;
        sei();
        strcpy(ultimo_conteudo, "");
        enviar_string("Buffer de recepcao limpo.\r\n");
      }
      else if (strncmp(cmd, "baud ", 5) == 0) {
        long new_baud = atol(&cmd[5]);
        if (new_baud > 0) {
          uint16_t b_setting = (16000000UL / 16 / new_baud) - 1;
          cli();
          UBRR0H = (b_setting >> 8);
          UBRR0L = b_setting;
          sei();
          enviar_string("Baud-rate alterado com sucesso!\r\n");
        }
      }
      else if (strncmp(cmd, "bits ", 5) == 0) {
        int bits = atoi(&cmd[5]);
        cli();
        if (bits == 8) UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
        else if (bits == 7) UCSR0C = (1 << UCSZ01);
        else if (bits == 6) UCSR0C = (1 << UCSZ00);
        else if (bits == 5) UCSR0C = 0;
        sei();
        enviar_string("Numero de bits alterado!\r\n");
      }
      else if (strcmp(cmd, "pointers") == 0) {
        char msg[80];
        sprintf(msg, "RX Head: %d, Tail: %d | TX Head: %d, Tail: %d\r\n",
                rx_head, rx_tail, tx_head, tx_tail);
        enviar_string(msg);
      }
      else {
        strcpy(ultimo_conteudo, cmd);
        enviar_string("Texto recebido e armazenado no buffer.\r\n");
      }
    }
  }
}

ISR(USART_RX_vect) {
  char c = UDR0;
  if (c == '\r') {
    cmd_ready = true;
  } else if (c != '\n') {
    rx_buf[rx_head] = c;
    rx_head = (rx_head + 1) % BUF_SIZE;
    if (rx_head == rx_tail) rx_tail = (rx_tail + 1) % BUF_SIZE;
  }
}

ISR(USART_UDRE_vect) {
  if (tx_head != tx_tail) {
    UDR0 = tx_buf[tx_tail];
    tx_tail = (tx_tail + 1) % BUF_SIZE;
  } else {
    UCSR0B &= ~(1 << UDRIE0);
  }
}
