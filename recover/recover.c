#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 512

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return (1);
    }

    FILE *raw = fopen(argv[1], "r");
    if (raw == NULL)
    {
        printf("File '%s' couldn't be opened\n", argv[1]);
        return (1);
    }

    int flagFound = 0;
    int count = 0;
    char jpeg_name[8];
    FILE *output = NULL;
    BYTE *buffer = malloc(BLOCK_SIZE * sizeof(BYTE));

    while (fread(buffer, BLOCK_SIZE, 1, raw) == 1)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (flagFound == 1)
                fclose(output);
            else
                flagFound = 1;

            sprintf(jpeg_name, "%03i.jpg", count++);
            output = fopen(jpeg_name, "w");
            if (output == NULL)
            {
                printf("Could not create file '%s'\n", jpeg_name);
                fclose(raw);
                free(buffer);
                return (3);
            }
        }

        if (flagFound == 1)
            fwrite(buffer, BLOCK_SIZE, 1, output);
    }

    fclose(raw);

    if (flagFound == 1)
        fclose(output);

    free(buffer);
    return (0);
}
