#include <stdio.h>
#include <stdlib.h>
#include "image.c"

void diminui(Imagem *imagem, int fator);
void mudaEscala(Imagem *imagem, int novaEscala);
void teste(Imagem *img);

int main() {
    Imagem *img;
    double fator;

    alocaImagem(&img);
    carregaImagem(img, "exemplos/dragon.pgm");
    fator = (img->numeroColunas) / 100.0;
    diminui(img, fator);
    mudaEscala(img, 6);
    salvaImagem(img, "img_modificada.pgm");
    teste(img);

    return 0;
}

void diminui(Imagem *imagem, int fator) {
    int   i, j, k, nc, nl;
    int      *novosPixels;

    nc = imagem->numeroColunas / fator;
    nl = imagem->numeroLinhas  / fator;

    novosPixels = (int *) malloc(sizeof(int) * nc * nl);
    
    for (i = 0, j = fator, k = fator; i < nc * nl; i++, j += fator) {
        if (imagem->numeroColunas * k + j >= imagem->numeroColunas * imagem->numeroLinhas) {
            novosPixels[i] = novosPixels[i - nc];
        } else {
            novosPixels[i] = j >= imagem->numeroColunas ? novosPixels[i - 1] : imagem->pixels[imagem->numeroColunas * k + j];
        }
        if ((i + 1) % nc == 0) {
            j = 0;
            k += fator;
        }    
    }

    imagem->numeroColunas =   nc;
    imagem->numeroLinhas  =   nl;
    imagem->pixels = novosPixels;
}

void mudaEscala(Imagem *imagem, int novaEscala) {
    int i;

    for (i = 0; i < imagem->numeroLinhas * imagem->numeroColunas; i++) {
        int pct = (imagem->pixels[i] * 100) / imagem->tonsDeCinza;
        imagem->pixels[i] = (pct * novaEscala) / 100;
    }

    imagem->tonsDeCinza = novaEscala;
}

void teste(Imagem *img) {
    int i, j, k, l, cont;
    Imagem **dados;
    dados = malloc(sizeof(Imagem *) * 7);
    char nomeDado[30];

    for (i = 0; i < 7; i++) {
        alocaImagem(&dados[i]);
        sprintf(nomeDado, "./dados_img/dado-%d.pgm", i);
        carregaImagem(dados[i], nomeDado);
    }

    int *pixelsNovaImagem;

    pixelsNovaImagem = (int *) malloc(sizeof(int) * (img->numeroColunas * 40) * (img->numeroLinhas * 40));

    cont = 0;
    for (i = 0; i < img->numeroLinhas; i++) {
        for (j = 0; j < 40; j++) {
            for (k = 0; k < img->numeroColunas; k++) {
                for (l = 0; l < 40; l++) {
                    pixelsNovaImagem[cont++] = dados[img->pixels[i * img->numeroColunas + k]]->pixels[j * 40 + l];
                }
            }
        }
    }

    Imagem *novaImagem;
    alocaImagem(&novaImagem);
    criaImagem(novaImagem, "P2", img->numeroColunas * 40, img->numeroLinhas * 40, 255, pixelsNovaImagem);
    salvaImagem(novaImagem, "novaimagem.pgm"); 

}