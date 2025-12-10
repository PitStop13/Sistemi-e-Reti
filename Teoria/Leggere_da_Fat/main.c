#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Funzioni per la lettura (invariate)

void read_string_simple(FILE * input, long pos, size_t count, unsigned char* dest)
{
    if (fseek(input, pos, SEEK_SET) != 0)
    {
        perror("Errore durante fseek (read_string_simple)");
        return;
    }
    size_t bytes_read = fread(dest, 1, count, input);
    if (count > 0) {
        dest[count] = '\0';
    }
    if (bytes_read != count)
    {
        fprintf(stderr, "Avviso: Letti solo %zu byte invece dei %zu attesi.\n", bytes_read, count);
    }
}

unsigned long read_word_little_endian(FILE* input, long pos)
{
    unsigned char buffer[2];
    if (fseek(input, pos, SEEK_SET) != 0)
    {
        perror("Errore durante fseek (read_word_little_endian)");
        return 0;
    }
    if (fread(buffer, 1, 2, input) != 2)
    {
        // Se si verifica un errore di lettura o lettura incompleta
        return 0;
    }
    // LSB + (MSB << 8)
    unsigned long value = (unsigned long)buffer[0] | ((unsigned long)buffer[1] << 8);
    return value;
}

unsigned char read_byte(FILE* input, long pos)
{
    unsigned char value = 0;
    if (fseek(input, pos, SEEK_SET) != 0)
    {
        perror("Errore durante fseek (read_byte)");
        return 0;
    }
    if (fread(&value, 1, 1, input) != 1)
    {
        return 0;
    }
    return value;
}


int main()
{
    unsigned char nome_del_filesystem [8 + 1];
    FILE* fileSystem = fopen("fat", "rb");

    if (fileSystem == NULL)
    {
        perror("Errore nell'apertura del file 'fat'");
        return 1;
    }

    printf("--- FAT Boot Sector (BPB) Dati Fondamentali ---\n");

    // 1. Nome File System (0x03, 8 byte)
    read_string_simple(fileSystem, 0x03L, 8, nome_del_filesystem);
    printf("Nome del file system (BS_OEMName): %s\n", nome_del_filesystem);

    // 2. Bytes per Sector (0x0B, 2 byte)
    unsigned long bytes_per_sector = read_word_little_endian(fileSystem, 0x0BL);
    printf("Bytes per Sector (BPB_BytsPerSec): %lu\n", bytes_per_sector);

    // 3. Sectors per Cluster (0x0D, 1 byte)
    unsigned char sectors_per_cluster = read_byte(fileSystem, 0x0DL);
    printf("Sectors per Cluster (BPB_SecPerClus): %u\n", sectors_per_cluster);

    // 4. Reserved Sector Count (0x0E, 2 byte)
    unsigned long reserved_sector_count = read_word_little_endian(fileSystem, 0x0EL);
    printf("Reserved Sector Count (BPB_RsvdSecCnt): %lu\n", reserved_sector_count);

    printf("------------------------------------------------\n");

    // 5. NUOVA LETTURA: Numero di FAT (0x10, 1 byte)
    unsigned char num_fats = read_byte(fileSystem, 0x10L);
    printf("Numero di FAT (BPB_NumFATs): %u\n", num_fats);

    // 6. NUOVA LETTURA: Root Entry Count (0x11, 2 byte)
    unsigned long root_ent_cnt = read_word_little_endian(fileSystem, 0x11L);
    printf("Max Root Directory Entries (BPB_RootEntCnt): %lu\n", root_ent_cnt);

    // 7. NUOVA LETTURA: Total Sectors 16-bit (0x13, 2 byte)
    unsigned long tot_sec_16 = read_word_little_endian(fileSystem, 0x13L);
    printf("Total Sectors (16-bit) (BPB_TotSec16): %lu\n", tot_sec_16);

    // 8. NUOVA LETTURA: Media Descriptor (0x15, 1 byte)
    unsigned char media_desc = read_byte(fileSystem, 0x15L);
    printf("Media Descriptor (BPB_Media): 0x%02X\n", media_desc);

    printf("------------------------------------------------\n");

    // Chiusura del file
    if (fclose(fileSystem) == EOF)
    {
        perror("Errore nella chiusura del file");
        return 1;
    }

    return 0;
}
