#include <stdio.h>
int notturno(int ore)
{
    if (ore >= 22 || ore < 6)
        return 1;
    return 0;
}
void lettura_file(FILE *fp)
{
    int anno, mese, giorno, ore, min, sec, msec, umid, n_conv;
    int n = 0;
    float temp_max = -300;
    char id[11], buf[1024];
    float temp, veloc;
    printf("[NOTTURNO]\n");
    while (fgets(buf, sizeof(buf), fp))
    {
        n_conv = sscanf(buf, "%d-%d-%d %d:%d:%d.%d %s %f %d%%%f", &anno, &mese, &giorno, &ore, &min,
                        &sec, &msec, id, &temp, &umid, &veloc);
        if (n_conv != 11)
            continue;
        if (notturno(ore))
            printf("%s\n", buf);
        if (temp > temp_max)
            temp_max = temp;
        n++;
    }
    printf("[MAX-TEMP]\n%f\n", temp_max);
    printf("[RIGHE]\n%d\n", n);
}
int main(int argc, char *argv[])
{
    FILE *fp;
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
    lettura_file(fp);
    fclose(fp);
    return 0;
}