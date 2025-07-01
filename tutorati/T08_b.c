#include <stdio.h>
#define NMAX 168
struct misura
{
    int anno, mese, giorno, ore, min, sec, msec, umid;
    char id[11];
    float temp, veloc;
};
int notturno(int ore)
{
    if (ore >= 22 || ore < 6)
        return 1;
    return 0;
}
void lettura_file(FILE *fp, struct misura *vet, int *n)
{
    int n_conv;
    char buf[1024];
    *n = 0;
    while (fgets(buf, sizeof(buf), fp))
    {
        n_conv = sscanf(buf, "%d-%d-%d %d:%d:%d.%d %s %f %d%%%f", &(vet[*n].anno), &(vet[*n].mese),
                        &(vet[*n].giorno), &(vet[*n].ore), &(vet[*n].min),
                        &(vet[*n].sec), &(vet[*n].msec), (vet[*n].id),
                        &(vet[*n].temp), &(vet[*n].umid), &(vet[*n].veloc));
        if (n_conv != 11)
            continue;
        (*n)++;
        if (*n >= NMAX)
            break;
    }
}
void stampa_riga(struct misura *vet)
{
    printf("%.4d-%.2d-%.2d %.2d:%.2d:%.2d.%.3d %s %.1f %d%%%.1f\n", vet[0].anno, vet[0].mese,
           vet[0].giorno, vet[0].ore, vet[0].min,
           vet[0].sec, vet[0].msec, vet[0].id,
           vet[0].temp, vet[0].umid, vet[0].veloc);
}
void invertita(struct misura *vet, int n)
{
    int i;
    if (n <= 6)
    {
        for (i = n - 1; i >= 0; i--)
            stampa_riga(vet + i);
    }
    else
    {
        for (i = 0; i < 3; i++)
            stampa_riga(vet + i);
        for (i = n - 1; i >= n - 3; i--)
            stampa_riga(vet + i);
    }
}
void temp_max(struct misura *vet, int n)
{
    float max = vet[0].temp;
    int i;
    for (i = 1; i < n; i++)
    {
        if (vet[i].temp > max)
            max = vet[i].temp;
    }
    printf("%f\n", max);
}
int main(int argc, char *argv[])
{
    FILE *fp;
    struct misura vet[NMAX];
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
    lettura_file(fp, vet, &n);
    printf("[INVERSO]\n");
    invertita(vet, n);
    printf("[MAX-TEMP]\n");
    temp_max(vet, n);
    fclose(fp);
    return 0;
}