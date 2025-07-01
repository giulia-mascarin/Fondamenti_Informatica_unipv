#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct sale // salva le informazioni presenti nel file passato su linea di comando
{
    int ora, min;
    char cliente[5];
    int sala;
    char tipo;
};

struct utenti
{ // salva le informazioni riguardo ad un singolo cliente
    char cliente[5];
    int tempo_tot;
    int accessi[5];
};

struct sale *lettura_file(FILE *fp, int *n);
int accessi_tot(struct sale *vet, int n);
int ora_max(struct sale *vet, int n);
int ind_max(int *vet, int n);
void presenze_sala(struct sale *vet, int n);
void stampa_vet(int *vet, int n);
struct utenti *tempo_medio(struct sale *vet, int n, int *count);
int trova_ripetizioni(struct utenti *ut, char *nome, int count);
void calcola_tempo(struct sale *p, struct utenti *ut, int *n);
int conversione_ore(int ore, int min);
int media(struct utenti *ut, int count, int n_accessi);
int accessi_sale(struct sale *vet, int n, struct utenti *ut, int count);
int conta_sale_diverse(struct utenti *ut);
int conta_multipli(struct utenti *ut, int count);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        return 1;
        fprintf(stderr, "#numero parametri errato\n");
    }
    FILE *fp = NULL;
    struct sale *vet;
    struct utenti *ut;
    int n;     // dimensione del vettore struct sale;
    int count; // dimensione del vettore struct utenti

    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        fprintf(stderr, "#errore nell'apertura del file");
        return 1;
    }

    vet = lettura_file(fp, &n);
    printf("[ACCESSI]\n");
    printf("%d\n", accessi_tot(vet, n));
    printf("[ORA-MAX]\n");
    printf("%d\n", ora_max(vet, n));
    printf("[PRESENZE]\n");
    presenze_sala(vet, n);
    printf("[TEMPO-MEDIO]\n");
    ut = tempo_medio(vet, n, &count);
    // printf("%s\n", ut[0].cliente);
    printf("[ACCESSI-MULTIPLI]\n");
    printf("%d\n", accessi_sale(vet, n, ut, count));
    fclose(fp);
    free(vet);
    free(ut);
    return 0;
}

