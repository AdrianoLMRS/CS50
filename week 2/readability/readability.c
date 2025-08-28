#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main(void)
{
    // Get text input
    string text = get_string("Text: ");

    int letters = 0;
    int words = 0;
    int sentences = 0;

    // Count letters, words, and sentences
    for (int i = 0; text[i] != '\0'; i++)
    {
        char c = text[i];

        // Count letters
        if (isalpha(c))
        {
            letters++;
        }

        // Count words
        if (isspace(c) && text[i+1] != '\0' && !isspace(text[i+1]))
        {
            words++;
        }

        // Count sentences (. ! ?)
        if (c == '.' || c == '!' || c == '?')
        {
            sentences++;
        }
    }

    // Add 1 to words to count the last word
    words++;

    // Compute averages per 100 words
    float lettersAvg = ((float) letters / words) * 100;
    float sentencesAvg = ((float) sentences / words) * 100;

    // Compute Coleman-Liau index
    int grade = round(0.0588 * lettersAvg - 0.296 * sentencesAvg - 15.8);

    // Print grade
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}