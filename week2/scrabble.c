#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


int calculate_score( string string);

int main(void)
{
    char* string_1 = get_string("Player 1: ");
    char* string_2 = get_string("Player 2: ");

    int score_1 = calculate_score(string_1);
    int score_2 = calculate_score(string_2);

    if (score_1 < score_2)
        printf("Player 2 wins!\n");

    else if (score_2 < score_1)
        printf("Player 1 wins!\n");

    else  printf("Tie!\n");

    return 0;
}

int calculate_score( string my_string)
{
    int score = 0;
    int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

    for (int i = 0, len = strlen(my_string); i < len; i++)
    {
        if (isupper(my_string[i]))
            score += POINTS[my_string[i] - 'A'];

        else if (islower(my_string[i]))
            score += POINTS[my_string[i] - 'a'];
    }

    return score;
}
