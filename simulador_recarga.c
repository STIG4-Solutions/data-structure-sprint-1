/*
 * CHARGEGRID -- Simulador de Sessao de Recarga Veicular
 * Sprint 1 -- Estruturas de Dados em C
 * Hardware: GW7K-HCA-20 | Linha HCA G2 | 7,4 kW CA
 */

#include <stdio.h>
#include <string.h>

// Tarifas (R$/kWh)
#define TARIFA_NORMAL     0.85f
#define TARIFA_PONTA      1.40f
#define TARIFA_FIDELIDADE 0.70f

// Equipamento e horario de ponta
#define POTENCIA_KW    7.4f
#define HORA_PONTA_INI 18
#define HORA_PONTA_FIM 21

// Tipos de usuario
#define TIPO_COMUM      1
#define TIPO_FIDELIDADE 2

// Limites de duracao da sessao (em minutos)
#define DURACAO_MIN   1
#define DURACAO_MAX 480

int main(void)
{
    return 0;
}
