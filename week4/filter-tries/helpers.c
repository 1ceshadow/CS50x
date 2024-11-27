#include <math.h>

#include "helpers.h"

int suitrange(int g)
{
    return (g < 0) ? 0 : (g > 255) ? 255 : g;
}
// Convert image to sepia // 棕色
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE red = image[i][j].rgbtRed;
            BYTE green = image[i][j].rgbtGreen;
            BYTE blue = image[i][j].rgbtBlue;

            BYTE sepiaRed = suitrange(round(.393 * red + .769 * green + .189 * blue));
            BYTE sepiaGreen = suitrange(round(.349 * red + .686 * green + .168 * blue));
            BYTE sepiaBlue = suitrange(round(.272 * red + .534 * green + .131 * blue));

            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Convert image to grayscale // 灰白
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

// Reflect image horizontally // 镜像
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop through each row
    for (int i = 0; i < height; i++)
    {
        // Only loop through the left half of the row, swap with corresponding element on the right
        for (int j = 0; j < width / 2; j++)
        {
            // Swap image[i][j] with image[i][width - j - 1]
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}


// Blur image // 模糊
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[3][width];  // 临时数组来存储模糊后的图像

    // 遍历图像的每个像素
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int tred = 0, tgreen = 0, tblue = 0;
            float num = 0;  // 记录有效像素的数量

            // 遍历当前像素周围的邻域
            for (int h = -1; h <= 1; h++)
            {
                for (int w = -1; w <= 1; w++)
                {
                    int ni = i + h;  // 邻域像素行索引
                    int nj = j + w;  // 邻域像素列索引

                    // 检查邻域像素是否在图像范围内
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        tred += image[ni][nj].rgbtRed;
                        tgreen += image[ni][nj].rgbtGreen;
                        tblue += image[ni][nj].rgbtBlue;
                        num++;  // 增加有效像素数
                    }
                }
            }

            // 计算模糊后的颜色值并存入临时数组
            tmp[i % 3][j].rgbtRed = round(tred / num);
            tmp[i % 3][j].rgbtGreen = round(tgreen / num);
            tmp[i % 3][j].rgbtBlue = round(tblue / num);

            if (i + 1 >= 3)
            {
                for (int wid = 0; wid < width; wid++)
                {
                    image[i - 2][wid] = tmp[(i + 1) % 3][wid];
                }
            }
        }
    }

    // 图象太小的情况
    if (height <= 3)
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                image[i][j] = tmp[i][j];
            }
        }
    }
}

void edge(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[3][width];  // When the fourth line(also tmp[2]) is calculated,
                              //  the first line is updated to the original file.
                              // To avoid occupying large memory


    // Sobel kernels for gx and gy
    int gx[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int gy[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int g[2][3] = {0};  // gx, gy for red, green, blue

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
            tmp[i % 3][j].rgbtRed = suitrange(round(hypot(g[0][0], g[1][0])));
            tmp[i % 3][j].rgbtGreen = suitrange(round(hypot(g[0][1], g[1][1])));
            tmp[i % 3][j].rgbtBlue = suitrange(round(hypot(g[0][2], g[1][2])));

            // If tmp is filled, the line that does not affect the calculation is updated into the original image
            if (i + 1 >= 3)
            {
                for (int wid = 0; wid < width; wid++)
                {
                    image[i - 2][wid] = tmp[(i + 1) % 3][wid];
                }
            }
        }
    }
    if (height <= 3)  // if the height of original image is too few.
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                image[i][j] = tmp[i][j];
            }
        }
    }
}

// AI generation.version2
// 原来AI笨的，apply_sobel_kernel中它只处理red值，edges函数中还三次调用这个函数，导致生成出来的图象是灰白的，下面我又改进了下
// Apply Sobel kernel to calculate the gradients for all channels (red, green, blue)
void apply_sobel_kernel(int i, int j, int height, int width, int *gx, int *gy, RGBTRIPLE image[height][width], char color)
{
    // Sobel kernels for gx and gy
    int gx_kernel[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int gy_kernel[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    // Initialize gradient sums for gx and gy
    *gx = *gy = 0;

    for (int di = -1; di <= 1; di++)
    {
        for (int dj = -1; dj <= 1; dj++)
        {
            // Compute neighbor pixel coordinates
            int ni = i + di;
            int nj = j + dj;

            // Skip out-of-bound pixels
            if (ni < 0 || ni >= height || nj < 0 || nj >= width)
                continue;

            // Get the kernel values (gx or gy) based on position
            int gx_weight = gx_kernel[(di + 1) * 3 + (dj + 1)];
            int gy_weight = gy_kernel[(di + 1) * 3 + (dj + 1)];

            // Accumulate weighted sum of gradients for red, green, and blue channels
            if (color == 'R')  // Red channel
            {
                *gx += image[ni][nj].rgbtRed * gx_weight;
                *gy += image[ni][nj].rgbtRed * gy_weight;
            }
            else if (color == 'G')  // Green channel
            {
                *gx += image[ni][nj].rgbtGreen * gx_weight;
                *gy += image[ni][nj].rgbtGreen * gy_weight;
            }
            else if (color == 'B')  // Blue channel
            {
                *gx += image[ni][nj].rgbtBlue * gx_weight;
                *gy += image[ni][nj].rgbtBlue * gy_weight;
            }
        }
    }
}

// Detect edges using Sobel operator
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Declare temporary variables to hold gradient values
    int gx, gy;
    RGBTRIPLE tmp[3][width];  // Temporary array to store the edge-detected image

    // Iterate over every pixel in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate the gradient for the red channel
            apply_sobel_kernel(i, j, height, width, &gx, &gy, image, 'R');
            tmp[i % 3][j].rgbtRed = suitrange(round(sqrt(gx * gx + gy * gy)));

            // Calculate the gradient for the green channel
            apply_sobel_kernel(i, j, height, width, &gx, &gy, image, 'G');
            tmp[i % 3][j].rgbtGreen = suitrange(round(sqrt(gx * gx + gy * gy)));

            // Calculate the gradient for the blue channel
            apply_sobel_kernel(i, j, height, width, &gx, &gy, image, 'B');
            tmp[i % 3][j].rgbtBlue = suitrange(round(sqrt(gx * gx + gy * gy)));

            // If tmp is filled, the line that does not affect the calculation is updated into the original image
            if (i + 1 >= 3)
            {
                for (int wid = 0; wid < width; wid++)
                {
                    image[i - 2][wid] = tmp[(i + 1) % 3][wid];
                }
            }
        }
    }

    if (height <= 3)  // if the height of original image is too few.
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                image[i][j] = tmp[i][j];
            }
        }
    }
}

void r_r(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = 255 - image[i][j].rgbtRed;
            image[i][j].rgbtGreen = 255 - image[i][j].rgbtGreen;
            image[i][j].rgbtBlue = 255 - image[i][j].rgbtBlue;
        }
    }
}