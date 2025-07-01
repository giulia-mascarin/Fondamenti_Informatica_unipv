#include <stdio.h>
#define MAX 100
#define VOTOMIN 18
#define VOTOMAX 30
#define NVOTI (VOTOMAX - VOTOMIN + 1)
void leggi_valori(int *v, int n)
{
    int i = 0;
    int voto;
    while (i < n)
    {
        printf("Inserire il voto: ");
        scanf("%d", &voto);
        if (voto >= VOTOMIN && voto <= VOTOMAX)
        {
            v[i] = voto;
            i++;
        }
    }
}
void stampa_valori(int *v, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("%d\n", v[i]);
    }
}
void freq(int *v, int n, int *f)
{
    int i;
    for (i = 0; i < n; i++)
        f[v[i] - VOTOMIN]++;
}
int minimo(int *v, int n)
{
    int i;
    int min = v[0];
    for (i = 1; i < n; i++)
    {
        if (v[i] < min)
        {
            min = v[i];
        }
    }
    return min;
}
int max_indice(int *f, int n)
{
    int i;
    int max = 0;
    for (i = 1; i < n; i++)
    {
        if (f[i] > f[max])
        {
            max = i;
        }
    }
    return max;
}
int main()
{
    int vet[MAX], n;
    printf("Inserire il numero di voti: ");
    scanf("%d", &n);
    if (n <= 0 || n > 100)
    {
        printf("[RISULTATO]\nerrore\n");
        return 0;
    }
    leggi_valori(vet, n);
    printf("[VALORI]\n");
    stampa_valori(vet, n);
    int min;
    min = minimo(vet, n);
    printf("[MINIMO]\n%d\n", min);
    // printf("[MINIMO]\n%d\n", minimo(vet, n) );
    int frequenza[NVOTI] = {0};
    freq(vet, n, frequenza);
    printf("[FREQUENZE]\n");
    stampa_valori(frequenza, NVOTI);
    int max = max_indice(frequenza, NVOTI);
    printf("[MAXFREQ]\n%d\n", VOTOMIN + max);
    return 0;
}