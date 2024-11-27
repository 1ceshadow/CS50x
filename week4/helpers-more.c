#include <math.h>

#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE grayscale =
                round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
            image[i][j].rgbtRed = grayscale;
            image[i][j].rgbtGreen = grayscale;
            image[i][j].rgbtBlue = grayscale;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height][width]; // 临时数组来存储模糊后的图像

    // 遍历图像的每个像素
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int tred = 0, tgreen = 0, tblue = 0;
            float num = 0; // 记录有效像素的数量

            // 遍历当前像素周围的邻域
            for (int h = -1; h <= 1; h++)
            {
                for (int w = -1; w <= 1; w++)
                {
                    int ni = i + h; // 邻域像素行索引
                    int nj = j + w; // 邻域像素列索引

                    // 检查邻域像素是否在图像范围内
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        tred += image[ni][nj].rgbtRed;
                        tgreen += image[ni][nj].rgbtGreen;
                        tblue += image[ni][nj].rgbtBlue;
                        num++; // 增加有效像素数
                    }
                }
            }

            // 计算模糊后的颜色值并存入临时数组
            tmp[i][j].rgbtRed = round(tred / num);
            tmp[i][j].rgbtGreen = round(tgreen / num);
            tmp[i][j].rgbtBlue = round(tblue / num);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmp[i][j];
        }
    }
    return;
}

// make sure that gx or gy is in 0~255.
int suitrange(int g)
{
    return (g < 0) ? 0 : (g > 255) ? 255 : g;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height][width];

    // Sobel kernels for gx and gy
    int gx[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int gy[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int g[2][3] = {0}; // gx, gy for red, green, blue

            for (int x = 0; x < 2; x++) // x: 0 -> gx, 1 -> gy
            {
                int *kernel = (x == 0) ? gx : gy; // Select the correct kernel

                for (int h = 0; h < 3; h++) // For each row of the 3x3 kernel
                {
                    for (int w = 0; w < 3; w++) // For each column of the 3x3 kernel
                    {
                        int ni = i - 1 + h; // Neighbor pixel row
                        int nj = j - 1 + w; // Neighbor pixel column

                        // Skip if the neighbor pixel is out of bounds
                        if (ni < 0 || ni >= height || nj < 0 || nj >= width)
                            continue;

                        int weight = kernel[h * 3 + w]; // Get corresponding weight
                        g[x][0] += image[ni][nj].rgbtRed * weight;
                        g[x][1] += image[ni][nj].rgbtGreen * weight;
                        g[x][2] += image[ni][nj].rgbtBlue * weight;
                    }
                }
            }

            // Compute the gradient magnitude for each color channel
            tmp[i][j].rgbtRed = suitrange(round(hypot(g[0][0], g[1][0])));
            tmp[i][j].rgbtGreen = suitrange(round(hypot(g[0][1], g[1][1])));
            tmp[i][j].rgbtBlue = suitrange(round(hypot(g[0][2], g[1][2])));
        }
    }
    // Copy the results back to the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmp[i][j];
        }
    }
    return;
}
