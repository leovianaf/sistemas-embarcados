# 🛠️ Laboratórios de Sistemas Embarcados - UFRPE

Repositório dedicado ao armazenamento e organização das atividades práticas e laboratórios desenvolvidos para a disciplina de Sistemas Embarcados na Universidade Federal Rural de Pernambuco (UFRPE).

## 🎯 Objetivo do Repositório

O principal objetivo deste repositório é documentar a evolução dos projetos práticos, consolidando o aprendizado em programação de microcontroladores (especificamente o ATmega328P presente no Arduino Nano).

As implementações focam em técnicas avançadas de desenvolvimento embarcado, priorizando a abordagem **Bare-Metal** (manipulação direta de registradores de hardware, como `DDR`, `PORT` e `PIN`) e o uso eficiente de interrupções, evitando o uso de funções nativas ocultas da IDE do Arduino (como `digitalWrite`, `pinMode` ou `delay`).

---

## 📂 Estrutura e Organização das Atividades

O repositório está organizado de forma modular. Cada atividade prática possui sua própria pasta contendo o código-fonte principal (`.ino`) e os arquivos de suporte local da biblioteca de comunicação UART, quando aplicável.

```text
sistemas-embarcados/
├── debounce/
├── display1/
├── display2/
├── display3/
├── interrupcao/
├── teclado/
├── usart_imprime_hexa/
├── usart_version1/
├── usart_version2/
├── usart_version3/
└── README.md
```

### 🏛️ Biblioteca Compartilhada Localmente

Para os projetos que dependem de comunicação serial, as funções foram encapsuladas em uma biblioteca Bare-Metal integrada diretamente à pasta do respectivo laboratório:

- `bare_serial.h`: Declarações das funções de controle e configuração da UART.
- `bare_serial.c`: Implementação Bare-Metal da inicialização, escrita e tratamento de dados da UART.

### 📝 Atividades Inclusas

| Pasta / Projeto      | Descrição Técnica                                                                                                                                                                               | Conceitos Aplicados                                                                             |
| -------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------- |
| `debounce`           | Leitura estável de entrada digital utilizando debounce por amostragem baseada em tempo (base de 1ms).                                                                                           | GPIO, Polling Temporizado, Filtro de Ruído Mecânico                                             |
| `display1`           | Contador hexadecimal de 0 a F exibido em um display de 7 segmentos de cátodo comum manipulando um porto inteiro de uma vez.                                                                     | Mapeamento de Hardware, Porto D Completo, Tabela de Conversão (`hexTable`)                      |
| `display2`           | Fatiamento de um byte em dois Nibbles (Alta e Baixa) para exibição simultânea em dois displays de 7 segmentos separados.                                                                        | Divisão de Bits (Shift/Máscara), Manipulação dos Portos B, C e D                                |
| `display3`           | Exibição de valores decimais de 0 a 99 com sistema de tratamento de estouro (exibindo "F.F." em caso de valores maiores).                                                                       | Lógica de Overflow, Ponto Decimal Dinâmico, Divisão/Resto Inteiro                               |
| `interrupcao`        | Medição precisa do tempo decorrido entre transições de subida (RISING) e descida (FALLING) de sinal.                                                                                            | Interrupções Externas (`INT0`/`INT1`), Salvamento de Estado de Interrupções (`SREG`)            |
| `teclado`            | Varredura de teclado matricial 4x4 e envio do caractere pressionado via comunicação serial.                                                                                                     | GPIO (Entrada Pull-up/Saída), Máscara de Bits, Comunicação UART                                 |
| `usart_imprime_hexa` | Utilitário para leitura e exibição em formato numérico do estado físico atual dos registradores de controle e configuração da UART0.                                                            | Diagnóstico Bare-Metal, Leitura de Registradores (`UCSR0A`, `UCSR0B`, etc.)                     |
| `usart_version1`     | Receptor de caracteres via serial que ecoa instantaneamente os dados de volta ao terminal através de interrupção.                                                                               | USART0, Interrupção de Recepção (`USART_RX_vect`)                                               |
| `usart_version2`     | Evolução da comunicação UART implementando buffers circulares independentes para TX e RX, com lógica de retransmissão sob gatilho (`\r`) e sobrescrita de dados antigos.                        | Estrutura de Dados (Ponteiros Head/Tail), Operações de Resto (`%`), Buffer Circular             |
| `usart_version3`     | Sistema completo com interpretador de strings que processa comandos em tempo real para exibir buffers, strings invertidas, posições de ponteiros e reconfigurar parâmetros físicos de hardware. | Manipulação de Strings (`strcmp`/`strncmp`), Alteração dinâmica de Baud-rate e tamanho de Frame |

---

## 🛠️ Tecnologias e Ferramentas Utilizadas

- **Microcontrolador:** ATmega328P (Arduino Nano)
- **Linguagem de Programação:** C / C++ (Dialeto AVR-GCC)
- **Ambiente de Desenvolvimento:** Arduino IDE
- **Comunicação:** Terminal Serial UART (Protocolo RS-232)

---

## 🚀 Como Compilar e Rodar os Projetos

Como o repositório utiliza a abordagem modular por abas para as bibliotecas locais, executar qualquer laboratório é extremamente simples:

1. Clone este repositório no seu computador:

```bash
git clone [https://github.com/leovianaf/sistemas-embarcados](https://github.com/leovianaf/sistemas-embarcados)

```

2. Abra a pasta do laboratório desejado (ex: `usart_version3`) utilizando a [**IDE do Arduino**](https://www.arduino.cc/en/software/).
3. Certifique-se de que os arquivos `bare_serial.h` e `bare_serial.c` estão presentes na mesma pasta que o arquivo `.ino`.
4. Conecte sua placa Arduino ao computador via cabo USB.
5. Faça o Upload do código.
6. _(Para projetos USART)_ Abra o Monitor Serial na velocidade especificada no código (normalmente `9600` bps) para interagir com o programa.

_Nota: Para os projetos envolvendo displays de 7 segmentos ou botões, consulte o mapeamento de pinos detalhado nos comentários de inicialização de cada arquivo para realizar a montagem física correta._
