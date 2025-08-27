#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    // Prompt until a valid height (1–8)
    do
    {
        height = get_int("Height (1-8): ");
        if (height < 1 || height > 8)
        {
            printf("Invalid. Please enter a number between 1 and 8.\n");
        }
    }
    while (height < 1 || height > 8);

    // Loop through each row of the pyramid
    for (int row = 1; row <= height; row++)
    {
        // Print leading spaces on the left pyramid
        for (int s = 0; s < height - row; s++)
        {
            printf(" ");
        }
        // Print left pyramid blocks
        for (int t = 0; t < row; t++)
        {
            printf("#");
        }
        printf("  "); // gap
        // Print right pyramid blocks
        for (int t = 0; t < row; t++)
        {
            printf("#");
        }
        printf("\n");
    }
}
