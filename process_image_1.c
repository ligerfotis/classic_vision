#include <stdio.h>
#include <math.h>
#include <X11/Xlib.h>

#define DIM 512

/******************************************************************/
/* This structure contains the coordinates of a box drawn with    */
/* the left mouse button on the image window.                     */
/* roi.x , roi.y  - left upper corner's coordinates               */
/* roi.width , roi.height - width and height of the box           */
/******************************************************************/
extern XRectangle roi;


/******************************************************************/
/* Main processing routine. This is called upon pressing the      */
/* Process button of the interface.                               */
/* image  - the original greyscale image                          */
/* size   - the actual size of the image                          */
/* proc_image - the image representation resulting from the       */
/*              processing. This will be displayed upon return    */
/*              from this function.                               */
/******************************************************************/
// pictures/chess.lpr
void process_image(image, size, proc_img)
unsigned char image[DIM][DIM];
int size[2];
unsigned char proc_img[DIM][DIM];
{
int image_int[DIM][DIM];
int proc_img_x[DIM][DIM];
int proc_img_y[DIM][DIM];

// convert char array to int array
for (int i=0; i < DIM; i++){
	for (int j=0; j < DIM; j++){
		image_int[i][j] = (int)image[i][j];
	}
}

// Sobel's Mask
int kernel_ver[3][3] = { { -1, -0, 1},
						{ -2, 0, 2},
						{ 1, 0, 1}};

int kernel_hor[3][3] = { { -1, -2, -1},
							{ 0, 0, 0},
							{ 1, 2, 1}};
int max = -1;
int min = 600;
// start from the second pixel in each dimensions
for (int row = 1; row < size[0] - 1; row++) {
    for (int col = 1; col < size[1] - 1; col++) {
        int cell_val = 0.0; // magnitude of cell
        // calculate the convolution with the kernel
        for (int a = 0; a < 3; a++){
            for (int b = 0; b < 3; b++) {

            	int tmp_cell = image_int[row -1 + a][col -1 + b];
            	int tmp_krnl = kernel_hor[a][b];

            	cell_val += (tmp_cell * tmp_krnl);
            }
        }
        // calculate min and max values of the convoluted image
        if (cell_val < min)
        	min = cell_val;
        if (cell_val > max)
        	max = cell_val;
        // insert the convoluted pixel to the filtered image array
        proc_img_x[row] [col] = cell_val;


   }
}

// normalize filtered image
for (int i=0; i < size[0]; i++){
	for (int j=0; j < size[1]; j++){
		// convert image array back to unsigned char and normalize
		proc_img[i][j] = (unsigned char) (int)((proc_img_x[i][j]-min)*(255 - 0)/(max - min));
	}
}

}


