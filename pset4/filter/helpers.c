#include <math.h>
#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int r_channel = image[h][w].rgbtRed;
            int g_channel = image[h][w].rgbtGreen;
            int b_channel = image[h][w].rgbtBlue;
            int average = (round((r_channel + g_channel + b_channel) / 3.0));
            image[h][w].rgbtRed = average;
            image[h][w].rgbtGreen = average;
            image[h][w].rgbtBlue = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a temporary image to store filtered pixels in,
    // lest the filtered RGB channel value will mess up the filtering of the channel next to it
    RGBTRIPLE temp[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int red = image[h][w].rgbtRed;
            int green = image[h][w].rgbtGreen;
            int blue = image[h][w].rgbtBlue;

            int sepia_red = round(.393 * red + .769 * green + .189 * blue);
            int sepia_green = round(.349 * red + .686 * green + .168 * blue);
            int sepia_blue = round(.272 * red + .534 * green + .131 * blue);

            // Set upper limit of 255 to new RGB value
            temp[h][w].rgbtRed = (sepia_red > 255) ? 255 : sepia_red;
            temp[h][w].rgbtGreen = (sepia_green > 255) ? 255 : sepia_green;
            temp[h][w].rgbtBlue = (sepia_blue > 255) ? 255 : sepia_blue;
        }
    }
    // Replace pixels in image with pixels from temp
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = temp[h][w];
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < .5 * width; w++)
        {
            RGBTRIPLE temp = image[h][w];
            image[h][w] = image[h][width - 1 - w];
            image[h][width - 1 - w] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a temporary image to store filtered pixels in,
    // lest the filtered pixel will mess up the filtering of the pixel next to it
    RGBTRIPLE temp[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Count of grids to be used as the denominator in RGB mean value calculation
            float grid_counter = 0;
            // Sum of RGB value to be used as the numerator in RGB mean value calculation
            int total_r = 0;
            int total_g = 0;
            int total_b = 0;
            // Loop over the 9 × 9 grid around the pixel to be filtered
            for (int v_grid = -1; v_grid < 2; v_grid++)
            {
                for (int h_grid = -1; h_grid < 2; h_grid++)
                {
                    // Rule out exceptions
                    if ((h + v_grid) >= 0 && (w + h_grid) >= 0 && (h + v_grid) < height && (w + h_grid) < width)
                    {
                        // Add up the RGB value
                        total_r += image[h + v_grid][w + h_grid].rgbtRed;
                        total_g += image[h + v_grid][w + h_grid].rgbtGreen;
                        total_b += image[h + v_grid][w + h_grid].rgbtBlue;
                        // Increment grid counter
                        grid_counter ++;
                    }
                }
            }
            // Calculate mean RGB value of filtered pixel
            temp[h][w].rgbtRed = (int)(round(total_r / grid_counter));
            temp[h][w].rgbtGreen = (int)(round(total_g / grid_counter));
            temp[h][w].rgbtBlue = (int)(round(total_b / grid_counter));
        }
    }
    // Replace pixels in image with pixels from temp
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = temp[h][w];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a temporary image to store filtered pixels in,
    // lest the filtered pixel will mess up the filtering of the pixel next to it
    RGBTRIPLE temp[height][width];
    // Store Sobel operator kernels
    int gx_kernel[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int gy_kernel[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Initialize altered RGB values
            int gx_red = 0;
            int gx_green = 0;
            int gx_blue = 0;
            int gy_red = 0;
            int gy_green = 0;
            int gy_blue = 0;

            // Initalize counter for kernel
            int grid_counter = 0;

            // Loop over kernel
            for (int v_grid = -1; v_grid < 2; v_grid++)
            {
                for (int h_grid = -1; h_grid < 2; h_grid++)
                {
                    // Rule out exceptions
                    if ((h + v_grid) >= 0 && (w + h_grid) >= 0 && (h + v_grid) < height && (w + h_grid) < width)
                    {
                        // Multiplier
                        int gx = gx_kernel[grid_counter];
                        int gy = gy_kernel[grid_counter];
                        // Add up the altered RGB value

                        // Gx kernel
                        gx_red += image[h + v_grid][w + h_grid].rgbtRed * gx;
                        gx_green += image[h + v_grid][w + h_grid].rgbtGreen * gx;
                        gx_blue += image[h + v_grid][w + h_grid].rgbtBlue * gx;

                        // Gy kernel
                        gy_red += image[h + v_grid][w + h_grid].rgbtRed * gy;
                        gy_green += image[h + v_grid][w + h_grid].rgbtGreen * gy;
                        gy_blue += image[h + v_grid][w + h_grid].rgbtBlue * gy;
                    }
                    // Increment grid counter outside of if statement
                    // because grid counter must iterate even if the kernel hasn't been used
                    grid_counter++;
                }
            }

            // Combine kernel to obtain new RGB value
            int sobel_red = round(pow((pow(gx_red, 2) + pow(gy_red, 2)), .5));
            int sobel_green = round(pow((pow(gx_green, 2) + pow(gy_green, 2)), .5));
            int sobel_blue = round(pow((pow(gx_blue, 2) + pow(gy_blue, 2)), .5));

            // Set upper limit of 255 to new RGB value
            temp[h][w].rgbtRed = (sobel_red > 255) ? 255 : sobel_red;
            temp[h][w].rgbtGreen = (sobel_green > 255) ? 255 : sobel_green;
            temp[h][w].rgbtBlue = (sobel_blue > 255) ? 255 : sobel_blue;
        }
    }
    // Replace pixels in image with pixels from temp
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = temp[h][w];
        }
    }
    return;
}
