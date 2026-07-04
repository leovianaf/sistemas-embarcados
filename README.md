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
├── projeto/
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
| `projeto`            | Rastreador solar de um eixo que compara dois sensores LDR, movimenta um servomotor e apresenta as medições em um display OLED.                                                                  | ADC e PWM Bare-Metal, Timer1, controle por zona morta e display OLED I2C                        |
| `teclado`            | Varredura de teclado matricial 4x4 e envio do caractere pressionado via comunicação serial.                                                                                                     | GPIO (Entrada Pull-up/Saída), Máscara de Bits, Comunicação UART                                 |
| `usart_imprime_hexa` | Utilitário para leitura e exibição em formato numérico do estado físico atual dos registradores de controle e configuração da UART0.                                                            | Diagnóstico Bare-Metal, Leitura de Registradores (`UCSR0A`, `UCSR0B`, etc.)                     |
| `usart_version1`     | Receptor de caracteres via serial que ecoa instantaneamente os dados de volta ao terminal através de interrupção.                                                                               | USART0, Interrupção de Recepção (`USART_RX_vect`)                                               |
| `usart_version2`     | Evolução da comunicação UART implementando buffers circulares independentes para TX e RX, com lógica de retransmissão sob gatilho (`\r`) e sobrescrita de dados antigos.                        | Estrutura de Dados (Ponteiros Head/Tail), Operações de Resto (`%`), Buffer Circular             |
| `usart_version3`     | Sistema completo com interpretador de strings que processa comandos em tempo real para exibir buffers, strings invertidas, posições de ponteiros e reconfigurar parâmetros físicos de hardware. | Manipulação de Strings (`strcmp`/`strncmp`), Alteração dinâmica de Baud-rate e tamanho de Frame |

## ☀️ Projeto Final — Rastreador Solar de Um Eixo

O diretório [`projeto/`](projeto/) contém um rastreador solar vertical baseado no ATmega328P. O sistema utiliza dois sensores LDR para identificar a direção com maior incidência de luz, movimenta um servomotor em pequenos passos e exibe os dados de operação em um display OLED SH1106 de 128 × 64 pixels.

O projeto foi adaptado de Henukh et al., _Solar Tracker Design Based on Arduino Nano to Improve Solar Energy Efficiency_, Technium, volume 16, 2023. Em relação ao trabalho original, esta implementação utiliza dois LDRs e somente um eixo de movimentação.

### Funcionamento

1. O ADC lê os LDRs superior e inferior pelos canais ADC0 e ADC1.
2. O programa calcula o erro vertical pela diferença `LDR superior - LDR inferior`.
3. Quando o módulo do erro ultrapassa `ERROR_THRESHOLD`, o servo avança um passo na direção do sensor correspondente.
4. Enquanto o erro permanece dentro da zona morta, a posição é mantida. Isso reduz oscilações causadas por pequenas variações nas leituras.
5. A cada ciclo, o display apresenta as duas leituras, o erro calculado e o valor de PWM aplicado ao servo.

O ciclo de controle é executado a cada 50 ms. O ADC e o Timer1 são configurados diretamente pelos registradores do ATmega328P. O display utiliza bibliotecas do ecossistema Arduino.

### Mapeamento de hardware

| Componente         | Pino Arduino Nano | Recurso do ATmega328P |
| ------------------ | ----------------- | --------------------- |
| Sinal PWM do servo | D9                | PB1/OC1A              |
| LDR superior       | A0                | PC0/ADC0              |
| LDR inferior       | A1                | PC1/ADC1              |
| SDA do display     | A4                | PC4/SDA               |
| SCL do display     | A5                | PC5/SCL               |

Todos os componentes devem compartilhar o mesmo GND. Caso o servo seja alimentado por uma fonte externa, o GND dessa fonte também deve ser ligado ao GND do Arduino. A fonte deve suportar a corrente exigida pelo servo; não é recomendável alimentar servos de maior corrente diretamente pelo pino de 5 V da placa.

### Organização do código

| Arquivo                             | Responsabilidade                                       |
| ----------------------------------- | ------------------------------------------------------ |
| `projeto/projeto.ino`               | Inicialização, laço de controle e decisão de movimento |
| `projeto/config.h`                  | Canais ADC, limiar de erro e limites do servo          |
| `projeto/ldr.cpp` e `ldr.h`         | Configuração e leitura Bare-Metal do ADC               |
| `projeto/servo.cpp` e `servo.h`     | Geração do PWM de 50 Hz pelo Timer1                    |
| `projeto/display.cpp` e `display.h` | Inicialização e atualização do OLED por I2C            |

### Dependências do display

Instale as bibliotecas abaixo pelo **Gerenciador de Bibliotecas** da Arduino IDE:

- **Adafruit SH110X** — versão testada: `2.1.14`;
- **Adafruit GFX Library** — versão testada: `1.12.6`;
- **Adafruit BusIO** — versão testada: `1.17.4`.

A biblioteca `Wire` já faz parte do Arduino Core. O display usado pelo projeto possui controlador SH1106 e o código instancia `Adafruit_SH1106G`.

Para instalar, abra **Ferramentas → Gerenciar Bibliotecas**, pesquise por **Adafruit SH110X** e selecione **Instalar**. A IDE normalmente oferece a instalação automática das dependências GFX e BusIO; confirme essa opção.

### Compilação do projeto

1. Instale as dependências do display.
2. Abra `projeto/projeto.ino` na Arduino IDE.
3. Selecione a placa **Arduino Nano** e o processador correspondente ao bootloader da sua placa.
4. Conecte os componentes conforme o mapeamento acima.
5. Compile e envie o programa para a placa.

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
