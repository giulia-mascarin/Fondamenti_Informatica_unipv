#include <stdio.h>
#include <stdlib.h>
#define NMAX 168
struct misura
{
    int anno, mese, giorno, ore, min, sec, msec, umid;
    char id[11];
    float temp, veloc;
};
struct misura *lettura_file(FILE *fp, int *n)
{
    int n_conv;
    int size = 8;
    struct misura *vet;
    char buf[1024];
    *n = 0;
    vet = malloc(sizeof(*vet) * size);
    while (fgets(buf, sizeof(buf), fp))
    {
        n_conv = sscanf(buf, "%d-%d-%d %d:%d:%d.%d %s %f %d%%%f ", &(vet[*n].anno), &(vet[*n].mese),
                                 & (vet[*n].giorno),
                        &(vet[*n].ore), &(vet[*n].min),
                        &(vet[*n].sec), &(vet[*n].msec), (vet[*n].id),
                        &(vet[*n].temp), &(vet[*n].umid), &(vet[*n].veloc));
        if (n_conv != 11)
            continue;
        (*n)++;
        if ((*n) >= size)
        {
            size *= 2;
            vet = realloc(vet, sizeof(*vet) * size);
            if (vet == NULL)
            {
                free(vet);
                return NULL;
            }
        }
    }
    vet = realloc(vet, sizeof(*vet) * (*n));
    return vet;
}
void stampa_id(struct misura *vet, int n)
{
    int i;
    for (i = n - 1; i >= 0; i--)
        printf("%s\n", (vet + i)->id);
}
int main(int argc, char *argv[])
{
    FILE *fp;
    struct misura *vet = {0};
    int n;
    if (argc != 2)
    {
        return 1;
        fprintf(stderr, "#numero parametri sbagliato\n");
    }
    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        return 1;
        fprintf(stderr, "#Errore di apertura del file\n");
    }
    vet = lettura_file(fp, &n);
    fclose(fp);
    printf("[IDENTIFICATIVI]\n");
    stampa_id(vet, n);
    free(vet);
    return 0;
}