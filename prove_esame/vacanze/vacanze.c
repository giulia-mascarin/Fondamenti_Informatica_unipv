#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 6
#define DIM 31

struct cartolina
{
    int anno, mese;
    char mitt[DIM];
    char dest[DIM];
    char loc[DIM];
};

struct utente
{
    char nome[DIM];
    int inviate, ricevute;
};

struct cartolina *lettura_file(FILE *fp, int *n);
void stampa(struct cartolina *vet, int n);
void conta_mesi(struct cartolina *vet, int n);
void inserimento_dati(struct cartolina *vet, struct utente *utenti,
                      int n);
void ricevute(struct utente *utenti);
void stampa_ut(struct utente *utenti);
void max_mesi(struct cartolina *vet, int n);
void localita(struct cartolina *vet, int n);
int controllo_ripetizioni(char localita[][DIM], char *nome, int n);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        return 1;
        fprintf(stderr, "numero parametri errato\n");
    }
    FILE *fp;
    struct cartolina *vet;
    struct utente utenti[N];
    int n;
    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        return 1;
        fprintf(stderr, "errore nell'apertura file\n");
    }
    vet = lettura_file(fp, &n);
    stampa(vet, n);
    printf("[CARTOLINE]\n%d\n", n);
    printf("[MEDIA_PERIODO]\n");
    conta_mesi(vet, n);
    inserimento_dati(vet, utenti, n);
    printf("[MASSIMO_RICEVUTE]\n");
    ricevute(utenti);
    printf("[INVIATE]\n");
    stampa_ut(utenti);
    printf("[MESI_MAX_CARTOLINE]\n");
    max_mesi(vet, n);
    printf("[LOCALITA]\n");
    localita(vet, n);
    fclose(fp);
    free(vet);
    return 0;
}

struct cartolina *lettura_file(FILE *fp, int *n)
{
    struct cartolina *vet;
    int n_conv;
    int size = 8;
    char buf[1000];
    (*n) = 0;
    if (!(vet = malloc(sizeof(*vet) * size)))
    {
        fprintf(stderr, "errore nell'allocazione della memoria 1\n");
        free(vet);
        return NULL;
    }

    while (fgets(buf, sizeof(buf), fp))
    {
        n_conv =
            sscanf(buf, "%d %d %s %s %s", &(vet[*n].anno),
                   &(vet[*n].mese), vet[*n].mitt, vet[*n].dest,
                   vet[*n].loc);
        if (n_conv != 5)
            continue;

        (*n)++;
        if ((*n) >= size)
        {
            size *= 2;
            if (!(vet = realloc(vet, sizeof(*vet) * size)))
            {
                fprintf(stderr, "errore nell'allocazione della memoria 2\n");
                free(vet);
                return NULL;
            }
        }
    }

    if (!(vet = realloc(vet, sizeof(*vet) * (*n))))
    {
        fprintf(stderr, "errore nell'allocazione della memoria 3\n");
        free(vet);
        return NULL;
    }
    return vet;
}

void stampa(struct cartolina *vet, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("%d %d %s %s\n", vet[i].anno, vet[i].mese,
               vet[i].mitt,
               vet[i].dest);
    }
}
void conta_mesi(struct cartolina *vet, int n)
{
    int mesi_tot;
    mesi_tot = (vet[n - 1].anno - vet[0].anno) * 12 -
               vet[0].mese + vet[n - 1].mese + 1;
    printf("%d\n", mesi_tot);
    double media_mesi = n / (double)mesi_tot;
    printf("%.2lf\n", media_mesi);
}

void inserimento_dati(struct cartolina *vet, struct utente *utenti, int n)
{
    int i, j;
    char *nomi[N] = {"Gino", "Luigi", "Mario", "Tino", "Toni", "Ugo"};
    for (i = 0; i < N; i++)
    {
        strcpy(utenti[i].nome, nomi[i]);
        utenti[i].inviate = 0;
        utenti[i].ricevute = 0;
    }
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < N; j++)
        {
            if (!(strcmp(utenti[j].nome, vet[i].mitt)))

                (utenti[j].inviate)++;
            if (!(strcmp(utenti[j].nome, vet[i].dest)))
                (utenti[j].ricevute)++;
        }
    }
}

void stampa_ut(struct utente *utenti)
{
    int i;
    for (i = 0; i < N; i++)
        printf("%s %d\n", utenti[i].nome,
               utenti[i].inviate);
}

void ricevute(struct utente *utenti)
{
    int i;
    int max = utenti[0].ricevute;
    for (i = 0; i < N; i++)
    {
        if (utenti[i].ricevute > max)
            max = utenti[i].ricevute;
    }
    for (i = 0; i < N; i++)
    {
        if (utenti[i].ricevute == max)
            puts(utenti[i].nome);
    }
}

void max_mesi(struct cartolina *vet, int n)
{
    int i;
    int max;
    int freq[12] = {0};
    char *mesi[12] =
        {"gen", "feb", "mar", "apr", "mag", "giu", "lug",
         "ago", "set",
         "ott", "nov", "dic"};
    for (i = 0; i < n; i++)
        freq[(vet[i].mese) - 1]++;
    max = freq[0];
    for (i = 1; i < 12; i++)
    {
        if (freq[i] > max)
            max = freq[i];
    }
    for (i = 0; i < 12; i++)
    {
        if (freq[i] == max)
            printf("%s\n", mesi[i]);
    }
}

void localita(struct cartolina *vet, int n)
{
    int i;
    char localita[n][DIM];
    int count = 0;
    for (i = 0; i < n; i++)
    {
        if (controllo_ripetizioni(localita, vet[i].loc,
                                  count))
        {
            strcpy(localita[count], vet[i].loc);
            printf("%s\n", localita[count++]);
        }
    }
}

int controllo_ripetizioni(char localita[][DIM], char *nome, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (!(strcmp(localita[i], nome)))
            return 0;
    }
    return 1;
}