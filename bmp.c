//ChatGPT code, I was too lazy do to by myself.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// BMP header structure
#pragma pack(push, 1)
typedef struct {
    uint16_t bfType;            // File type, should be 'BM' (0x42 0x4D)
    uint32_t bfSize;            // Size of the file
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;         // Offset to the pixel data
    uint32_t biSize;            // Size of the info header
    int32_t  biWidth;           // Image width
    int32_t  biHeight;          // Image height
    uint16_t biPlanes;          // Color planes (must be 1)
    uint16_t biBitCount;        // Bits per pixel (24 for RGB)
    uint32_t biCompression;     // Compression method (0 = no compression)
    uint32_t biSizeImage;       // Size of pixel data
    int32_t  biXPelsPerMeter;   // Horizontal resolution (pixels per meter)
    int32_t  biYPelsPerMeter;   // Vertical resolution (pixels per meter)
    uint32_t biClrUsed;         // Number of colors used (0 = default)
    uint32_t biClrImportant;    // Important colors (0 = all are important)
} BMPHeader;
#pragma pack(pop)

void create_bmp(const char *filename, int width, int height, uint8_t *pixels) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    // BMP Header
    BMPHeader header = {0};
    header.bfType = 0x4D42;  // 'BM' in little-endian
    header.bfSize = sizeof(BMPHeader) + width * height * 3;  // File size
    header.bfOffBits = sizeof(BMPHeader);  // Pixel data offset
    header.biSize = 40;  // Info header size
    header.biWidth = width;
    header.biHeight = height;
    header.biPlanes = 1;
    header.biBitCount = 24;  // 24 bits per pixel (RGB)
    header.biSizeImage = width * height * 3;  // Pixel data size

    // Write the header
    fwrite(&header, sizeof(BMPHeader), 1, file);

    // Write the pixel data
    // BMP stores data from bottom to top
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            uint8_t r = pixels[(y * width + x) * 3 + 0];  // Red
            uint8_t g = pixels[(y * width + x) * 3 + 1];  // Green
            uint8_t b = pixels[(y * width + x) * 3 + 2];  // Blue
            fwrite(&b, sizeof(uint8_t), 1, file);
            fwrite(&g, sizeof(uint8_t), 1, file);
            fwrite(&r, sizeof(uint8_t), 1, file);
        }
    }

    fclose(file);
}

int main() {
    int width = 256;
    int height = 256;
    
    // Allocate memory for pixel data (RGB)
    uint8_t *pixels = (uint8_t *)malloc(width * height * 3);
    if (!pixels) {
        perror("Failed to allocate memory for pixels");
        return -1;
    }

    // Create a simple gradient (example)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            pixels[(y * width + x) * 3 + 0] = x;    // Red channel
            pixels[(y * width + x) * 3 + 1] = y;    // Green channel
            pixels[(y * width + x) * 3 + 2] = (x + y) / 2;  // Blue channel
        }
    }

    // Create BMP image
    create_bmp("output.bmp", width, height, pixels);

    // Free allocated memory
    free(pixels);

    return 0;
}
