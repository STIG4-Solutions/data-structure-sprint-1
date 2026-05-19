# ChargGrid — Simulador de Sessão de Recarga EV

**Sprint 1 — Estruturas de Dados em C**
FIAP · Ciência da Computação

---

## Equipe Stig4 Solutions

| Nome                  | RM     |
|-----------------------|--------|
| Gabriel Fagundes      | 569074 |
| Gabriel Freitas       | 572943 |
| Giovanni Merlotti     | 573721 |
| Glauco Kelly          | 572840 |
| Sergio Augusto Amaral | 570184 |
| Thiago Renatino       | 569073 |

---

## Sobre o projeto

Programa em C que simula o funcionamento de um terminal de recarga para veículos elétricos, rodando diretamente no terminal. O operador informa os dados da sessão (nome, tipo de plano, horário e duração), e o programa calcula automaticamente a energia consumida, aplica a tarifa correta e exibe um relatório formatado ao final.

O equipamento simulado é o **GW7K-HCA-20** (linha HCA G2) — carregador CA monofásico de **7,4 kW**.

---

## Como compilar e executar

**Pré-requisito:** GCC instalado. No Windows, use [MinGW](https://www.mingw-w64.org/) ou WSL.

```bash
# Compilar
gcc simulador_recarga.c -o simulador_recarga.exe

# Executar
.\simulador_recarga.exe          # Windows
./simulador_recarga              # Linux/macOS

# Compilar e executar de uma vez (Windows)
gcc simulador_recarga.c -o simulador_recarga.exe && .\simulador_recarga.exe
```

---

## Fluxo da sessão

```
STANDBY (LED verde fixo)
    │
    ▼
Coleta de dados
    ├── Nome do usuário
    ├── Tipo de plano (1 = Comum / 2 = Fidelidade)
    ├── Horário de início (HH:MM)
    └── Duração desejada (1–480 minutos)
    │
    ▼
CARREGANDO (LED azul fixo)
    ├── Loop for: acumula energia minuto a minuto
    └── Barra de progresso animada no terminal
    │
    ▼
CONCLUÍDO (LED verde piscando)
    │
    ▼
Relatório da sessão
```

O programa permite múltiplas sessões consecutivas — ao final de cada uma, o operador pode iniciar uma nova ou encerrar.

---

## Tarifação

A tarifa varia conforme o tipo de plano e o horário de início da recarga:

| Plano      | Fora de ponta | Horário de ponta (18h–21h) |
|------------|---------------|----------------------------|
| Comum      | R$ 0,85/kWh   | R$ 1,40/kWh                |
| Fidelidade | R$ 0,70/kWh   | R$ 0,70/kWh (fixo)         |

- Usuários **Fidelidade** pagam tarifa fixa em qualquer horário.
- Usuários **Comuns** pagam tarifa majorada se a sessão iniciar entre 18h e 21h.

**Potência do equipamento:** 7,4 kW (fixo — GW7K-HCA-20)

**Fórmulas:**

```
Energia (kWh) = (7,4 / 60,0) × duração_em_minutos
Custo   (R$)  = energia_kWh × tarifa_aplicada
```

---

## Estruturas de C utilizadas

### Condicionais (`if/else` e `switch`)

- `if/else` em `calcular_tarifa()` — decide a tarifa conforme plano e horário
- `if/else` em `coletar_dados()` — valida o formato HH:MM do horário de entrada
- `switch` em `exibir_status_led()` — exibe o estado do LED conforme o status da sessão
- `switch` em `main()` — exibe a mensagem de encerramento conforme o resultado da sessão

### Repetição (`for` e `do-while`)

- `do-while` em `ler_inteiro()` — repete a leitura até o usuário digitar um valor válido
- `do-while` em `coletar_dados()` — repete a leitura do horário até o formato ser válido
- `do-while` em `main()` — mantém o programa ativo para múltiplas sessões consecutivas
- `for` em `simular_recarga()` — itera minuto a minuto acumulando energia consumida
- `for` interno em `simular_recarga()` — desenha a barra de progresso com 20 passos

### Struct e Enum

- `SessaoRecarga` — centraliza todos os dados de uma sessão (nome, plano, horário, duração, energia, tarifa, custo, status)
- `StatusLED` — enum com os quatro estados do equipamento: `STANDBY`, `CARREGANDO`, `CONCLUIDO`, `FALHA`

### Funções

| Função | Responsabilidade |
| ------ | ---------------- |
| `exibir_cabecalho()` | Exibe o cabeçalho do sistema |
| `exibir_status_led()` | Exibe o estado atual do equipamento (LED) |
| `ler_inteiro()` | Lê e valida qualquer entrada numérica inteira |
| `coletar_dados()` | Coleta todos os dados da sessão do operador |
| `calcular_tarifa()` | Retorna o valor R$/kWh conforme tipo e horário |
| `nome_tarifa()` | Retorna o nome da modalidade tarifária para exibição |
| `simular_recarga()` | Executa a simulação minuto a minuto com barra visual |
| `exibir_relatorio()` | Formata e imprime o relatório completo da sessão |

---

## Exemplo de saída

```
+--------------------------------------------------+
|    CHARGEGRID  --  Simulador de Recarga EV      |
|    GW7K-HCA-20  |  CA monofasico  |  7,4 kW     |
+--------------------------------------------------+

[ STATUS ] LED VERDE  (fixo)     -- Standby, aguardando

Nome do usuario: Thiago Renatino

  Opcoes de plano:
    1 -- Comum      (tarifa normal R$0,85 | ponta R$1,40)
    2 -- Fidelidade (tarifa fixa R$0,70/kWh)
Selecione o plano (1 ou 2): 2

  Horario de ponta: 18h00 ate 21h00 (tarifa mais cara)
Hora de inicio (HH:MM): 08:00
Duracao em minutos (1-480): 60

[ STATUS ] LED AZUL   (fixo)     -- Recarga em andamento

Recarga iniciada: 60 minutos @ 7.4 kW

  [####################] 100%  7.40 kWh

+--------------------------------------------------+
|        RELATORIO DA SESSAO DE RECARGA           |
+--------------------------------------------------+
| Usuario     : Thiago Renatino                   |
| Tipo        : Fidelidade                        |
| Hora inicio : 08:00                             |
| Hora fim    : 09:00                             |
| Duracao     : 1h 00min                          |
| Energia     : 7.40 kWh                          |
| Modalidade  : Tarifa Fidelidade                 |
| Tarifa      : R$ 0.70/kWh                       |
+--------------------------------------------------+
| CUSTO TOTAL : R$ 5.18                           |
+--------------------------------------------------+

[ STATUS ] LED VERDE  (piscando) -- Sessao concluida

Sessao encerrada. Desconecte o cabo com seguranca.

Iniciar nova sessao? (1=Sim / 0=Nao): 0

Sistema encerrado. Ate logo!
```
