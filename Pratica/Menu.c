#include <stdio.h>

void doOperation(int scelta) {
    switch (scelta) {
        case 1:
            printf("Hai scelto l'operazione 1");
            break;
        case 2:
            printf("Hai scelto l'operazione 2");
            break;
        case 0:
            printf("Uscita dal programma");
            break;
        default:
            printf("Scelta non valida!");
    }
}

int main() {
    int scelta;
    do {
        printf("--- MENU ---");
        printf("1 - Operazione 1");
        printf("2 - Operazione 2");
        printf("0 - Esci");
        printf("Scegli: ");
        scanf("%d", &scelta);

        doOperation(scelta);
    } while (scelta != 0);
    return 0;
}
