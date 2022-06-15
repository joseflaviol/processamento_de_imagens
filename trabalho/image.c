#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char          *tipo;
    int   numeroColunas;
    int    numeroLinhas;
    int     tonsDeCinza;
    int         *pixels;
} Imagem;

void alocaImagem(Imagem **imagem);
void carregaImagem(Imagem *imagem, char *nomeArquivo);
void criaImagem(Imagem *Imagem, char *tipo, int numeroColunas, int numeroLinhas, int tonsDeCinza, int *pixels);
void salvaImagem(Imagem *imagem, char *nomeArquivo);
void negativa(Imagem *imagem);

/*int main() {
    Imagem *imagem;

    alocaImagem(&imagem);
    carregaImagem(imagem, "fractal_tree.pgm");
    negativa(imagem);
    salvaImagem(imagem, "novaimagem.pgm");

    return 0;
}*/

void alocaImagem(Imagem **imagem) {
    *imagem = malloc(sizeof(Imagem));
    (*imagem)->tipo = malloc(sizeof(char) * 5);
}

void criaImagem(Imagem *imagem, char *tipo, int numeroColunas, int numeroLinhas, int tonsDeCinza, int *pixels) {
    imagem->tipo = tipo;
    imagem->numeroColunas = numeroColunas;
    imagem->numeroLinhas = numeroLinhas;
    imagem->tonsDeCinza = tonsDeCinza;
    imagem->pixels = pixels;

    printf("%s %d %d %d\n" , imagem->tipo, imagem->numeroColunas, imagem->numeroLinhas, imagem->tonsDeCinza);
}

void carregaImagem(Imagem *imagem, char *nomeArquivo) {
    int i;
    FILE *arquivoImagem;
    char tipo[5];

    arquivoImagem = fopen(nomeArquivo, "r");

    if (!arquivoImagem) {
        printf("Erro nao carregar imagem. Programa finalizado.\n");
        exit(1);
    }

    fgets(imagem->tipo, 5, arquivoImagem);
    imagem->tipo[strlen(imagem->tipo) - 1] = '\0';

    if (imagem->tipo[0] != 'P' && (imagem->tipo[1] < '1' || imagem->tipo[1] > '3')) {
        printf("Erro nao carregar imagem. Programa finalizado.\n");
        exit(1);
    }

    fscanf(arquivoImagem, "%d %d", &imagem->numeroColunas, &imagem->numeroLinhas);

    printf("%d %d\n", imagem->numeroColunas, imagem->numeroLinhas);

    imagem->pixels = (int *) malloc(sizeof(int) * imagem->numeroColunas * imagem->numeroLinhas);
    
    fscanf(arquivoImagem, "%d", &imagem->tonsDeCinza);

    for (i = 0; i < imagem->numeroColunas * imagem->numeroLinhas; i++) {
        fscanf(arquivoImagem, "%d", &imagem->pixels[i]);
    }

    fclose(arquivoImagem); 
}

void salvaImagem(Imagem *imagem, char *nomeArquivo) {
    int i;
    FILE *arquivoImagem;

    arquivoImagem = fopen(nomeArquivo, "w+");
    //printf("%s %d %d %d\n" , imagem->tipo, imagem->numeroColunas, imagem->numeroLinhas, imagem->tonsDeCinza);
    fprintf(arquivoImagem, "%s\n", imagem->tipo);
    fprintf(arquivoImagem, "%d %d\n", imagem->numeroColunas, imagem->numeroLinhas);
    fprintf(arquivoImagem, "%d\n", imagem->tonsDeCinza);

    for (i = 0; i < imagem->numeroColunas * imagem->numeroLinhas; i++) {
        fprintf(arquivoImagem, "%3d ", imagem->pixels[i]);
    }

    fclose(arquivoImagem);
}

void negativa(Imagem *imagem) {
    int i;

    for (i = 0; i < imagem->numeroColunas * imagem->numeroLinhas; i++) {
        imagem->pixels[i] = imagem->tonsDeCinza - imagem->pixels[i];
    }
}