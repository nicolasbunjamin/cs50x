#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float dollars;
    do
    {
        dollars = get_float("Change: ");
    }
    while (dollars <= 0);
    
    // turn dollar value to cent equivalent
    int cents = round(dollars * 100);
    
    int quarters = cents / 25; // calculate number of quarters
    int dimes = cents % 25 / 10; // calculate number of dimes
    int nickels = cents % 25 % 10 / 5; // calculate number of nickels
    int pennies = cents % 25 % 10 % 5; // calculate number of pennies

    int coins = quarters + dimes + nickels + pennies;
    printf("%i\n", coins);
}
