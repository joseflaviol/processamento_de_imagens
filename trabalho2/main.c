#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_DICT 4096
#define NUM_SIMB 256

typedef unsigned short int ui16;

typedef struct alocaInfo {
    ui16 *base;
    int tam;
    ui16 *proxAloca;
} alocaInfo;

int *pixels;

int toB64(char c);
unsigned long long decToBin(int dec);
ui16 binToDec(unsigned long long bin);
int pot2(int n);
void iniciaAloca(alocaInfo *aloc, int tam);
ui16 *aloca(alocaInfo *aloc, int len);
void decodifica(ui16 *in, int n);

int main(int argc, char **argv) {

    int i, j, c, l, t;
    ui16 *img;
    unsigned long long ax, ay;
    char aux;
    char *lzw, *codigo;

    if (argc < 2) {
        exit(0);
    }
    
    lzw = argv[1];

    FILE *f = fopen(lzw, "r");

    if (f == NULL) {
        exit(0);
    }

    fscanf(f, "%s", lzw);
    
    fscanf(f, "%d %d", &c, &l);

    codigo = (char *) malloc(sizeof(char) * c * l * 2);
    
    i = 0;
    while (fscanf(f, "%c", &aux) != EOF) {
        if ((aux > 64 && aux < 91) ||
            (aux > 96 && aux < 123) ||
            (aux > 47 && aux < 58) ||
            aux == '+' ||
            aux == '/') {
            codigo[i++] = aux;
        } 
    }

    codigo[i] = '\0';

    t = strlen(codigo);
    printf("%d\n", t);

    img = (ui16 *) malloc(sizeof(ui16) * (t / 2));
    pixels = (int *) malloc(sizeof(int) * c * l);
    
    for (i = 0, j = 0; i < t; i += 2, j++) {
        ax = decToBin(toB64(codigo[i])) * 1000000;
        ay = decToBin(toB64(codigo[i + 1]));   
        img[j] = binToDec(ax + ay);
    }
  
    fclose(f);

    decodifica(img, t / 2);

    f = fopen("saida.pgm", "w+");

    fprintf(f, "%s\n", "P2");
    fprintf(f, "%d %d\n", c, l);
    fprintf(f, "%d\n", 255);
    
    for (i = 0; i < l * c; i++) {
        fprintf(f, "%d ", pixels[i]);
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

    if (c > 47 && c < 58) {
        return c - 48 + 52;
    }

    if (c == '+') {
        return 62;
    }

    if (c == '/') {
        return 63;
    }
}

unsigned long long decToBin(int dec) {
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

ui16 binToDec(unsigned long long bin) {
    ui16 dec = 0;
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

void iniciaAloca(alocaInfo *aloc, int tam) {
    aloc->base = malloc(tam);
    aloc->tam = tam;
    aloc->proxAloca = aloc->base;
}

ui16 *aloca(alocaInfo *aloc, int len) {
    ui16 *ret = aloc->proxAloca;
    aloc->proxAloca += len;
    return ret;
}

void decodifica(ui16 *in, int n) {
    struct {
        ui16 *seq;
        int tam;
    } dicionario[TAM_DICT];

    alocaInfo aInfo;
    ui16 *marca;
    int posDict;
    int anterior;
    int i;
    int auxP = 0;

    iniciaAloca(&aInfo, TAM_DICT * TAM_DICT * sizeof(ui16));
    marca = aInfo.proxAloca;

    for (int i = 0; i < NUM_SIMB; i++) {
        dicionario[i].seq = aloca(&aInfo, 1);
        dicionario[i].seq[0] = i;
        dicionario[i].tam = 1;
    }

    posDict = NUM_SIMB;

    anterior = in[0];
    pixels[auxP++] = anterior;
    i = 1;

    while (i < n) {
        int simbolo = in[i++];
        if (posDict == TAM_DICT) {
            aInfo.proxAloca = marca;

            for (int j = 0; j < NUM_SIMB; j++) {
                dicionario[j].seq = aloca(&aInfo, 1);
                dicionario[j].seq[0] = j;
                dicionario[j].tam = 1;
            }

            posDict = NUM_SIMB;
        } else {
            int tam = dicionario[anterior].tam;
            
            dicionario[posDict].tam = tam + 1;
            dicionario[posDict].seq = aloca(&aInfo, tam + 1);

            for (int k = 0; k < tam; k++) {
                dicionario[posDict].seq[k] = dicionario[anterior].seq[k];
            }

            if (simbolo == posDict) {
                dicionario[posDict++].seq[tam] = dicionario[anterior].seq[0];
            } else {
                dicionario[posDict++].seq[tam] = dicionario[simbolo].seq[0];
            }
        }

        for (int k = 0; k < dicionario[simbolo].tam; k++) {
            pixels[auxP++] = dicionario[simbolo].seq[k];
        }

        anterior = simbolo;
    }
    free(aInfo.base);
}