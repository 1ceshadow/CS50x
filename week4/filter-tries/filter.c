#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helpers.h"  // 头文件包含滤镜函数定义

#define BMP_HEADER_SIZE 54
#define BMP_INFO_HEADER_SIZE 40
#define BMP_TYPE 0x4d42
#define BMP_BIT_COUNT 24
#define BMP_COMPRESSION 0

// 命令行参数支持的滤镜类型
const char *filters = "abegrst";

// 错误处理函数
void handle_error(const char *msg, FILE *inptr, FILE *outptr, void *image) {
    if (inptr) fclose(inptr);
    if (outptr) fclose(outptr);
    if (image) free(image);
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    // 检查命令行参数
    if (argc != 4) {
        printf("Usage: .\\filter.exe -[option] source_file target_file\n \
               -b Blur effect    -e Outline    -g Grayscale effect    -r Mirror\n \
               Example: .\\filter.exe -e 1.bmp 2.bmp\n");
        return 1;
    }

    // 获取滤镜类型
    int filter = getopt(argc, argv, filters);
    if (filter == '?') {
        printf("Invalid filter.\n");
        return 1;
    }

    // 检查是否传入多个滤镜选项
    if (getopt(argc, argv, filters) != -1) {
        printf("Only one filter allowed.\n");
        return 2;
    }

    // 获取输入和输出文件名
    char *infile = argv[2];
    char *outfile = argv[3];

    // 打开输入文件
    FILE *inptr = fopen(infile, "rb");
    if (!inptr) {
        printf("Could not open %s.\n", infile);
        return 3;
    }

    // 打开输出文件
    FILE *outptr = fopen(outfile, "wb");
    if (!outptr) {
        fclose(inptr);
        printf("Could not create %s.\n", outfile);
        return 4;
    }

    // 读取位图文件头和信息头
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // 验证 BMP 文件格式
    if (bf.bfType != BMP_TYPE || bf.bfOffBits != BMP_HEADER_SIZE || bi.biSize != BMP_INFO_HEADER_SIZE ||
        bi.biBitCount != BMP_BIT_COUNT || bi.biCompression != BMP_COMPRESSION) {
        fclose(inptr);
        fclose(outptr);
        printf("Unsupported file format.\n");
        return 5;
    }

    int height = abs(bi.biHeight);
    int width = bi.biWidth;

    // 为图像分配内存
    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;
    RGBTRIPLE (*image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (!image) {
        handle_error("Memory allocation failed", inptr, outptr, NULL);
    }

    // 读取像素数据
    for (int i = 0; i < height; i++) {
        fread(image[i], sizeof(RGBTRIPLE), width, inptr);
        fseek(inptr, padding, SEEK_CUR);
    }

    // 根据滤镜类型处理图像
    switch (filter) {
        // 模糊处理
        case 'b':
            blur(height, width, image);
            break;
        // 边缘检测
        case 'e':
            edges(height, width, image);
            break;
        case 'a':
            edge(height, width, image);
            break;
        // 灰度处理
        case 'g':
            grayscale(height, width, image);
            break;
        // 镜像处理
        case 'r':
            reflect(height, width, image);
            break;
        // 棕色处理
        case 's':
            sepia(height, width, image);
            break;
        // 灰白反转
        case 't':
            r_r(height, width, image);
            break;
        default:
            fclose(inptr);
            fclose(outptr);
            free(image);
            printf("Invalid filter type.\n");
            return 6;
    }

    // 写入输出文件头和信息头
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // 写入像素数据和填充字节
    for (int i = 0; i < height; i++) {
        fwrite(image[i], sizeof(RGBTRIPLE), width, outptr);
        for (int k = 0; k < padding; k++) {
            fputc(0x00, outptr);
        }
    }

    // 释放内存并关闭文件
    free(image);
    fclose(inptr);
    fclose(outptr);

    return 0;
}
