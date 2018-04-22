#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
typedef struct{
	double delay_prob;
	double lost_prob;
  double avg_delay;
  double total_delay;
  double remaining;
} resultados;

int main(void)
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    double epsilon = 0.20;
    double epsilon1 = 0.01;
    double epsilon2 = 30;
    double epsilon3 = 60;
    int foundm, foundl, foundmi;

    resultados * results = (resultados *)malloc(sizeof(resultados));
    int matchs = 0;

    fp = fopen("database.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    while ((read = getline(&line, &len, fp)) != -1) {
        char *token = strtok(line, ";");
        int m, l ,mi, counter = 0;
        double a,b,c,d;
        for(int i = 0 ; i < 7 ; i++)
        {
          if(i == 0)
            m = atoi(token);
          else if(i == 1)
            l = atoi(token);
          else if(i == 2)
            mi = atoi(token);
          else if(i == 3)
            sscanf(token, "%lf", &a);
          else if(i == 4)
            sscanf(token, "%lf", &b);
          else if(i == 5)
            sscanf(token, "%lf", &c);
          else if(i == 6)
            sscanf(token, "%lf", &d);
          token = strtok(NULL, ";");
        }
        if(a <= epsilon) counter++;
        if(b <= epsilon1) counter++;
        if(c <= epsilon2) counter++;
        if(d <= epsilon3) counter++;
        if(counter == 3){
          if(matchs == 0){
            foundm = m;
            foundl = l;
            foundmi = mi;
            results -> delay_prob = a;
            results -> lost_prob = b;
            results -> avg_delay = c;
            results -> total_delay = d;
            if( a <= epsilon ) a = 0;
            if( b <= epsilon ) b = 0;
            if( c <= epsilon ) c = 0;
            if( d <= epsilon ) d = 0;
            results -> remaining = (100 * a) + (100 * b) + c + d;
          }
          else {
            double a1 = a;
            double b1 = b;
            double c1 = c;
            double d1 = d;
            if( a <= epsilon ) a1 = 0;
            if( b <= epsilon ) b1 = 0;
            if( c <= epsilon ) c1 = 0;
            if( d <= epsilon ) d1 = 0;
            double remaining = (100 * a1) + (100 * b1) + c1 + d1;
            if( remaining < results -> remaining ){
              foundm = m;
              foundl = l;
              foundmi = mi;
              results -> delay_prob = a;
              results -> lost_prob = b;
              results -> avg_delay = c;
              results -> total_delay = d;
              results -> remaining = remaining;
            }
          }
          matchs++;
        }
        else if(counter == 4){
          printf("With {m = %d ; L = %d ; mi = %d} PERFECT MATCH!!\n", m , l , mi);
          printf("Delay calls prob. at prot.: %f\n", a);
          printf("Lost calls prob: %f\n", b);
          printf("Avg. delay Prot: %f\n", c);
          printf("Avg. delay Total: %f\n", d);
          printf("---------------------\n\n");
          matchs++;
          break;
        }

    }
    printf("With {m = %d ; L = %d ; mi = %d}\n", foundm , foundl , foundmi);
    printf("Delay calls prob. at prot.: %f\n", results -> delay_prob);
    printf("Lost calls prob: %f\n", results -> lost_prob);
    printf("Avg. delay Prot: %f\n", results -> avg_delay);
    printf("Avg. delay Total: %f\n", results -> total_delay);
    printf("---------------------\n\n");
    printf("---------------------\n\nFound %d matchs!\n", matchs);
    fclose(fp);
    free(results);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}
