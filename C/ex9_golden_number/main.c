/* Ask the user to guess a number between 0 and 99. */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void main()
{
    int tries=0, number, guess, error;
    time_t t;
    srand((unsigned) time(&t));     // initialize the random number generator
    number = rand() % 100;
    while (guess != number || tries == 0) {
        tries++;
        printf("Please enter a number between 0 and 99. n = ");
        while (!scanf("%d", &guess)) {
            printf("This is not a number. Please try again. n = ");
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}     // flush stdin
        }
        if (guess < number)
            printf("The number is higher.\n");
        else if (guess > number)
            printf("The number is lower.\n");
    }
    printf("Well done. Total tries: %d.\n", tries);
}
