#include <stdio.h>


void read_buffer(FILE *input, int pos, int count, unsigned char *dest)
{
    fseek(input, pos, SEEK_SET);
    fread(dest, 1, count, input);
}


void read_string(FILE *input, int pos, int count, unsigned char *dest)
{
    read_buffer(input, pos, count, dest);
    dest[count] = '\0';
}


unsigned long read_number(FILE *input, int pos, int count)
{
    unsigned long ret = 0;


    // Questa implementazione funziona per Little Endian leggendo i byte a ritroso
    unsigned char buffer[count];
    read_buffer(input, pos, count, buffer);


    for (int a = count - 1; a >= 0; a--)
    {
        ret = ret << 8;
        ret += buffer[a];
    }
    return ret;
}


int main()
{
    unsigned char nome_del_filesystem[9];
    unsigned long byte_per_settore = 0;
    unsigned long byte_per_cluster = 0;
    unsigned long numero_settori_riservati = 0;
    unsigned long inizio_area_fat = 0;
    unsigned long numero_fat = 0;
    unsigned long numero_righe_dir = 0;
    unsigned long bytes_per_fat = 0;
    unsigned long inizio_root_dir = 0;
    unsigned long inizio_area_dati = 0;
    unsigned long dimensione_disco = 0;


    FILE *file_system = fopen("fat", "rb");


    if (file_system == NULL)
    {
        perror("Errore nell'apertura del file 'fat'");
        return 1;
    }


    read_string(file_system, 0x03, 8, nome_del_filesystem);
    byte_per_settore = read_number(file_system, 0x0b, 2);
    byte_per_cluster = byte_per_settore * read_number(file_system, 0x0d, 1);
    numero_settori_riservati = read_number(file_system, 0x0e, 2);
    inizio_area_fat = numero_settori_riservati * byte_per_settore;
    numero_fat = read_number(file_system, 0x10, 1);
    numero_righe_dir = read_number(file_system, 0x11, 2);
    bytes_per_fat = byte_per_settore * read_number(file_system, 0x16, 2);
    inizio_root_dir = inizio_area_fat + bytes_per_fat * numero_fat;
    inizio_area_dati = inizio_root_dir + 32 * numero_righe_dir;
    dimensione_disco = byte_per_settore * read_number(file_system, 0x20, 4);


    printf("nome del file system: %s\n", nome_del_filesystem);
    printf("numero byte per settore: %lu\n", byte_per_settore);
    printf("numero byte per cluster: %lu\n", byte_per_cluster);
    printf("numero byte per fat: %lu\n", bytes_per_fat);
    printf("numero settori riservati: %lu\n", numero_settori_riservati);
    printf("ininzio area fat: 0x%lx\n", inizio_area_fat);
    printf("numero fat: %lu\n", numero_fat);
    printf("numero righe root directory: %lu\n", numero_righe_dir);
    printf("inizio root directory: 0x%lx\n", inizio_root_dir);
    printf("inizio area dati: 0x%lx\n", inizio_area_dati);
    printf("dimensione del disco: %lu\n", dimensione_disco);


    for (int f = 0; f < numero_righe_dir; f++)
    {
        unsigned long inizio_riga = inizio_root_dir + f * 32;


        unsigned char nome[9];
        unsigned char estensione[4];


        unsigned char attribute[3];
        unsigned char sola_letture = 0, nascosto = 0,
                      sistema = 0, sottodirectory = 0, archivio = 0;


        unsigned long orario_creazione = 0;
        unsigned long ora_creazione = 0;
        unsigned long minuti_creazione = 0;
        unsigned long secondi_creazione = 0;
        unsigned long millisecondi_creazione = 0;


        unsigned long data_creazione = 0;
        unsigned long giorno_creazione = 0;
        unsigned long mese_creazione = 0;
        unsigned long anno_creazione = 0;


        unsigned long dimensione = 0;
        unsigned long primo_cluster = 0;


        read_string(file_system, inizio_riga, 8, nome);


        if (nome[0] == 0 || nome[0] == 0xe5)
            continue;


        read_string(file_system, inizio_riga + 0x08, 3, estensione);


        read_buffer(file_system, inizio_riga + 0x0b, 1, attribute);


        sola_letture = attribute[0] & 0x01;
        nascosto = attribute[0] & 0x02;
        sistema = attribute[0] & 0x04;
        sottodirectory = attribute[0] & 0x10;
        archivio = attribute[0] & 0x20;


        printf("\tfile %s.%s\n", nome, estensione);


        if (sola_letture)
            printf("\t\tsola lettura\n");
        if (nascosto)
            printf("\t\tnascosto\n");
        if (sistema)
            printf("\t\tsistema\n");
        if (sottodirectory)
            printf("\t\tsottodirectory\n");
        if (archivio)
            printf("\t\tarchivio\n");


        // FAT 16  HEPOX
        // millisecondo presenti solo in creazione e non in modifica secondi dispari presenti solo creazione e non in modifche
        // file con modifica in secondi dispari --> problema pk non e presente un bit per i milli secondi
        // ambito unix si utilizza il 1970 pk prima non erano presenti i file prima di quella data


        orario_creazione = read_number(file_system, inizio_riga + 0x0e, 2);
        ora_creazione = orario_creazione >> 11;
        minuti_creazione = (orario_creazione >> 5) & 0x3f;
        secondi_creazione = (orario_creazione & 0x1f) * 2;
        millisecondi_creazione = read_number(file_system, inizio_riga + 0x0d, 1) * 10;


        if (millisecondi_creazione >= 1000)
        {
            millisecondi_creazione -= 1000;
            secondi_creazione++;
        }


        printf("\t\tOra di creazione: %lu:%lu:%lu:%lu\n", ora_creazione, minuti_creazione, secondi_creazione, millisecondi_creazione);


        data_creazione = read_number(file_system, inizio_riga + 0x10, 2);
        giorno_creazione = data_creazione & 0x1f;
        mese_creazione = (data_creazione >> 5) & 0x0f;
        anno_creazione = ((data_creazione >> 9) & 0x7f) + 1980;


        printf("\t\tData di creazione: %lu:%lu:%lu\n", giorno_creazione, mese_creazione, anno_creazione);


        // cluster 0 e 1 non esistono per socorerli parto dal 2
        dimensione = read_number(file_system, inizio_riga + 0x1c, 4);
        primo_cluster = read_number(file_system, inizio_riga + 0x1a, 2);


        printf("\t\tDimensione: %lu byte\n", dimensione);
        printf("\t\tPrimo cluster: %lu\n", primo_cluster);


        if (archivio)
        {
            unsigned char contenuto[1 + dimensione];
            unsigned long posizione = inizio_area_dati + (primo_cluster - 2) * byte_per_cluster;
            read_string(file_system, posizione, dimensione, contenuto);
            printf("\t\tContenuto: %s\n", contenuto);
        }
    }
    fclose(file_system);


    return 0;
}
