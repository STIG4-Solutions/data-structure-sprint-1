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

void coletar_dados(SessaoRecarga *s) // Ponteiro que aponta para Struct
{
    int len;

    printf("Nome do usuario: ");
    fgets(s->nome, sizeof(s->nome), stdin); // fgets lê linha inteira
    len = (int)strlen(s->nome);
    if (len > 0 && s->nome[len - 1] == '\n')
        s->nome[len - 1] = '\0';
    if (s->nome[0] == '\0')
        strcpy(s->nome, "Anonimo");

    printf("\n  Opcoes de plano:\n");
    printf("    1 -- Comum      (tarifa normal R$0,85 | ponta R$1,40)\n");
    printf("    2 -- Fidelidade (tarifa fixa R$0,70/kWh)\n");
    s->tipo_usuario = ler_inteiro("Selecione o plano (1 ou 2): ", 1, 2);

    printf("\n  Horario de ponta: %dh00 ate %dh00 (tarifa mais cara)\n",
           HORA_PONTA_INI, HORA_PONTA_FIM);
    {
        int h, m, lido, c;
        do
        {
            printf("Hora de inicio (HH:MM): ");
            lido = scanf("%d:%d", &h, &m);
            do { c = getchar(); } while (c != '\n' && c != EOF);
            if (lido != 2 || h < 0 || h > 23 || m < 0 || m > 59)
                printf("  [ERRO] Formato invalido. Use HH:MM (ex: 19:30).\n");
        } while (lido != 2 || h < 0 || h > 23 || m < 0 || m > 59);
        s->hora_inicio   = h;
        s->minuto_inicio = m;
    }

    printf("\n  Duracao maxima: %d minutos (8 horas)\n", DURACAO_MAX);
    s->duracao_min = ler_inteiro("Duracao em minutos (1-480): ", DURACAO_MIN, DURACAO_MAX);
}

float calcular_tarifa(int hora, int tipo)
{
    if (tipo == TIPO_FIDELIDADE)
        return TARIFA_FIDELIDADE;
    if (hora >= HORA_PONTA_INI && hora <= HORA_PONTA_FIM)
        return TARIFA_PONTA;
    return TARIFA_NORMAL;
}

const char *nome_tarifa(int hora, int tipo)
{
    if (tipo == TIPO_FIDELIDADE)
        return "Fidelidade";
    if (hora >= HORA_PONTA_INI && hora <= HORA_PONTA_FIM)
        return "Ponta";
    return "Normal";
}

void simular_recarga(SessaoRecarga *s)
{
    s->energia_kwh    = (POTENCIA_KW / 60.0f) * s->duracao_min;
    s->tarifa_aplicada = calcular_tarifa(s->hora_inicio, s->tipo_usuario);
    s->custo_total    = s->energia_kwh * s->tarifa_aplicada;
    s->status         = CONCLUIDO;

    exibir_status_led(CARREGANDO);
    printf("Recarga simulada: %d minutos @ %.1f kW\n\n", s->duracao_min, POTENCIA_KW);
}

void exibir_relatorio(const SessaoRecarga *s)
{
    char buf[40];
    int h = s->duracao_min / 60;
    int m = s->duracao_min % 60;
    int fim_total = s->hora_inicio * 60 + s->minuto_inicio + s->duracao_min;
    int hora_fim  = (fim_total / 60) % 24;
    int min_fim   = fim_total % 60;
    const char *tipo_str   = (s->tipo_usuario == TIPO_FIDELIDADE) ? "Fidelidade" : "Comum";
    const char *tarifa_str = nome_tarifa(s->hora_inicio, s->tipo_usuario);

    separador();
    printf("|        RELATORIO DA SESSAO DE RECARGA           |\n");
    separador();

    linha_rel("Usuario", s->nome);
    linha_rel("Tipo", tipo_str);

    snprintf(buf, sizeof(buf), "%02d:%02d", s->hora_inicio, s->minuto_inicio);
    linha_rel("Hora inicio", buf);

    snprintf(buf, sizeof(buf), "%02d:%02d", hora_fim, min_fim);
    linha_rel("Hora fim", buf);

    if (h > 0) snprintf(buf, sizeof(buf), "%dh %02dmin", h, m);
    else        snprintf(buf, sizeof(buf), "%d min", m);
    linha_rel("Duracao", buf);

    snprintf(buf, sizeof(buf), "%.2f kWh", s->energia_kwh);
    linha_rel("Energia", buf);

    snprintf(buf, sizeof(buf), "Tarifa %s", tarifa_str);
    linha_rel("Modalidade", buf);

    snprintf(buf, sizeof(buf), "R$ %.2f/kWh", s->tarifa_aplicada);
    linha_rel("Tarifa", buf);

    separador();

    snprintf(buf, sizeof(buf), "R$ %.2f", s->custo_total);
    linha_rel("CUSTO TOTAL", buf);

    separador();
    printf("\n");
    exibir_status_led(s->status);
}

int main(void)
{
    SessaoRecarga sessao;
    int nova_sessao;

    exibir_cabecalho();

    do
    {
        memset(&sessao, 0, sizeof(SessaoRecarga)); // Cada sessão começa do zero
        sessao.status = STANDBY;

        exibir_status_led(STANDBY);
        coletar_dados(&sessao);
        printf("\n");
        simular_recarga(&sessao);
        exibir_relatorio(&sessao);

        switch (sessao.status)
        {
        case CONCLUIDO:
            printf("Sessao encerrada. Desconecte o cabo com seguranca.\n\n");
            break;
        case FALHA:
            printf("Sessao encerrada com falha. Contate o suporte.\n\n");
            break;
        default:
            printf("Sessao encerrada.\n\n");
            break;
        }

        nova_sessao = ler_inteiro("Iniciar nova sessao? (1=Sim / 0=Nao): ", 0, 1);
        printf("\n");
    } while (nova_sessao);

    printf("Sistema encerrado. Ate logo!\n\n");
    return 0;
}
