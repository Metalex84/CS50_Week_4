#include <math.h>
#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float shadeGrey;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            shadeGrey = round(((float) image[i][j].rgbtBlue + (float) image[i][j].rgbtGreen + (float) image[i][j].rgbtRed) / 3);
            image[i][j].rgbtBlue = shadeGrey;
            image[i][j].rgbtGreen = shadeGrey;
            image[i][j].rgbtRed = shadeGrey;
        }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    float sepiaRed, sepiaGreen, sepiaBlue;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            // 1. Get "sepia" combination according to algorithm
            sepiaRed = round(0.393 * (float) image[i][j].rgbtRed + 0.769 * (float) image[i][j].rgbtGreen +
                             0.189 * (float) image[i][j].rgbtBlue);
            sepiaGreen = round(0.349 * (float) image[i][j].rgbtRed + 0.686 * (float) image[i][j].rgbtGreen +
                               0.168 * (float) image[i][j].rgbtBlue);
            sepiaBlue = round(0.272 * (float) image[i][j].rgbtRed + 0.534 * (float) image[i][j].rgbtGreen +
                              0.131 * (float) image[i][j].rgbtBlue);

            // 2. Make sure that values bigger than 0xff should be considered as "white"
            if (sepiaRed > 255)
                sepiaRed = 255;
            if (sepiaGreen > 255)
                sepiaGreen = 255;
            if (sepiaBlue > 255)
                sepiaBlue = 255;

            // 3. Assign "good sepia" values to actual RGB combination
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE swapReflect;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width / 2; j++)
        {
            swapReflect = image[i][j];
            image[i][j] = image[i][width - (j + 1)];
            image[i][width - (j + 1)] = swapReflect;
        }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    int totalBlue, totalGreen, totalRed;
    float count;

    //1. Backup source image
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            copy[i][j] = image[i][j];

    //2. Store all RGB values + the counter
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Initialize all counters (for each cell)
            totalBlue = 0;
            totalGreen = 0;
            totalRed = 0;
            count = 0.0;
            // Go find neighbours!
            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    // For each pixel, find "out-of-bounds neighbours"
                    int currX = x + i;
                    int currY = y + j;
                    if (currX < 0 || currX > (height - 1) || currY < 0 || currY > (width - 1))
                    {
                        // Then, ignore them
                        continue;
                    }
                // Accumulate separate values of total R, G and B.
                totalBlue += image[currX][currY].rgbtBlue;
                totalGreen += image[currX][currY].rgbtGreen;
                totalRed += image[currX][currY].rgbtRed;
                count++;
                }
            }
            // Assign partial averages
            copy[i][j].rgbtBlue = round(totalBlue / count);
            copy[i][j].rgbtGreen = round(totalGreen / count);
            copy[i][j].rgbtRed = round(totalRed / count);
        }
    }
    // 3. Restore original image
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            image[i][j] = copy[i][j];

    return;
}
