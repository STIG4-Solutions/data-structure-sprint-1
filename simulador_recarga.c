/*
 * CHARGEGRID -- Simulador de Sessão de Recarga Veicular
 * Sprint 1 -- Estruturas de Dados em C
 * Hardware: GW7K-HCA-20 | Linha HCA G2 | 7,4 kW CA
 */

#include <stdio.h>
#include <string.h>

// Tarifas (R$/kWh)
#define TARIFA_NORMAL     0.85f
#define TARIFA_PONTA      1.40f
#define TARIFA_FIDELIDADE 0.70f

// Equipamento e horário de maior demanda
#define POTENCIA_KW    7.4f
#define HORA_PONTA_INI 18
#define HORA_PONTA_FIM 21

// Tipos de usuário
#define TIPO_COMUM      1
#define TIPO_FIDELIDADE 2

// Limites de duração da sessão (em minutos)
#define DURACAO_MIN   1
#define DURACAO_MAX 480

typedef enum
{
    STANDBY    = 0, // LED verde fixo -- aguardando
    CARREGANDO = 1, // LED azul fixo -- recarga ativa
    CONCLUIDO  = 2, // LED verde piscando -- sessão OK
    FALHA      = 3  // LED vermelho fixo -- erro no sistema
} StatusLED;

typedef struct
{
    char      nome[50];
    int       tipo_usuario;
    int       hora_inicio;
    int       minuto_inicio;
    int       duracao_min;
    float     energia_kwh;
    float     tarifa_aplicada;
    float     custo_total;
    StatusLED status;
} SessaoRecarga;

// Def. protótipos das funções
void        exibir_cabecalho(void);
void        exibir_status_led(StatusLED status);
int         ler_inteiro(const char *prompt, int min, int max);
void        coletar_dados(SessaoRecarga *s);
float       calcular_tarifa(int hora, int tipo);
const char *nome_tarifa(int hora, int tipo);
void        simular_recarga(SessaoRecarga *s);
void        exibir_relatorio(const SessaoRecarga *s);

static void separador(void)
{
    printf("+--------------------------------------------------+\n");
}

// Formatação de linha do relatório
static void linha_rel(const char *label, const char *valor)
{
    printf("| %-13s: %-34s|\n", label, valor);
}

void exibir_cabecalho(void)
{
    printf("\n");
    separador();
    printf("|    CHARGEGRID  --  Simulador de Recarga EV      |\n");
    printf("|    GW7K-HCA-20  |  CA monofasico  |  7,4 kW     |\n");
    separador();
    printf("\n");
}

void exibir_status_led(StatusLED status)
{
    printf("[ STATUS ] ");
    switch (status)
    {
    case STANDBY:    printf("LED VERDE  (fixo)     -- Standby, aguardando\n");   break;
    case CARREGANDO: printf("LED AZUL   (fixo)     -- Recarga em andamento\n"); break;
    case CONCLUIDO:  printf("LED VERDE  (piscando) -- Sessao concluida\n");      break;
    case FALHA:      printf("LED VERMELHO (fixo)   -- Falha no sistema\n");      break;
    }
    printf("\n");
}

int ler_inteiro(const char *prompt, int min, int max)
{
    int valor, c;
    do
    {
        printf("%s", prompt);
        if (scanf("%d", &valor) != 1)
            valor = min - 1;
        do { c = getchar(); } while (c != '\n' && c != EOF); // Descarta buffer até o fim da linha
        if (valor < min || valor > max)
            printf("  [ERRO] Entrada invalida. Digite entre %d e %d.\n", min, max);
    } while (valor < min || valor > max);
    return valor;
}

int main(void)
{
    return 0;
}
