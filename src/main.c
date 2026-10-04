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
char *save_image = NULL;
bool no_print = false;

void get_bmp_file(char *path);
void usage(void);
void save_generated_image(char *printed);
void get_args(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	if (argc < 2 || strcmp(argv[1], "--help") == 0){
		usage();
		exit(EXIT_FAILURE);
	}
	
	get_args(argc, argv);

	char *palette = hirez_palette ? palette_high: palette_low;

	char *fpath = argv[1];
	get_bmp_file(fpath);

	get_bmp_headers(bmpfile, &info_header, &file_header);
	read_pixel_data(bmpfile, &bmp_image, &info_header, &file_header);
	char *ascii= get_ascii_string(&bmp_image, resolution, grid_ratio, palette, luma_formula);

	free(bmp_image.image);
	fclose(bmpfile);

	if (no_print == false)
		printf("%s", ascii);

	if (save_image != NULL)
		save_generated_image(ascii);

	free(ascii);

	exit(EXIT_SUCCESS);
}

void get_args(int argc, char *argv[]){
	
	for(int i = 2; i<argc; i++){
		if(strcmp(argv[i], "--no-print") == 0 || strcmp(argv[i], "-n") == 0 ){
			no_print = true;
		}
		else if(strcmp(argv[i], "--hirez") == 0 || strcmp(argv[i], "-h") == 0 ){
			hirez_palette = true;
		}
		else if (strcmp(argv[i], "--luma") == 0 || strcmp(argv[i], "-l") == 0){
			luma_formula = true;
		}
		else if (strcmp(argv[i], "--resolution") == 0 || strcmp(argv[i], "-r") == 0){
			if(i+1 < argc){
				char *end;
				long n = strtol(argv[i+1], &end, 10); 

				if (end == argv[i + 1] || *end != '\0') {
					printf("INVALID ARGUMENT: '%s %s'\nSee --help for details", argv[i], argv[i+1]);
					exit(EXIT_FAILURE);
				}
				if (n > 100 || n < 1){
					printf("INVALID USAGE OF: '%s %li'\nSee --help for details", argv[i], n);
					exit(EXIT_FAILURE);
				}

				resolution = n;
				i++;

			}else{
				printf("INVALID USAGE OF: '%s'\nSee --help for details", argv[i]);
				exit(EXIT_FAILURE);
			}
		}
		else if (strcmp(argv[i], "--savepath") == 0 || strcmp(argv[i], "-s") == 0){
			if(i+1 >= argc || argv[i+1][0] == '-'){
				printf("INVALID USAGE OF: '%s'\nSee --help for details", argv[i]);
				exit(EXIT_FAILURE);
			}

			save_image = argv[i+1];
			i++;

		}else{
			printf("INVALID ARGUMENT: '%s'\nSee --help for details", argv[i]);
			exit(EXIT_FAILURE);
		}
	}

	if (no_print == true && save_image == NULL){
		printf("'--no-print' can only be used when '--savepath <path>' is provided\nSee --help for details");
		exit(EXIT_FAILURE);
	}
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

void save_generated_image(char *printed){

	savefile = fopen(save_image, "w");

	if(!savefile){
		printf("Could not save the generated ASCII art to '%s'", save_image); 
		fclose(bmpfile);
		exit(EXIT_FAILURE);
	}

	fputs(printed, savefile);
	fclose(savefile);

	printf("ASCII art saved to '%s'", save_image);
}

void usage(){

	// -h, --hirez
	// -r, --resolution <value>
	// -s, --savepath <path>
	// -l, --luma
	// -n, --no-print
	
	printf("Usage: bmp-ascii.exe <bmp-image-path> [options]\n\n");

	printf("Arguments:\n  <bmp-image-path>\t\t Path to the BMP image (required)\n");
	printf("\t\t\t\t\t NOTE: ONLY 24-BIT UNCOMPRESSED BMP FILES ARE SUPPORTED\n\n");

	printf("Options:\n");
	printf("  -h, --hirez\t\t\t Enables high resolution ASCII palette\n");
	printf("  -l, --luma\t\t\t Enables luma approximation for image grayscale detection\n");
	printf("  -n, --no-print\t\t Disables terminal printing. \n\t\t\t\t\t NOTE: Filepath must be provided with '--savepath'\n");
	printf("  -r, --resolution <value>\t The resolution of generated ASCII art (default 5). \n\t\t\t\t\t NOTE: Lower value means higher quality\n");
	printf("  -s, --savepath <path>\t\t The path where the generated ASCII art is saved\n");
}
