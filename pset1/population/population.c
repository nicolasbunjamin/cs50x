#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int population = 0;
    int end;
    int counter = 0;

    do
    {
        population = get_int("Start Size: ");
    }
    while (population < 9);

    do
    {
        end = get_int("End Size: ");
    }
    while (end < population);

    while (population < end)
    {
        population = population + (population / 3) - (population / 4);
        counter += 1;
        printf("%i\n", counter);
        printf("%i\n", population);
    }

    if (population >= end)
    {
        printf("Years: %i\n", counter);
    }
}