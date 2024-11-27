/*
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define BLOCK_SIZE 512
#define JPEG_HEADER_SIZE 4

typedef struct {
    uint8_t *data;  // 动态分配内存
    size_t size;
} Block;

// 判断 JPEG 文件的开头
bool is_jpeg(Block block) {
    return (block.data[0] == 0xFF && block.data[1] == 0xD8 && block.data[2] == 0xFF &&
            block.data[3] >= 0xE0 && block.data[3] < 0xF0);
}

// 生成文件名
void namefile(int nth, char *outfile) {
    snprintf(outfile, 12, "%03d.jpg", nth);  // 生成文件名如 000.jpg, 001.jpg, ... 999.jpg
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Accept only one file.\n");
        return 1;
    }

    char *infile = argv[1];
    FILE *inptr = fopen(infile, "rb");
    if (!inptr) {
        printf("Could not open %s\n", infile);
        return 1;
    }

    // 动态分配内存来存储块
    Block block;
    block.data = (uint8_t *)malloc(BLOCK_SIZE);
    if (!block.data) {
        printf("Memory allocation failed\n");
        fclose(inptr);
        return 2;
    }
    block.size = BLOCK_SIZE;

    FILE *outptr = NULL;
    int nth = 0;
    bool is_firstfile = true;
    char outfile[12];  // 文件名存储空间

    while (fread(block.data, 1, block.size, inptr) == block.size) {  // 读取一个块
        if (is_jpeg(block)) {
            // 检查是否是新的 JPEG 文件
            if (is_firstfile) {
                namefile(nth, outfile);
                outptr = fopen(outfile, "wb");
                if (!outptr) {
                    printf("Could not create %s\n", outfile);
                    free(block.data);
                    fclose(inptr);
                    return 2;
                }
                is_firstfile = false;
            } else {
                fclose(outptr);  // 关闭旧的文件
                namefile(nth, outfile);
                outptr = fopen(outfile, "wb");
                if (!outptr) {
                    printf("Could not create %s\n", outfile);
                    free(block.data);
                    fclose(inptr);
                    return 2;
                }
            }
            nth++;  // 增加文件编号
        }

        // 写入当前块
        if (outptr) {
            fwrite(block.data, 1, block.size, outptr);
        }
    }

    // 最后一个文件关闭
    if (outptr) {
        fclose(outptr);
    }

    free(block.data);  // 释放动态分配的内存
    fclose(inptr);  // 关闭输入文件

    return 0;
}
*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;
const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    BYTE buffer[BLOCK_SIZE];
    int nth = 0;
    FILE *output = NULL;
    char filename[8];

    // Repeat to read 512 bytes to a buffer
    while (fread(&buffer, BLOCK_SIZE, 1, input))
    {
        // Check start of new JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // Check first
            if (nth)
            {
                fclose(output);
            }

            // Generate JPEG file
            sprintf(filename, "%03i.jpg", nth);
            output = fopen(filename, "w");
            nth++;
        }

        // Already found JPEG
        if (nth)
        {
            fwrite(&buffer, BLOCK_SIZE, 1, output);
        }
    }

    fclose(input);
    fclose(output);
    return 0;
}
/*
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct
{
    uint8_t body[512];
}BLOCK;


int is_jpeg(BLOCK block); // judge if a block is the beginning of a jpeg.(This C don't contain
bool?) void namefile(int nth, char *outfile); // name the nth file.

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("accept only a file.\n");
        return 1;
    }

    char *infile = argv[1];
    FILE *inptr = fopen(infile,"rb");
    if (! inptr)
    {
        printf("Cound not open %s\n", infile);
        return 1;
    }


    int is_firstfile = 1;  // bool is_firstfile = true.

    int nth = 0;
    char *outfile = "000.jpg";
    FILE *outptr = NULL;

    while (nth < 50)
    {
        BLOCK block;
        size_t bytes_read = fread(&block, sizeof(BLOCK), 1, inptr);
        if (bytes_read != 1)
        {
            // 检查是否读取成功
            break;
        }

        if (is_jpeg(block))
        {
            if (is_firstfile)
            {
                namefile(nth, outfile);
                outptr = fopen(outfile, "wb");
                if (! outptr)
                {
                    printf("Could not create %s\n", outfile);
                    return 2;
                }
                is_firstfile = 0;
            }
            else
            {
                fclose(outptr);
                namefile(nth, outfile);
                outptr = fopen(outfile, "wb");
                if (! outptr)
                {
                    printf("Could not create %s\n", outfile);
                    return 2;
                }
            }
            nth++;
        }
        else if (is_firstfile) continue;

        if (outptr)
        {
            fwrite(&block, sizeof(BLOCK), 1, outptr);
        }
    }
    if (outptr)
    {
        fclose(outptr);
    }
    fclose(inptr);

    return 0;
}

int is_jpeg(BLOCK block)
{
    if (block.body[0] == 0xff && block.body[1] == 0xd8 && block.body[2] == 0xff && \
    (block.body[3] >= 0xe0 && block.body[3] < 0xf0))
        return 1;  // true
    else return 0;  // false;
}

void namefile(int nth, char *outfile)
{
    if (nth < 10)
    {
        sprintf(outfile, "00%d.jpg", nth);
        return;
    }
    if (nth < 100)
    {
        sprintf(outfile, "0%d,jpg", nth);
        return;
    }
    if (nth < 1000)
    {
        sprintf(outfile, "%d,jpg", nth);
        return;
    }
}
*/
