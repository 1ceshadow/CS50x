#include <cs50.h>
#include <stdio.h>

void pyramids( int height )
{
    for (int i = 1; i <= height; i++){
        for (int j = 0; j < height - i; j++)
            printf(" ");
        for (int t = 0; t < i; t++)
            printf("#");
        printf("  ");
        for (int t = 0; t < i; t++)
            printf("#");
        printf("\n");
    }
}


int main()
{
    int height;
    do{
        height = get_int("Height:");
    }while(!(height >= 1 && height <= 8));
    pyramids(height);

    return 0;
}
