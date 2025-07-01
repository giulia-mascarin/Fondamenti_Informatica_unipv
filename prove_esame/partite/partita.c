#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRINGA 80
typedef char nome[MAX_STRINGA + 1];

typedef struct partita
{
    nome squadre[2];
    int punti[2];
} partita;

typedef struct squadra
{
    char nome[MAX_STRINGA + 1];
    int giocate;
    int vinte;
} squadra;

partita *lettura_file(FILE *fp, int *n);
void compila(partita *vet, int n);
// int trova_ripetizioni(squadra *sq, int count, char *n);
// int partita_vinta(partita *p);
// int miglior_punt(squadra *sq, int count);
// void stampa_squadre(squadra *sq, int count);
int max_punt(partita *vet, int n);
void squadra_max(partita *vet, int n);
void vitt_in_out(partita *vet, int n);
double media(partita *vet, int n);
int trova_ripetizione(squadra *elenco, char *c, int count);
void giocate_vinte(partita *vet, int n, squadra *sq, int count);
void stampa_sq(squadra *sq, int count);
int cmp_nome(const void *c1, const void *c2);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        return 1;
        fprintf(stderr, "#numero parametri errato\n");
    }
    FILE *fp;
    partita *vet;
    int n;
    fp = fopen(argv[1], "r");
    vet = lettura_file(fp, &n);
    printf("[PUNTEGGIO-MIGLIORE]\n");
    // compila(vet, n, &count);
    squadra_max(vet, n);
    printf("[VITTORIE]\n");
    vitt_in_out(vet, n);
    printf("[MEDIA]\n");
    printf("%lf\n", media(vet, n));
    printf("[SQUADRE]\n");
    compila(vet, n);
    free(vet);
    fclose(fp);
    return 0;
}

partita *lettura_file(FILE *fp, int *n)
{
    int n_conv;
    int size = 8;
    char buf[1000];
    partita *vet;
    (*n) = 0;
    vet = malloc(sizeof(*vet) * size);
    while (fgets(buf, sizeof(buf), fp))
    {
        n_conv = sscanf(buf, "%s %*s %s %d - %d",
                        vet[*n].squadre[0], vet[*n].squadre[1], &(vet[*n].punti[0]),
                        &(vet[*n].punti[1]));
        if (n_conv != 4)
        {
            printf("formato non valido %d %d\n",
                   *n, n_conv);
            continue;
        }
        (*n)++;
        if (*n >= size)
        {
            size *= 2;
            vet = realloc(vet, sizeof(*vet) * size);
        }
    }
    vet = realloc(vet, sizeof(*vet) * size);
    return vet;
}

int max_punt(partita *vet, int n)
{
    int max = 0;
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < 2; j++)
        {
            if (vet[i].punti[j] > max)
                max = vet[i].punti[j];
        }
    }
    return max;
}

void squadra_max(partita *vet, int n)
{
    int i;
    int j;
    int max = max_punt(vet, n);
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < 2; j++)
        {
            if (vet[i].punti[j] == max)
                printf("%s\n", vet[i].squadre[j]);
        }
    }
}

void vitt_in_out(partita *vet, int n)
{
    int i;
    int vitt_in = 0;
    int vitt_out = 0;
    for (i = 0; i < n; i++)
    {
        if (vet[i].punti[0] < vet[i].punti[1])
            vitt_out++;
        else
            vitt_in++;
    }
    printf("%d\n%d\n", vitt_in, vitt_out);
}

double media(partita *vet, int n)
{
    int i;
    int tot = 0;
    for (i = 0; i < n; i++)
        tot += vet[i].punti[0] + vet[i].punti[1];
    return (double)(tot / (n));
}

void compila(partita *vet, int n)
{
    int i, j;
    squadra *elenco;
    int(count) = 0;
    elenco = malloc(sizeof(*elenco) * n);
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < 2; j++)
        {
            if (trova_ripetizione(elenco,
                                  vet[i].squadre[j], count) < 0)
                strcpy(elenco[(count)++].nome,
                       vet[i].squadre[j]);
        }
    }
    for (i = 0; i < (count); i++)
        printf("%s\n", elenco[i].nome);
    giocate_vinte(vet, n, elenco, count);
    qsort(elenco, count, sizeof(*elenco), cmp_nome);
    printf("[PARTITE]\n");
    stampa_sq(elenco, count);
}

int trova_ripetizione(squadra *elenco, char *c, int count)
{
    int i;
    for (i = 0; i < count; i++)
    {
        if (!(strcmp(elenco[i].nome, c)))
            return i;
    }
    return -1;
}

void giocate_vinte(partita *vet, int n, squadra *sq, int count)
{
    int i, ind_0, ind_1;
    for (i = 0; i < n; i++)
    {
        sq[i].giocate = 0;
        sq[i].vinte = 0;
    }
    for (i = 0; i < n; i++)
    {
        ind_0 = trova_ripetizione(sq,
                                  vet[i].squadre[0], count);
        sq[ind_0].giocate++;
        ind_1 = trova_ripetizione(sq,
                                  vet[i].squadre[1], count);
        sq[ind_1].giocate++;
        if (vet[i].punti[0] > vet[i].punti[1])
            sq[ind_0].vinte++;
        else
            sq[ind_1].vinte++;
    }
}

void stampa_sq(squadra *sq, int count)
{
    int i;
    for (i = 0; i < count; i++)
        printf("%s %d %d\n", sq[i].nome, sq[i].giocate, sq[i].vinte);
}

int cmp_nome(const void *p1, const void *p2)
{
    const squadra *c1 = p1;
    const squadra *c2 = p2;
    return strcmp(c1->nome, c2->nome);
}
