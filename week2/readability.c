#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <math.h>

int calculate_grade(char* text);

int main(void)
{
    char* text = get_string("Text: ");

    int grade = calculate_grade(text);

    if (grade >= 16)
        printf("Grade 16+\n");
    else if (grade < 1)
        printf("Before Grade 1\n");
    else
        printf("Grade %d\n", grade);

    return 0;
}

int count_letters(char* text)
{
    int i = 0, sum_letters=0;

    while (text[i] != '\0')
    {
        if (isalnum(text[i]))
            sum_letters++;
        i++;
    }

    return sum_letters;
}

int count_words(char* text)
{
    int j = 0, sum_words = 0;

    while (text[j] != '\0')
    {
        if ((! isspace(text[j])) && isspace(text[j+1]))
            sum_words++;
        j++;
    }

    return sum_words + 1; //this algorithm does not contain ten last word.
}

int count_sentences(char* text)
{
    int n = 0, sum_sentences = 0;

    while (text[n] != '\0')
    {
        char char_ = text[n+1];
        if (isalnum(text[n]) && (char_ == '?' || char_ == '.' || char_ == '!'))
            sum_sentences++;
        n++;
    }

    return sum_sentences;
}

int calculate_grade(char* text)
{
    int words = count_words(text);
    float L = count_letters(text) / ((float) words) * 100;
    float S = count_sentences(text) / ((float) words) * 100;

    int index = round(0.0588 * L - 0.296 * S - 15.8);

    return index;
}