struct sale *lettura_file(FILE *fp, int *n)
{
    struct sale *vet;
    int n_conv;
    int size = 8;
    char buf[1000];
    (*n) = 0;
    if (!(vet = malloc(sizeof(*vet) * size)))
    {
        fprintf(stderr, "#errore nell'allocazione della memoria 1\n");
        free(vet);
        return NULL;
    }
    while (fgets(buf, sizeof(buf), fp))
    {
        n_conv = sscanf(buf, "%d:%d %s %d %c",
                        &(vet[*n].ora), &(vet[*n].min), vet[*n].cliente,
                        &(vet[*n].sala), &(vet[*n].tipo));
        if (n_conv != 5)
        {
            fprintf(stderr, "#formato file non valido %d\n", *n);
            continue;
        }
        {
            (*n)++;
            if ((*n) >= size)
            {
                size *= 2;
                if (!(vet = realloc(vet, sizeof(*vet) * size)))
                    fprintf(stderr, "#errore nell'allocazione della memoria 2\n");
                free(vet);
                return NULL;
            }
        }
    }
    vet = realloc(vet, sizeof(*vet) * (*n));
    return vet;
}
int accessi_tot(struct sale *vet, int n)
{
    int i;
    int tot = 0;
    for (i = 0; i < n; i++)
    {
        if (vet[i].tipo == 'E')
            tot++;
    }
    return tot;
}
int ora_max(struct sale *vet, int n)
{
    int *freq; // ogni entrata del vettore registra il numero di accessi avvenuti in una singola ora,
               // partendo dalla prima ora della giornata indicata nel file
    int i;
    int count = 0;        // conta le ore totali dalla prima all'ultima in cui sono avvenuti accessi
    int ind_max_presenze; // indice dell'elemento con valore massimo del vettore freq
    int ora_di_punta;
    int ora = vet[0].ora; // tiene in memoria l'ora appena presa in considerazione nel ciclo for
    freq = calloc(sizeof(*vet) * n, '0');
    for (i = 0; i < n; i++)
    {
        if (vet[i].tipo == 'E')
        {
            if (vet[i].ora == ora) // se l'accesso è avvenuto nella stessa ora di quello precedente appena controllato
                freq[count]++;
            else
            {
                count += vet[i].ora - ora;
                freq[count]++;
                ora = vet[i].ora;
            }
        }
    }
    freq = realloc(freq, sizeof(*vet) * (count + 1));
    ind_max_presenze = ind_max(freq, count + 1);
    ora_di_punta = vet[0].ora + ind_max_presenze;
    return ora_di_punta;
}
int ind_max(int *vet, int n)
{
    int i;
    int max = 0;
    for (i = 1; i < n; i++)
    {
        if (vet[i] >= vet[max])
            max = i;
    }
    return max;
}
void presenze_sala(struct sale *vet, int n)
{
    int i, j;
    int pres[5] = {0};
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < 5; j++)
        {
            if (vet[i].sala == j + 1)
            {
                if (vet[i].tipo == 'E')
                    pres[j]++;
                else
                    pres[j]--;
            }
        }
    }
    stampa_vet(pres, 5);
}
void stampa_vet(int *vet, int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d\n", vet[i]);
}
struct utenti *tempo_medio(struct sale *vet, int n, int *count)
{
    struct utenti *ut;
    int i;
    int ind;
    int n_accessi = 0;
    (*count) = 0;
    ut = malloc(sizeof(*ut) * n);
    // printf("%d\n", n);
    for (i = n - 1; i >= 0; i--)
    {
        ind = trova_ripetizioni(ut, vet[i].cliente,
                                *count);
        // printf("%d\n", ind);
        if (ind < 0)
        {
            strcpy(ut[(*count)].cliente,
                   vet[i].cliente);
            // printf("%s\n", ut[(*count)].cliente);
            ut[*count].tempo_tot = 0;
            if (vet[i].tipo == 'U')
            {
                // printf("ok\n");
                calcola_tempo(vet + i, ut + (*count), &n_accessi);
            }
            (*count)++;
        }

        else
            calcola_tempo(vet + i, ut + ind,
                          &n_accessi);
    }
    // printf("%d\n", *count);
    // ut=realloc(ut, sizeof(*ut)*(*count));
    printf("%d\n", media(ut, *(count), n_accessi));
    return ut;
}
int trova_ripetizioni(struct utenti *ut, char *nome, int count)
{
    int i;
    for (i = 0; i < count; i++)
    {
        if (!(strcmp(ut[i].cliente, nome)))
            return i;
    }
    return -1;
}

void calcola_tempo(struct sale *p, struct utenti *ut, int *n)
{
    int minuti;
    minuti = conversione_ore(p->ora, p->min);
    if (p->tipo == 'U')
    {
        (ut->tempo_tot) += minuti;
        (*n)++;
        // rintf("%d\n", ut->tempo_tot);
    }
    else
        (ut->tempo_tot) = (ut->tempo_tot) - minuti;
    // printf("%d\n", ut->tempo_tot);
}

int conversione_ore(int ore, int min)
{
    int minuti_tot = ore * 60 + min;
    return minuti_tot;
}

int media(struct utenti *ut, int count, int n_accessi)
{
    int i;
    int media = 0;
    for (i = 0; i < count; i++)
        media += ut[i].tempo_tot;
    if (media == 0)
        return 0;
    return media / n_accessi;
}

int accessi_sale(struct sale *vet, int n, struct utenti *ut, int count)
{
    int i;
    int ind;
    int sala;
    for (i = 0; i < n; i++)
    {
        if (vet[i].tipo == 'E')
        {
            ind = trova_ripetizioni(ut,
                                    vet[i].cliente, count);
            sala = vet[i].sala;
            (ut[ind].accessi[sala - 1])++;
        }
    }
    return conta_multipli(ut, count);
}

int conta_sale_diverse(struct utenti *ut)
{
    int i;
    int conta_sale = 0;
    for (i = 0; i < 5; i++)
    {
        if ((ut->accessi[i]) != 0)
            conta_sale++;
    }
    if (conta_sale > 1)
        return 1;
    return 0;
}

int conta_multipli(struct utenti *ut, int count)
{
    int i;
    int multipli = 0;
    for (i = 0; i < count; i++)
    {
        if (conta_sale_diverse(ut + i))
            multipli++;
    }
    return multipli;
}