#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int toB64(char c);
unsigned long decToBin(int dec);
int binToDec(unsigned long bin);
int pot2(int n);

int main(int argc, char **argv) {

    if (argc < 2) {
        exit(0);
    }
    
    char *lzw = argv[1];

    FILE *f = fopen(lzw, "r");

    if (f == NULL) {
        exit(0);
    }

    fscanf(f, "%s", lzw);
    
    int c, l;
    fscanf(f, "%d %d", &c, &l);
    char *codigo;

    codigo = (char *) malloc(sizeof(char) * c * l * 2);
    fscanf(f, "%s", codigo);

    while (fscanf(f, "%s", lzw) != EOF) {
        strcat(codigo, lzw);
    }

    int img[l * c];
    int t = strlen(codigo);
    printf("%d\n", t);
    unsigned long ax, ay;
    for (int i = 0, j = 0; i < t; i += 2, j++) {
        ax = decToBin(toB64(codigo[i])) * 1000000;
        ay = decToBin(toB64(codigo[i + 1]));   
        img[j] = binToDec(ax + ay);
        //printf("%d\n", img[j]);
    }
    
    fclose(f);

    f = fopen("saida.pgm", "w+");

    fprintf(f, "%s\n", "P2");
    fprintf(f, "%d %d\n", c, l);
    fprintf(f, "%d\n", 4095);
    //fprintf(f, "%s", codigo);
    for (int i = 0; i < l * c; i++) {
        fprintf(f, "%d ", img[i]);
    }

    fclose(f);

    return 0;

}

int toB64(char c) {
    if (c > 64 && c < 91) {
        return c - 65;
    }

    if (c > 96 && c < 123) {
        return c - 97 + 26;
    }

    if (c == '+') {
        return 62;
    }

    if (c == '/') {
        return 63;
    }
}

unsigned long decToBin(int dec) {
    int bin = 0;
    int fator = 1;
    int r;

    do {
        r = dec % 2;

        if (r == 1) {
            bin += fator;
        } 
        
        fator *= 10;
        dec /= 2;
    } while (dec > 0);

    return bin;
}

int binToDec(unsigned long bin) {
    int dec = 0;
    int pot = 0;
    int resto;

    while (bin > 0) {
        resto = bin % 10;
        dec += resto * pot2(pot);
        pot++;
        bin /= 10;
    }

    return dec;
}

int pot2(int n) {
    int p = 1;
    int i;

    for (i = 0; i < n; i++) {
        p *= 2;
    }

    return p;
}