#include "helpers.h"
#include <math.h>

// Limit RGB value to 255 (1 byte)
int limit(int value)
{
    if (value > 255)
        return 255;
    return value;
}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through each pixel of the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate avg values in RGB
            int avgValue = (int) round(
                (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            // Set same avg values to RGB
            image[i][j].rgbtBlue = avgValue;
            image[i][j].rgbtRed = avgValue;
            image[i][j].rgbtGreen = avgValue;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through each pixel of the image...
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Using the Sepia formula for calculating RGB values
            int sepiaRed = (int) round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen +
                                       .189 * image[i][j].rgbtBlue);
            int sepiaGreen = (int) round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen +
                                         .168 * image[i][j].rgbtBlue);
            int sepiaBlue = (int) round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen +
                                        .131 * image[i][j].rgbtBlue);

            sepiaRed = limit(sepiaRed);
            sepiaGreen = limit(sepiaGreen);
            sepiaBlue = limit(sepiaBlue);

            image[i][j].rgbtBlue = sepiaBlue;
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through each row and 1/2 width
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // Temporarily store the reflected pixel on the right side
            RGBTRIPLE tmpReplace = image[i][width - 1 - j];
            // Swap pixel for reflection
            image[i][width - 1 - j] = image[i][j];
            image[i][j] = tmpReplace;
        }
    }
}

// Blur image (box blur)
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    RGBTRIPLE tmpCopy[height][width];

    // Copy matrix into temp matrix
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmpCopy[i][j] = image[i][j];
        }
    }

    // Iterate through each pixel of the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avgR = 0, avgG = 0, avgB = 0, counter = 0;

            // Neighbor pixels (3x3 grid)
            for (int row = -1; row < 2; row++)
            {
                for (int column = -1; column < 2; column++)
                {
                    int ni = i + row;
                    int nj = j + column;

                    // Check valid neighbor
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        avgR += tmpCopy[ni][nj].rgbtRed;
                        avgG += tmpCopy[ni][nj].rgbtGreen;
                        avgB += tmpCopy[ni][nj].rgbtBlue;
                        counter++;
                    }
                }
            }

            // Assign average values
            image[i][j].rgbtRed = round((float) avgR / counter);
            image[i][j].rgbtGreen = round((float) avgG / counter);
            image[i][j].rgbtBlue = round((float) avgB / counter);
        }
    }
}
