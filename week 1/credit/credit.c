#include <cs50.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
    long cc;

    // Get input (must be positive)
    do
    {
        cc = get_long("Insert C/C number: ");
    }
    while (cc <= 0);

    // Count digits
    int cc_length = floor(log10(cc)) + 1;

    // Check valid lengths
    if (!(cc_length == 13 || cc_length == 15 || cc_length == 16))
    {
        printf("INVALID\n");
        return 0;
    }

    // Luhn checksum calculation
    int checksum = 0;
    bool alternate = false;
    long num = cc;

    while (num > 0)
    {
        int digit = num % 10;
        if (alternate)
        {
            digit *= 2;
            checksum += digit / 10 + digit % 10;
        }
        else
        {
            checksum += digit;
        }
        alternate = !alternate;
        num /= 10;
    }

    // If checksum is invalid
    if (checksum % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }

    // Extract first two digits
    long prefix = cc;
    while (prefix >= 100)
    {
        prefix /= 10;
    }
    int first_two = (int) prefix;
    int first_one = first_two / 10;

    // Determine card type
    if ((first_two == 34 || first_two == 37) && cc_length == 15)
    {
        printf("AMEX\n");
    }
    else if (first_one == 4 && (cc_length == 13 || cc_length == 16))
    {
        printf("VISA\n");
    }
    else if (first_two >= 51 && first_two <= 55 && cc_length == 16)
    {
        printf("MASTERCARD\n");
    }
    else
    {
        printf("INVALID\n");
    }
}
