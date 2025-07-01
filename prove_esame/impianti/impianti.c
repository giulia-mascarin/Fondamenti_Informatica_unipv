#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COSTO 0.05

struct accesso
{
    int giorno;
    char impianto[7];
    int punti;
    char skipass[7];
    char tipo;
};

struct imp
{
    char impianto[7];
    int n_accessi;
};

struct accesso *lettura_file(FILE *fp, int *n);
void accessi_per_pass(struct accesso *vet, int n, char *c);
void stampa_vet(int *vet, int n);
void max_accessi(struct accesso *vet, int n);
int incrementa_ripetizioni(struct imp *imp, char *nome, int n);
int trova_max(struct imp *imp, int n);
int vet_ind_max(int *vet, int n);
void giorno_punta(struct accesso *vet, int n);
void incasso(struct accesso *vet, int n);

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        return 1;
        fprintf(stderr, "#numero parametri errato\n");
    }
    FILE *fp;
    struct accesso *vet;
    int n;

    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        fprintf(stderr, "#errore nell'apertura del file\n");
        return 1;
    }
    vet = lettura_file(fp, &n);
    printf("[TOTALE_ACCESSI]\n");
    printf("%d\n", n);
    printf("[ACCESSI_IMPIANTO]\n");
    accessi_per_pass(vet, n, argv[2]);
    printf("[IMPIANTO_MAX]\n");
    max_accessi(vet, n);
    printf("[GIORNO_PUNTA]\n");
    giorno_punta(vet, n);
    printf("[INCASSO_PUNTI]\n");
    incasso(vet, n);
    fclose(fp);
    free(vet);
    return 0;
}

struct accesso *lettura_file(FILE *fp, int *n)
{
    struct accesso *vet;
    int n_conv;
    int size = 8;
    char buf[1000];
    (*n) = 0;
    if (!(vet = malloc(sizeof(*vet) * size)))
    {
        fprintf(stderr, "#errore nell'allocazione di memoria1\n");
        free(vet);
        return NULL;
    }
    while (fgets(buf, sizeof(buf), fp))
    {
        n_conv = sscanf(buf, "%d %s %d %s %c",
                        &(vet[*n].giorno), vet[*n].impianto,
                        &(vet[*n].punti), vet[*n].skipass,
                        &(vet[*n].tipo));
        if (n_conv != 5)
            continue;
        (*n)++;
        if ((*n) >= size)
        {
            size *= 2;
            if (!(vet = realloc(vet, sizeof(*vet) *
                                         size)))
            {
                fprintf(stderr, "#errore nell'allocazione di memoria 2\n");
                free(vet);
                return NULL;
            }
        }
    }
    if (!(vet = realloc(vet, sizeof(*vet) * (*n))))
    {
        fprintf(stderr, "#errore nell'allocazione di memoria 3\n");
        free(vet);
        return NULL;
    }
    return vet;
}

void accessi_per_pass(struct accesso *vet, int n, char *c)
{
    int i, j;
    int freq[3] = {0};
    char type[3] = {'G', 'P', 'S'};
    for (i = 0; i < n; i++)
    {
        if (!(strcmp(vet[i].impianto, c)))
        {
            for (j = 0; j < 3; j++)
            {
                if (vet[i].tipo == type[j])
                    freq[j]++;
            }
        }
    }
    stampa_vet(freq, 3);
}
void stampa_vet(int *vet, int n)
{
    int i = 0;
    for (i = 0; i < n; i++)
        printf("%d\n", vet[i]);
}
void max_accessi(struct accesso *vet, int n)
{
    struct imp *imp;
    int i;
    int count = 0;
    int max;
    imp = malloc(sizeof(*imp) * n);
    for (i = 0; i < n; i++)
    {
        imp[i].n_accessi = 0;
    }
    for (i = 0; i < n; i++)
    {
        if (incrementa_ripetizioni(imp, vet[i].impianto, count))
        {
            strcpy(imp[count].impianto, vet[i].impianto);
            imp[count++].n_accessi++;
        }
    }
    max = trova_max(imp, count);
    for (i = 0; i < count; i++)
    {
        if (imp[i].n_accessi == max)
            printf("%s\n", imp[i].impianto);
    }
}
int incrementa_ripetizioni(struct imp *imp, char *nome, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (!(strcmp(imp[i].impianto, nome)))
        {
            (imp[i].n_accessi)++;
            return 0;
        }
    }
    return 1;
}

int trova_max(struct imp *imp, int n)
{
    int i;
    int max = imp[0].n_accessi;
    for (i = 1; i < n; i++)
    {
        if (imp[i].n_accessi > max)
            max = imp[i].n_accessi;
    }
    return max;
}

void giorno_punta(struct accesso *vet, int n)
{
    int i;
    int max;
    int *freq;
    int count = 0;
    freq = calloc(sizeof(*freq) * n, '0');
    for (i = 0; i < n; i++)
    {
        if (vet[i].giorno == vet[0].giorno + count)
            freq[count]++;
        else
        {
            count += vet[i].giorno - vet[i - 1].giorno;
            freq[count]++;
        }
    }
    max = vet_ind_max(freq, count + 1);
    for (i = 0; i < (count + 1); i++)
    {
        if (freq[i] == freq[max])
        {
            printf("%d\n", vet[0].giorno + max);
            break;
        }
    }
}

int vet_ind_max(int *vet, int n)
{
    int i;
    int max = 0;
    for (i = 1; i < n; i++)
    {
        if (vet[i] > vet[max])
            max = i;
    }
    return max;
}

void incasso(struct accesso *vet, int n)
{
    int i;
    float incasso = 0;
    for (i = 0; i < n; i++)
    {
        if (vet[i].tipo == 'P')
            incasso += COSTO * vet[i].punti;
    }
    printf("%.2lf\n", incasso);
}