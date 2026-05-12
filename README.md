# ⚡ EV Charger Simulator — Sprint 1

Simulador de sessão de recarga para veículos elétricos em C puro.  
**FIAP · Análise e Desenvolvimento de Sistemas**

---

## Sobre o projeto

Este programa simula o funcionamento de um terminal de recarga para veículos elétricos, rodando diretamente no terminal. O operador informa os dados da sessão (placa, tipo de usuário, carregador, nível de bateria, tempo e horário), e o programa calcula automaticamente a energia consumida, aplica a tarifa correta e exibe um relatório formatado ao final.

O projeto foi desenvolvido para o Sprint 1 da disciplina de Linguagem C, cobrindo os critérios de avaliação: estruturas condicionais, repetição, lógica de sessão, tarifação, organização de código e entrada/saída de dados.

---

## Como compilar e executar

**Pré-requisito:** GCC instalado. No Windows, use [MinGW](https://www.mingw-w64.org/) ou WSL.

```bash
# Compilar
gcc main.c -o ev_charger

# Executar
./ev_charger          # Linux/macOS
ev_charger.exe        # Windows

# Compilar e executar de uma vez
gcc main.c -o ev_charger && ./ev_charger
```

**No VS Code:** instale a extensão `C/C++` (ms-vscode.cpptools), abra a pasta do projeto e use o terminal integrado com Ctrl+`.

---

## Fluxo da sessão

```
Início
 ├─ Entrada de dados (placa, tipo de usuário, carregador, bateria, tempo, hora)
 ├─ Validação de cada entrada com do-while (rejeita valores fora do intervalo)
 ├─ Detecção de horário de ponta (18h–21h aplica tarifa majorada)
 ├─ Cálculo de energia: acumula potencia_kw / 60.0 por minuto (loop for)
 ├─ Cálculo de tarifa: switch por tipo de usuário + if para horário de ponta
 ├─ Barra de progresso no terminal (loop for com 20 passos)
 └─ Relatório final com todos os dados da sessão
```

O programa permite realizar múltiplas sessões consecutivas — ao final de cada uma, o operador pode iniciar uma nova ou encerrar.

---

## Tarifação

A tarifa varia conforme o tipo de usuário e o horário de início da recarga:

| Usuário  | Fora de ponta | Horário de ponta (18h–21h) |
|----------|---------------|----------------------------|
| Comum    | R$ 1,20/kWh   | R$ 1,85/kWh                |
| Premium  | R$ 1,08/kWh   | R$ 1,67/kWh *(−10%)*       |
| Frota    | R$ 0,90/kWh   | R$ 1,85/kWh                |

Usuários **Premium** recebem 10% de desconto em qualquer horário. Usuários de **Frota** têm tarifa reduzida fora do pico, mas pagam a tarifa cheia no horário de ponta.

**Potência simulada por tipo de carregador:**

| Carregador | Tecnologia | Potência |
|------------|-----------|----------|
| Lento      | AC        | 7,4 kW   |
| Rápido     | DC        | 50 kW    |
| Ultra      | DC        | 150 kW   |

**Fórmula do custo:** `energia_kWh × tarifa_R$/kWh`  
**Fórmula da energia:** `(potencia_kW / 60.0) × tempo_minutos`

---

## Estruturas de C utilizadas

### Condicionais (`if/else` e `switch`)
- `switch` seleciona a tarifa base de acordo com o tipo de usuário (Comum, Premium, Frota)
- `switch` também determina a potência do carregador (Lento, Rápido, Ultra)
- `if/else` aplica a majoração do horário de ponta e o desconto Premium
- `if/else` valida se a bateria ultrapassaria 100% após a recarga

### Repetição (`for` e `do-while`)
- `for` em `calcular_energia()`: acumula a energia consumida minuto a minuto
- `for` em `simular_progresso()`: desenha a barra de progresso passo a passo
- `do-while` em `ler_inteiro()`: repete a leitura até o usuário digitar um valor válido
- `do-while` no `main()`: mantém o programa rodando para múltiplas sessões

### Struct
`Sessao` centraliza todos os dados de uma recarga em uma única estrutura — placa, tipo de usuário, carregador, bateria inicial/final, tempo, energia, tarifa e valor total — e é passada por ponteiro para a função de relatório.

### Funções
O código é dividido em funções com responsabilidade única: `calcular_energia()`, `calcular_tarifa()`, `ler_inteiro()`, `simular_progresso()`, `exibir_relatorio()` e auxiliares de nomes (`nome_tipo_usuario()`, `nome_carregador()`).

---

## Exemplo de saída

```
*******************************************************
*     ⚡  EV CHARGER SIMULATOR  —  Sprint 1  ⚡     *
*       Simulador de Sessao de Recarga em C           *
*******************************************************

=======================================================
  NOVA SESSAO DE RECARGA  #001
=======================================================
[VEICULO]
  Placa do veiculo (ex: ABC1234): EVT1234
[TIPO DE USUARIO]
  1 - Comum
  2 - Premium (10% de desconto)
  3 - Frota
  Selecione: 2
...
=======================================================
  RELATORIO DA SESSAO  #001
=======================================================
  Placa             : EVT1234
  Tipo de usuario   : Premium (10% desc.)
  Carregador        : Rapido — DC 50 kW
-------------------------------------------------------
  Bateria inicial   :  30%
  Bateria final     :  79%
  Tempo de recarga  : 30 min
  Energia consumida : 25.000 kWh
-------------------------------------------------------
  Hora de inicio    : 19h00
  Horario           : PONTA (18h–21h) — tarifa majorada
  Tarifa aplicada   : R$ 1.67/kWh
-------------------------------------------------------
  VALOR TOTAL       : R$ 41.62
=======================================================
```

---

*Desenvolvido por Thiago Renatino*