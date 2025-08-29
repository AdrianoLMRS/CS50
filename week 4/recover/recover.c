#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define BLOCK_SIZE 512
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover <.raw file>\n");
        return 1;
    }

    char *infile = argv[1];

    // Memory card file
    FILE *inptr = fopen(infile, "rb");
    if (inptr == NULL)
    {
        printf("Could not open file\n");
        return 1;
    }

    BYTE buffer[BLOCK_SIZE]; // Temporary storage for 512 bytes
    int imageCount = 0;      // Counter for recovered images
    char filename[8];        // Stores generated image filename
    FILE *outFile = NULL;    // Output JPEG file pointer

    while (true)
    {
        // Read one 512-byte block from memory card
        size_t bytesRead = fread(buffer, sizeof(BYTE), BLOCK_SIZE, inptr);

        // Stop loop if end of file is reached
        if (bytesRead == 0 && feof(inptr))
        {
            break;
        }

        bool containsJpegHeader = bytesRead >= 4 && buffer[0] == 0xff && buffer[1] == 0xd8 &&
                                  buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0;

        // If a new JPEG is found and one is already open, close it
        if (containsJpegHeader && outFile != NULL)
        {
            fclose(outFile);
            imageCount++;
        }

        if (containsJpegHeader)
        {
            snprintf(filename, sizeof filename, "%03i.jpg", imageCount);
            outFile = fopen(filename, "wb");
            if (outFile == NULL)
            {
                fclose(inptr);
                return 1;
            }
        }

        if (outFile != NULL)
        {
            fwrite(buffer, sizeof(BYTE), bytesRead, outFile);
        }
    }

    if (outFile != NULL)
    {
        fclose(outFile);
    }
    fclose(inptr);
    return 0;
}
