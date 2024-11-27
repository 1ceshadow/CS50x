#include <math.h>

#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE red = image[i][j].rgbtRed;
            BYTE green = image[i][j].rgbtGreen;
            BYTE blue = image[i][j].rgbtBlue;

            BYTE grayscale = round((red + green + blue) / 3.0);

            image[i][j].rgbtRed = grayscale;
            image[i][j].rgbtGreen = grayscale;
            image[i][j].rgbtBlue = grayscale;
        }
    }
    return;
}

int lesser(int a, int b)
{
    if (a > b)
        return b;
    else
        return a;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE red = image[i][j].rgbtRed;
            BYTE green = image[i][j].rgbtGreen;
            BYTE blue = image[i][j].rgbtBlue;

            BYTE sepiaRed = lesser(round(.393 * red + .769 * green + .189 * blue), 255);
            BYTE sepiaGreen = lesser(round(.349 * red + .686 * green + .168 * blue), 255);
            BYTE sepiaBlue = lesser(round(.272 * red + .534 * green + .131 * blue), 255);

            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < (width / 2); i++)
        {
            RGBTRIPLE temp;

            temp = image[j][i];
            image[j][i] = image[j][width - i - 1];
            image[j][width - i - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];  // 临时数组来存储模糊后的图像

    // 定义结构体SUMROUND，存储RGB的累加值
    typedef struct
    {
        int red;
        int green;
        int blue;
    } SUMROUND;

    // 遍历图像的每个像素
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            SUMROUND sumround = {0, 0, 0};
            float num = 0;  // 记录有效像素的数量

            // 遍历当前像素周围的邻域
            for (int s = -1; s <= 1; s++) // -1, 0, 1
            {
                for (int t = -1; t <= 1; t++) // -1, 0, 1
                {
                    int ni = i + s;  // 邻域像素行索引
                    int nj = j + t;  // 邻域像素列索引

                    // 检查邻域像素是否在图像范围内
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        sumround.red += image[ni][nj].rgbtRed;
                        sumround.green += image[ni][nj].rgbtGreen;
                        sumround.blue += image[ni][nj].rgbtBlue;
                        num++;  // 增加有效像素数
                    }
                }
            }

            // 计算模糊后的颜色值并存入临时数组
            copy[i][j].rgbtRed = round(sumround.red / num);
            copy[i][j].rgbtGreen = round(sumround.green / num);
            copy[i][j].rgbtBlue = round(sumround.blue / num);
        }
    }

    // 将模糊后的数据复制回原图像
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }
    return;
}
