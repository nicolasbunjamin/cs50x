#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // for each row,
    for (int row = 1; row <= height; row++)
    {
        // print " " (height - row) times
        for (int space = height - row; space > 0; space--)
        {
            printf(" ");
        }

        // print "#" (row) times
        for (int pyramid = 1; pyramid <= row; pyramid++)
        {
            printf("#");
        }

        // print the gap between the pyramids
        printf("  ");

        // print "#" (row) times
        for (int pyramid2 = 1; pyramid2 <= row; pyramid2++)
        {
            printf("#");
        }

        // go to the next row
        printf("\n");
    }
}