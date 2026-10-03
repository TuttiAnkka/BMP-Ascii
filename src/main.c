#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdbool.h"
#include "bmp.h"

FILE *bmpfile;
FILE *savefile;
BMPImage bmp_image;
BMPInfoHeader info_header;
BMPFileHeader file_header;

char palette_low[] = "@%#*+=-:. "; // Stored in order of brightness. First index is the darkest.
char palette_high[] =
	"$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/"
	"\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";

unsigned int resolution = 5; // Horizontal resolution - Lower = higher
unsigned int grid_ratio = 2; // Vertical resolution - determines the ratio of hor_resolution:ver_resolution
bool luma_formula = false; 
bool hirez_palette = false; 
bool save_image = false;

void get_bmp_file(char *path);
void usage(void);

int main(int argc, char *argv[])
{
	if (argc < 2 || argc > 5 || strcmp(argv[1], "--help") == 0){
		usage();
		exit(EXIT_FAILURE);
	}

	if (argc >= 3)
		resolution = (atoi(argv[2]) > 0 && atoi(argv[2]) < 10) ? atoi(argv[2]) : resolution;
	if (argc >= 4)
		luma_formula = atoi(argv[3]);
	if (argc >= 5)
		hirez_palette = atoi(argv[4]);
	if (argc >= 6)
		save_image = atoi(argv[5]);

	char *palette = hirez_palette ? palette_high: palette_low;

	char *fpath = argv[1];
	get_bmp_file(fpath);

	get_bmp_headers(bmpfile, &info_header, &file_header);
	read_pixel_data(bmpfile, &bmp_image, &info_header, &file_header);
	print_image(&bmp_image, resolution, grid_ratio, palette, luma_formula);

	free(bmp_image.image);
	fclose(bmpfile);

	exit(EXIT_SUCCESS);
}

void get_bmp_file(char *path){

	bmpfile = fopen(path, "rb");

	if(!bmpfile){
		printf("Could not open BMP file");
		fclose(bmpfile);
		exit(EXIT_FAILURE);
	}

	//printf("File opened succesfully!\n");
}

void usage(){
	printf("USAGE:\n");
	printf("./AsciiConvert.exe [IMAGEPATH] [RESOLUTION (1-9)] [LUMA FORMULA (0-1)] [HIREZ PALETTE (0-1)] [SAVEPATH]\n\n");
	printf("MINIMAL EXAMPLE:\n");
	printf("./AsciiConvert.exe Path/To/Image.bmp\n\n");
	printf("COMPLETE EXAMPLE:\n");
	printf("./AsciiConvert.exe Path/To/Image.bmp 5 1 1 Path/To/Save.txt\n\n");
	printf("MINIMAL EXAMPLE DEFAULT VALUES:\n");
	printf("[IMAGEPATH] 5 0 0\n\n");
	printf("NOTE: ONLY 24-BIT UNCOMPRESSED BMP FILES ARE SUPPORTED\n");
}
