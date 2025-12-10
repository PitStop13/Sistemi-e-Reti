#include <stdio.h>
#include <stdlib.h>
#include <string.h>   // aggiunto per memset (facoltativo ma utile)


void read_buffer(FILE* input, int pos, int count, unsigned char* dest)
{
    fseek(input, pos, SEEK_SET);
    fread(dest, 1, count, input);   // già corretto nel tuo commento
}


void read_string(FILE* input, int pos, int count, unsigned char* dest)
{
    read_buffer(input, pos, count, dest);
    dest[count] = '\0';              // termina la stringa
}


unsigned long read_Number(FILE* input, int pos, int count)
{
    unsigned char buffer[8];         // max 8 byte per unsigned long (64 bit)
    unsigned long ret = 0;


    // sicurezza: count non deve superare la dimensione del buffer
    if (count > sizeof(buffer)) count = sizeof(buffer);


    read_buffer(input, pos, count, buffer);


    // ricostruzione little-endian
    for (int a = 0; a < count; a++)
    {
        ret |= ((unsigned long)buffer[a]) << (a * 8);
    }
    return ret;
}


int main()
{
    unsigned char nome_del_file[9] = {0};   // 8 caratteri + '\0' → inizializzato
    unsigned long bytes_per_settore = 0;
    unsigned long bytes_per_cluster = 0;
    unsigned long numero_setori_riservato = 0;
    unsigned long inizo_area_fat = 0;
    unsigned long numero_fat = 0;
    FILE* file = fopen("fat", "rb");
    if (file == NULL) {
        perror("Errore apertura file");
        return 1;
    }

    // Legge il campo OEM Name (offset 0x03, 8 byte) tipico dei volumi FAT12/16/32
    read_string(file, 0x03, 8, nome_del_file);

    bytes_per_settore = read_Number(file, 0x0B, 2);
    bytes_per_cluster=bytes_per_settore*read_Number(file,0x0d,1);
    numero_setori_riservato=read_Number(file,0x0e,2);
    inizo_area_fat=numero_setori_riservato*bytes_per_settore;
    numero_fat=read_Number(file,0x010,1);
    printf("Nome OEM letto: \"%s\"\n", nome_del_file);
    printf("Bytes per settore: %lu\n", bytes_per_settore);
    printf("Bytes per cluster: %lu\n", bytes_per_cluster);
    printf("settori risevati: %lu\n", numero_setori_riservato);
    printf("area fat: %lu\n", inizo_area_fat);
    printf("numero fat: %lu\n", numero_fat);

    fclose(file);
    return 0;
}




