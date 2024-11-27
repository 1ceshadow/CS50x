/*
this code “encrypt” a message by replacing every letter with another letter, using a key.
A key, like NQXPOMAFTRHLZGECYJIUwskdvb, is a string.
This 26-character key means that A (the first letter of the alphabet) should be converted into N
(the first character of the key), B (the second letter of the alphabet) should be converted into
Q (the second character of the key), and so forth.
*/
#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void substitution_cipher(char *key, char *plaintext);
int key_judge(char *key);

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // judge the key
    int result = key_judge(argv[1]);

    if (result == 1)
    {
        printf("Key must contain exactly 26 characters.\n");
        return 1;
    }
    else if (result == 2)
    {
        printf("Key must not contain repeated characters.\n");
        return 1;
    }
    char *plaintext = get_string("plaintext:  ");
    substitution_cipher(argv[1], plaintext);

    return 0;
}

int key_judge(char *key)
{
    int len = strlen(key);

    if (len != 26)
        return 1;


    for (int i = 0; i < 26; i++)
    {
        if (!isalpha(key[i]))
            return 1;
    }

    int exist[26] = {0};
    for (int i = 0; i < 26; i++)
    {
        exist[toupper(key[i]) - 'A']++;
    }

    for (int i = 0; i < 26; i++)
    {
        if (exist[i] != 1)
            return 2;
    }

    return 0;
}

void substitution_cipher(char *key, char *plaintext)
{
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        if (isalpha(plaintext[i]))
        {
            if (isupper(plaintext[i]))
                plaintext[i] = toupper(key[plaintext[i] - 'A']);
            else
                plaintext[i] = tolower(key[plaintext[i] - 'a']);
        }
    }

    printf("ciphertext: %s\n", plaintext);
}
