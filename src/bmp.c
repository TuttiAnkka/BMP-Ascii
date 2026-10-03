#include "bmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "stdbool.h"

void get_bmp_headers(FILE *bmpfile, BMPInfoHeader *info_header, BMPFileHeader *file_header){

	// File header
	if (fread(file_header, sizeof(*file_header), 1, bmpfile) != 1){
		printf("Failed to read file header\n");
		fclose(bmpfile);
		exit(EXIT_FAILURE);
	}

	// Only 24-bit uncompressed files are supported.
	// Little endian hexadecimal representation of 0x42 0x4D (ASCII string "BM")
	if (file_header->type != 0x4D42){
		printf("File type is incorrect.\n");
		printf("Please provide a path to an uncompressed 24-bit BMP file!\n");
		fclose(bmpfile);
		exit(EXIT_FAILURE);
	}

	// Info header
	if (fread(info_header, sizeof(*info_header), 1, bmpfile) != 1){
		printf("Failed to read info header\n");
		fclose(bmpfile);
		exit(EXIT_FAILURE);
	}

	//printf("File headers read succesfully!\n");
}

void read_pixel_data(FILE *bmpfile, BMPImage *image, BMPInfoHeader *info_header, BMPFileHeader *file_header){

	// Test prints
	//printf("Width:  %d\n", info_header->width);
	//printf("Height: %d\n", info_header->height);
	//printf("Bits:   %d\n", info_header->color_depth);
	//printf("Compression: %u\n", info_header->compression);

	//printf("Pixel offset:   %u\n", file_header->offset);
	//printf("sizeof(offset):   %zu\n", sizeof(bmp_image.file_header.offset));

	image->width = info_header->width;
	image->height = info_header->height;
	image->abs_height = abs(image->height); // BMP files can have negative height - using absolute values

	// Rows are padded to a multiple of 4 bytes.
	image->row_size = ((image->width * 3 + 3) / 4) * 4;
	image->image = malloc(image->row_size * image->abs_height);

	if (!image->image){
		printf("Memory allocation failed\n");
		fclose(bmpfile);
		exit(EXIT_FAILURE);
	}

	// Jump to pixel array data using offset value
	if (fseek(bmpfile, file_header->offset, SEEK_SET) != 0){
		printf("Failed to read pixel data\n");
		fclose(bmpfile);
		free(image);
		exit(EXIT_FAILURE);
	}

	// Read the whole image
	for (int y=0; y < image->abs_height; y++){
		if (fread(image->image + y * image->row_size, 1, image->row_size, bmpfile) != (size_t)image->row_size){
			printf("Failed to read pixel data\n");
			free(image);
			fclose(bmpfile);
			exit(EXIT_FAILURE);
		}
	}

}


void print_image(const BMPImage *image, unsigned int resolution, unsigned int grid_ratio, const char *palette, bool luma_formula){

	int height = image->height;
	int width = image->width;
	int abs_height = image->abs_height;
	int row_size = image->row_size;

	//printf("Printing started!\n");

	// Columns
	for (int y = 0; y < abs_height; y+=(resolution*grid_ratio)){
		// Rows
		for (int x = 0; x < width; x+=resolution){

			// Average grayscale of grid pixels
			int average = 0; 
			int pixel_count = 0;

			for (int grid_x = x; grid_x < x+resolution && grid_x < width; grid_x++){

				for (int grid_y = y; grid_y < y+(resolution*grid_ratio) && grid_y < abs_height; grid_y++){

					// If BMP is stored bottom-to-top instead of top-to-bottom. Making sure Y is correct
					int actual_y = height > 0 ? abs_height - 1 - grid_y : grid_y;

					unsigned char *row = image->image + actual_y * row_size;

				  unsigned char b = row[grid_x * 3 + 0];
				  unsigned char g = row[grid_x * 3 + 1];
				  unsigned char r = row[grid_x * 3 + 2];
					
					// Grayscale brightness of chosen pixel 
					int gray = luma_formula 
						? (299 * r + 587 * g + 114 * b) / 1000 
						: (r + g + b) / 3;

					average += gray;
					pixel_count++;
				}
			}

			if (pixel_count > 0){
				average /= pixel_count;
			}

			char c = get_brightness(average, palette);
			printf("%c", c);
		}

		printf("\n");
	}

	//printf("Success!");
}

char get_brightness(int grid_average, const char *palette){

	int length = strlen(palette);
	int brightness = (grid_average * (length - 1)) / 255;

	return palette[brightness];
}
