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
int mean_magnitude = 0;
int sum = 0;
for (int i=0; i < size[0]; i++){
	for (int j=0; j < size[1]; j++){
		image_int[i][j] = (int)image[i][j];
		sum += image_int[i][j];
	}
}
int n_img = size[0]*size[1];
int mean_img = sum/n_img;
int mean_temp;
sum = 0;
int template[roi.height][roi.width];
// fill in the template from the coordinates of the mouse
for (int i=0; i < roi.height; i++){
	for (int j=0; j < roi.width; j++){
		template[i][j] = (int)image[roi.x + i][roi.y + j];
		sum += template[i][j];
	}
}
int n_tmp = roi.height* roi.width;
mean_temp = sum/n_tmp;

// calculate std for image and template
int std_img = 0;
for (int i=0; i < size[0]; i++){
	for (int j=0; j < size[1]; j++){
		std_img += pow(image_int[i][j] - mean_img,2);
	}
}
int std_temp = 0;
for (int i=0; i < roi.height; i++){
	for (int j=0; j < roi.width; j++){
		std_temp += pow(template[i][j] - mean_temp,2);
	}
}
std_img = sqrt(std_img/n_img);
std_temp = sqrt(std_temp/n_tmp);
int max = -1;
int min = 600;
for (int row = 1; row < size[0] - roi.height; row++) {
    for (int col = 1; col < size[1] - roi.width; col++) {
        int cell_val = 0.0; // magnitude of cell
        // calculate for vertical kernel
        for (int a = 0; a < roi.height; a++){
            for (int b = 0; b < roi.width; b++) {

            	int tmp_cell = image_int[row -1 + a][col -1 + b] -mean_img;
            	int tmp_krnl = template[a][b] - mean_temp;
            	cell_val += (tmp_cell * tmp_krnl)/(std_img*std_temp);
            }
        }
        if (cell_val < min)
            min = cell_val;
		if (cell_val > max)
			max = cell_val;
        proc_img_x[row] [col] = cell_val;
   }
}
//int mean = 0;
//for (int i=0; i < size[0] - roi.height +1; i++){
//	for (int j=0; j < size[1] - roi.width +1; j++){
//		mean += proc_img_x[i][j];
//	}
//}
//mean = mean/n_img;
//int std = 0;
//for (int i=0; i < size[0] - roi.height +1; i++){
//	for (int j=0; j < size[1] - roi.width +1; j++){
//		std += pow(proc_img_x[i][j] - mean,2);
//	}
////}
//std = sqrt(std/n_img);
//printf("%d %d \n", mean, std);
// normalize
for (int i = 0; i < size[0]; i++) {
    for (int j = 0; j < size[1]; j++) {
//    	proc_img[i][j] = (unsigned char)(int)(proc_img_x[i][j]);

		proc_img[i][j] = (unsigned char) (int)((proc_img_x[i][j]-min)*(255 - 0)/(max - min));

//    	proc_img[i][j] = (unsigned char)(int)((proc_img_x[i][j]-mean)/std);
//    	if ((int)proc_img[i][j] > 255)
//    		printf("yes");

//    	printf("%d ", (int)proc_img_x[row][col]);
   }
//    printf("\n");

}




}


