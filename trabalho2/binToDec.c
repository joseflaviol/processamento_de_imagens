#include <stdio.h>
#include <stdlib.h>

int binToDec(int bin);
int pot2(int n);

int main() {

    int bin = 10101010;

    printf("%d\n", binToDec(bin));

    return 0;

}

int binToDec(int bin) {
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