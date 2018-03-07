/********************************************************************************************************************************
 * Image Transformations                                                                                                        *
 * 																*
 * Author: Matt Viego													 	*
 *																*
 * Purpose: This program has the ability to read in .bmp files and alter them in a user specified way. The data from the files  *
 * will be stored into structs. The bmp header and dip header are both stored in individual structs and will not be altered.    *
 * Each pixel is read in from the .bmp file, one at a time, into a struct and is altered before being read back into the file.  *
 * Users have the option of grayscaling or inverting the image.                                                                 *
 ********************************************************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#pragma pack(1)

/* Struct to hold the BMP Header */

struct bmp_header {

	char format_identifier[2];
	int file_size;
	short reserve1;
	short reserve2;
	int offset;
};

/* Struct to hold the DIB Header */

struct dib_header{
	int dib_size;
	int width;
	int height;
	short color_planes;
	short bits_per_pixel;
	int compression_scheme;
	int image_size;
	int horizontal_resolution;
	int vertical_resolution;
	int num_colors_palette;
	int num_colors_important;
};

/* Struct tht holds each pixel */

struct pixel{
	char blue;
	char green;
	char red;
};

int isFormatCorrect(struct bmp_header *bmp, struct dib_header *dib); /* Function Declaration */

/* ******************************
 * Function - isFormatCorrect() *
 *****************************************************************************************
 * Parameters - 1. Struct Pointer for BMP Header
 *		2. Struct Pointer for DIB Header
 * 
 * This function is responsible for checking to ensure that we have a valid .bmp file. If 
 * any credentials in the if statement do not match up then the program will display an 
 * error message and return 0, ending the program. Otherwise will return 1.
 *
 */

int isFormatCorrect(struct bmp_header *bmp, struct dib_header *dib){

	if (strncmp(bmp->format_identifier, "BM", 2) != 0 || dib->dib_size != 40 || dib->bits_per_pixel != 24){
		printf("This program does not support this file format.");
		return 0;
	}

	else{
		return 1;
	}
}


int main(int argc, char* argv[]){

	FILE *f;		/* file pointer to hold refernce to .bmp file */		
	struct bmp_header h1;  	/* instance of bmp_header */
	struct dib_header h2;	/* instance of dib_header */
	struct pixel p;		/* instance of pixel */
	int i, j;		/* integers used for trafersing through file with for loops */
	float y;		/* floating point value for operations in grayscale */
	
	f = fopen(argv[2], "rb+");	/* Opens file in binary mode for reading and writing */

	fread(&h1, sizeof(struct bmp_header), 1, f);	/* Reads in the BMP Header */
	fread(&h2, sizeof(struct dib_header), 1, f);	/* Reads in the DIB Header */

	fseek(f, 10, SEEK_SET);				/* Sets file pointer to 10 bytes after the beginning of file */
	fseek(f, h1.offset, SEEK_CUR);			/* Sets file pointer to location of pixel start */

	if (isFormatCorrect(&h1, &h2)){
		int padding = (4-((sizeof(struct pixel) * h2.width)%4))%4;		/* Finds amount of padding to account for */
		if(strcmp(argv[1], "-invert") == 0){					
			for(i = 1; i <= h2.height; i++){				/* for loops traverse through each row of pixels from left to right */
				for(j = 1; j <= h2.width; j++){
					fread(&p, sizeof(struct pixel), 1, f);		/* reads in one pixel */
					p.blue = ~p.blue;				/* sets blue scale to the negation of the blue value */
					p.green = ~p.green;				/* negates green */
					p.red = ~p.red;					/* negates red */
					fseek(f, -3, SEEK_CUR);				/* puts file pointer at beginning of pixel for writing */
					fwrite(&p, 1, sizeof(struct pixel), f);		/* writes pixel back into the file */
				}
				fseek(f, padding, SEEK_CUR);				/* Moves file pointer past padding upon reaching the end of a row */
			} 	
		}
		if(strcmp(argv[1], "-grayscale") == 0){
			 for(i = 1; i <= h2.height; i++){
                                for(j = 1; j <= h2.width; j++){
                                        fread(&p, sizeof(struct pixel), 1, f);

					/* The following lines are from the equation given in the assignment using appropriate casting. */
						
					float blue  = ((float)p.blue)/255; float green = ((float)p.green)/255; float red = ((float)p.red)/255;  
					y = (0.2126*red) + (0.7152*green) + (0.0722*blue);							
					
					if (y <= 0.0031308){
						y = y * 12.92;
					}
					else {
						y = pow(y, (1/2.4));
						y = y * 1.055;
						y = y - 0.055;
					}
					y = y*255;
					p.blue = (char)y; p.green = (char)y; p.red = (char)y;

					fseek(f, -3, SEEK_CUR);
                                        fwrite(&p, sizeof(struct pixel), 1, f);
                                }
				fseek(f, padding, SEEK_CUR);
                        }

		}

	}

	fclose(f); 	/* Close file upon completion */

	return 0;
}
