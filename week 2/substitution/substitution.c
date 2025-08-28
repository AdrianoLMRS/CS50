#include <cs50.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution <KEY>\n");
        return 1;
    }

    char *key = argv[1]; // Store the key from argv

    if (strlen(key) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // Track which letters have been seen (ensure uniqueness)
    bool seen[26] = {false};

    // Validate key
    for (int i = 0; i < 26; i++)
    {
        if (!isalpha((unsigned char) key[i]))
        {
            printf("Key must only contain alphabetic characters.\n");
            return 1;
        }

        int idx = tolower((unsigned char) key[i]) - 'a'; // case-insensitively

        if (seen[idx])
        {
            printf("Key must not contain repeated characters.\n");
            return 1;
        }
        seen[idx] = true;
    }

    // substitution for uppercase and lowercase letters
    char map_upper[26];
    char map_lower[26];
    for (int i = 0; i < 26; i++)
    {
        char k = key[i];
        map_upper[i] = toupper((unsigned char) k);
        map_lower[i] = tolower((unsigned char) k);
    }

    string plaintext = get_string("plaintext: ");
    if (plaintext == NULL)
    {
        return 1;
    }

    printf("ciphertext: ");
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        unsigned char c = plaintext[i];
        // If uppercase => substitute using the uppercase arr
        if (isupper(c))
        {
            int idx = c - 'A';
            putchar(map_upper[idx]);
        }
        // If lowercase => substitute using the lowercase arr
        else if (islower(c))
        {
            int idx = c - 'a';
            putchar(map_lower[idx]);
        }
        // If !letter, dont change
        else
        {
            putchar(c);
        }
    }
    putchar('\n');

    return 0;
}