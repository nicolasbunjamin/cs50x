#include <stdio.h>
#include <cs50.h>

long cc; // credit card number
int sum_digits; // the product of luhns_algorithm
void luhns_algorithm(void); // a formula used to validate credit card numbers
void check(void);

int main(void)
{
    cc = get_long("Credit Card Number: ");
    luhns_algorithm();
    check();
}

void luhns_algorithm(void)
{
    int dod;    // dod = "doubled other digits"
    sum_digits = 0; // sum of dod's digits (not sum of cc's digits, neither just sum of dod)

    for (long n = 10; n <= 1000000000000000; n *= 100)
    {
        // get every other digits (hence n *= 100
        // from the second-to-last (/ 10 % 10)
        // and doubles it (* 2)
        dod = cc / n % 10 * 2;
        for (int m = 1; m <= 100; m *= 10)
        {
            // separate every digit of the product (e.g. 12 becomes 1 and 2)
            sum_digits += dod / m % 10;
        }
    }

    for (long o = 1; o <= 1000000000000000; o *= 100)
    {
        // take the previously calculated digit and add the rest of the remaining digits
        sum_digits += cc / o % 10;
    }
}

void check(void)
{
    long amex = 10000000000000; // amex cc has 15 digits
    long mcard = 100000000000000; // mcard cc has 16 digits
    long visa1 = 1000000000000000; // visa cc can have 13 or 16 digits
    long visa2 = 1000000000000;

    bool luhncheck = sum_digits % 10 == 0;
    bool amexcheck = cc / amex == 34 || cc / amex == 37;
    bool mcardcheck = cc / mcard >= 51 && cc / mcard <= 55;
    bool visacheck = cc / visa1 == 4 || cc / visa2 == 4;

    if (luhncheck && amexcheck)
    {
        printf("AMEX\n");

    }
    else if (luhncheck && mcardcheck)
    {
        printf("MASTERCARD\n");

    }
    else if (luhncheck && visacheck)
    {
        printf("VISA\n");

    }
    else
    {
        printf("INVALID\n");

    }
}