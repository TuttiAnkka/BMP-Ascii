#ifndef BMP_H
#define BMP_H

#include <stdio.h>
#include <stdint.h>
#include "stdbool.h"

#define ALLOWED_BYTES 24

; // Semicolon, because clangd has a bug with pragma pack
#pragma pack(push, 1)
typedef struct {
	uint16_t type; // Used for recognition: first two bytes should be 'B' and 'M' in ASCII
	uint32_t size; // Size of the BMP file in bytes
	uint16_t reserved1; // --
	uint16_t reserved2; // --
	uint32_t offset; // Starting address of pixel array
} BMPFileHeader;

typedef struct {
	uint32_t size; // Size of the header in bytes (40)
	int32_t width; // Bitmap width in pixels
	int32_t height; // Bitmap height in pixels
	uint16_t color_planes; // Must be 1
	uint16_t color_depth; // Bits per pixel, typically 1, 4, 8, 16, 24, 32
	uint32_t compression; // 0 = uncompressed
	uint32_t image_size; // Can be 0 with uncompressed bitmaps
	int32_t horizontal_resolution; // Pixel per meter - X
	int32_t vertical_resolution; // Pixel per meter - Y
	uint32_t colors; // Total number of colors in the palette
	uint32_t important_colors; // Number of important colors in the palette - usually ignored
} BMPInfoHeader;
#pragma pack(pop)

typedef struct {
	unsigned char *image; // Pixel data
	int width;
	int height; // Can be negative
	int abs_height; // Absolute height value used to make sure ASCII is printed top-to-bottom
	int row_size;
} BMPImage;

//Gets the header info from BMP file
void get_bmp_headers(FILE *bmpfile, BMPInfoHeader *info_header, BMPFileHeader *file_header);
// Reads pixel data into BMPImage from BMP headers
void read_pixel_data(FILE *bmpfile, BMPImage *image, BMPInfoHeader *info_header, BMPFileHeader *file_header);
// Prints the generated ASCII image 
void print_image(const BMPImage *image, unsigned int resolution, unsigned int grid_ratio, const char *palette, bool luma_formula);
// Gets the ASCII symbol corresponding to grid_average brightness value
char get_brightness(int grid_average, const char *palette);

#endif
