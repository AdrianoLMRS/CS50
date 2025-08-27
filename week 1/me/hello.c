#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Get name input
    string name = get_string("what is your name?    ");
    // Print out message with input
    printf("hello, %s\n", name);
};
