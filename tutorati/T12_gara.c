#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RIGA 50
#define P 10 // fino dove arrivano nella classifica i piloti che prendono punti
#define U 20 // fino dove arrivando quelli che non prendono punti

int punti[] = {25, 18, 15, 12, 10, 8, 6, 4, 2, 1};

typedef struct classificato
{
    char nome[MAX_RIGA + 1];
    char squadra[MAX_RIGA + 1];
    int punteggio;
} classificato;

typedef struct squadra
{
    char squadra[MAX_RIGA + 1];
    int punteggio;
} squadra;

classificato *lettura_file(FILE *fp, int *n);
void calcola_punteggio(classificato *vet, int n);
void stampa_punti(classificato *vet, int n);
int cmp_punti_nome(const void *p1, const void *p2);
squadra *squadra_vincitrice(classificato *vet, int n);
int trova_ripetizioni(squadra *sq, int n, char *nome);
int cmp_punti_squadra(const void *p1, const void *p2);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        return 1;
        fprintf(stderr, "#uso dei parametri errato\n");
    }
    FILE *fp;
    classificato *vet;
    int n;
    squadra *sq;
    fp = fopen(argv[1], "r");
    vet = lettura_file(fp, &n);
    printf("[NUMERO_GARE]\n");
    printf("%d\n", n / U);
    printf("[PILOTA_VINCITORE]\n");
    calcola_punteggio(vet, n);
    qsort(vet, U, sizeof(*vet), cmp_punti_nome);
    printf("%s %d\n", vet[0].nome, vet[0].punteggio);
    printf("[SQUADRA_VINCITRICE]\n");
    sq = squadra_vincitrice(vet, U);
    qsort(sq, 10, sizeof(*sq), cmp_punti_squadra);
    printf("%s %d\n", sq[0].squadra, sq[0].punteggio);
    free(vet);
    fclose(fp);
    return 0;
}

classificato *lettura_file(FILE *fp, int *n)
{
    classificato *vet;
    char buf[MAX_RIGA + 1];
    int size = 8;

    (*n) = 0;
    if (!(vet = malloc(sizeof(*vet) * size)))
    {
        fprintf(stderr, "errore nell'allocazione di memoria 1\n");
        return NULL;
    }

    while (fgets(buf, sizeof(buf), fp))
    {
        if (sscanf(buf, "%s %s\n", vet[*n].nome,
                   vet[*n].squadra) != 2)
            continue;
        {

            (*n)++;
            if ((*n) >= size)
            {
                size *= 2;
                if (!(vet = realloc(vet, sizeof(*vet) * size)))
                    fprintf(stderr, "errore nell'allocazione di memoria 2\n");
                free(vet);
                return NULL;
            }
        }
    }
    return vet;
}

void calcola_punteggio(classificato *vet, int n)
{
    int i;
    classificato *p;
    int j = U;
    for (i = 0; i < P; i++)
        vet[i].punteggio = punti[i];
    for (i = P; i < U; i++)
    {
        vet[i].punteggio = 0;
    }
    for (p = vet + U; p < vet + n; p++)
    {
        for (i = 0; i < U; i++)
        {
            if (!(strcmp(vet[i].nome, p->nome)))
            {
                if ((j % U) < P)
                    vet[i].punteggio += punti[j % U];
            }
        }
        j++; // conta a che riga dopo le prime 20 si trova il pilota
    }
}

void stampa_punti(classificato *vet, int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%s %d\n", vet[i].nome, vet[i].punteggio);
}
int cmp_punti_nome(const void *p1, const void *p2)
{
    const classificato *c1 = p1;
    const classificato *c2 = p2;
    if (c1->punteggio > c2->punteggio)
        return -1;
    else if (c1->punteggio < c2->punteggio)
        return 1;
    else
        return strcmp(c1->nome, c2->nome);
}

squadra *squadra_vincitrice(classificato *vet, int n)
{
    int i;
    squadra *sq;
    int count = 0;
    int ind;
    sq = malloc(sizeof(*sq) * 10);
    for (i = 0; i < n; i++)
    {
        if (ind < 0)
        {
            ind = trova_ripetizioni(sq, count, vet[i].squadra);
            strcpy(sq[count].squadra, vet[i].squadra);
            sq[count++].punteggio = vet[i].punteggio;
        }
        else
            sq[ind].punteggio += vet[i].punteggio;
    }
    return sq;
}

int trova_ripetizioni(squadra *sq, int n, char *nome)
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (!(strcmp(nome, sq[i].squadra)))
            return i;
    }
    return -1;
}

int cmp_punti_squadra(const void *p1, const void *p2)
{
    const squadra *c1 = p1;
    const squadra *c2 = p2;
    if (c1->punteggio > c2->punteggio)
        return -1;
    else if (c1->punteggio < c2->punteggio)
        return 1;
    else
        return strcmp(c1->squadra, c2->squadra);
}